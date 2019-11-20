using std::vector;

#include <WiFi.h>
#include <HTTPClient.h>
#include "operation.h"
#include "script.h"
#define MILLIS_PER_MIN 60000
#define MILLIS_PER_SEC 1000
/*
const char* ssidWifi = "TIM-91746045";
const char* passWifi = "1Oj3eyR5qHD3jAaT5Jfj1Ooh";
*/
const char* ssidWifi = "TORNATOREwifi";
const char* passWifi =  "finalborgo";

const String username = "riccardo-office-temperature-sensor-username";
const String password =  "riccardo-office-temperature-sensor-password";
//route
const String url = "http://students.atmosphere.tools";
const String ver = "v1";
const String login = "login";
const String devs = "devices";
const String scps = "scripts";
const String measurements = "measurements";

const String thing = "riccardo-office";
const String device = "temperature-sensor-riccardo-office";
const String id = "temperature-sensor-riccardo-office";
const String feature="temperature";
//the operations allowed are in script.h
//vector<String> allowedOperations {"accept","max","min","send"};

String token;
int httpsCode;
String response;
vector<script> scripts;
String scriptsId; // id of the scripts

double startLogCount=0;// starting instant of counter of Login
double startGetCount=0;// starting instant of counter of Get scripts

int tokenDuration=1800;//30 minutes= 1800 seconds; interval between two login
int period=120;//2 minutes; interval between two GET script

double logCount=(double)tokenDuration;
double getCount=(double)period;

int i;

/*
const char* ssid = "S7Chicco";
const char* password = "LLLLLLLL";
*/
/*
const char* root_ca= \
"-----BEGIN CERTIFICATE-----\n" \
"MIIDJTCCAg2gAwIBAgIJAL2SSUeiac7lMA0GCSqGSIb3DQEBCwUAMGAxLTArBgNV\n" \
"BAMMJEJpdGRlZmVuZGVyIFBlcnNvbmFsIENBLmF2ZnJlZTAwMDAwMDEMMAoGA1UE\n" \
"CwwDSURTMRQwEgYDVQQKDAtCaXRkZWZlbmRlcjELMAkGA1UEBhMCVVMwHhcNMTkw\n" \
"OTA0MTQzOTE4WhcNMTkxMjAzMTQzOTE4WjAfMR0wGwYDVQQDExRsM3AuYXRtb3Nw\n" \
"aGVyZS50b29sczCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAKIv/oZH\n" \
"GYvosa9Hpga7V9G065/+MzgKKhOqJXkEF1YTkyQ7l395RyHHm6qYxRTunh405/il\n" \
"KrtZIcX3wBfDYjPuvBk28KME17zlWfW1ZZfFoJjjrx1yeOD6lvDNNZgnt1GRZCRX\n" \
"JcrSQR67ihcvn6Qp3ItdxzItU5Xoet9+ARiyjcF1qOomBPsrm21zXeclbhGAFJqT\n" \
"CyDMWIahZByNN6deurTX2+CGGj0z89mujwfT0MtK8LQxaZ6FrmMXC6q+dcLIITuc\n" \
"jFPRqOqqIPK7p3nLn9T9U6/BKRmrN8tMDiYDj6ho6At5vAVpFJRjvrcrJWBCSF4o\n" \
"kGHcE6h/6nabUUsCAwEAAaMjMCEwHwYDVR0RBBgwFoIUbDNwLmF0bW9zcGhlcmUu\n" \
"dG9vbHMwDQYJKoZIhvcNAQELBQADggEBAAtQsteDHMM0aWgAj2KpjTL0h2Aq9DQw\n" \
"7RgFOOBscmDJBOJr8/Kg7SRnLuX0Xgngo9hLkWRDEm4Z8w+qtXWYBESgeKIGG8wM\n" \
"OE+uiZG2W6+sMijUVFq1ZO8ZEqlmt8aF/TF3syPNiZiJWzhqNnO+Ihm7a+xBGe2d\n" \
"Nd89cDp3n1fFG3Go0hB25CkObVMRwisDlZ/J9OEONoS+SVmzzOmiXD38ifphP1EZ\n" \
"QUdm499dycwPutvUXeO0aqfNE7ONA/GIeDy+XKeake6SlqsJWLko2oBSy8HDZu1j\n" \
"66BLarM52c/65MkI8e9XnjXpeSGQ4x6+4qKpapQyj3xOLKqVEYnEZbM=\n" \
"-----END CERTIFICATE-----\n"; //Atmosphere CA
*/
/**
 * setup
 */
void setup() {
  Serial.begin(115200);
  delay(2000);
  WiFi.begin(ssidWifi, passWifi); 
  while (WiFi.status() != WL_CONNECTED) {
    delay(2000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  
  startLogCount = millis();      
  token = POSTLogin(username, password); // Authentication
  
  startGetCount = millis();
  response = GETDescr(token); // Get the virtual description
  //period = ParseResponse(response,"period")!="" ? ParseResponse(response,"period").toInt():600; // default 10 minutes
  scriptsId = ParseResponse(response,"scripts");
  
  retrieveScriptsCode(token, scriptsId);
}

void loop() {
  if ((WiFi.status() == WL_CONNECTED)) //Check the current connection status
  { 
    getCount = (double)( millis()-startGetCount )/MILLIS_PER_SEC;
    if( getCount >= period ){
      startGetCount = millis(); 
      response = GETDescr(token); // Get the scripts
      //period = ParseResponse(response,"period")!="" ? ParseResponse(response,"period").toInt():600; //default 10 minutes
      scriptsId = ParseResponse(response,"scripts");
      
      retrieveScriptsCode(token, scriptsId);
    }
    
    logCount = (double)( millis()-startLogCount )/MILLIS_PER_SEC;      
    if( logCount >= tokenDuration ){ //every "tokenDuration" authenticate
      startLogCount = millis();
      token = POSTLogin(username, password); //Authentication
      setToken(token); // Update the token in each script
    }  
   
    executeScripts();
  }
}

String POSTLogin (String username, String password){
  HTTPClient https;
  https.begin(url+"/"+ver+"/"+login); //Specify the URL and certificate
  
  https.addHeader("Content-Type","application/json");
  httpsCode = https.POST("{\"username\": \"" + username + "\",\"password\": \"" + password + "\"}");//this is the body

  if (httpsCode > 0) { //Check for the returning code
      response = https.getString();
      Serial.println(httpsCode);
      Serial.println(response);
  }
  else {
    response="none";
    Serial.printf("[HTTPS] POST Login... failed, error: %s\n", https.errorToString(httpsCode).c_str());
  }
  
  https.end(); //Free the resources
  return ParseToken( response );
}

String GETDescr(String token){
   HTTPClient https;
   https.begin(url+"/"+ver+"/"+devs+"/"+id); //Specify the URL and certificate

   https.addHeader("Authorization",token);
   
   httpsCode = https.GET();//the body is empty
   if (httpsCode > 0) { //Check for the returning code
        response = https.getString();
        Serial.println(httpsCode);
        Serial.println(response);
   }
   else {
     response="none";
     Serial.printf("[HTTPS] GET Script... failed, error: %s\n", https.errorToString(httpsCode).c_str());
   }
   https.end(); //Free the resources

   return response;
}

int startIndex;
int endIndex;
String tempCode;
String scriptId;

void retrieveScriptsCode(String token, String scriptsId){
  startIndex=1;
  endIndex=1;

  Serial.println("retrieveScriptsCode :"+scriptsId);
  scriptsId.replace(" ","");//delete whitespace
  
  while( startIndex < scriptsId.length() ){
    
    endIndex=scriptsId.indexOf("\"",startIndex+1); // start the search from the next charater
    
    scriptId= scriptsId.substring(startIndex,endIndex);
    tempCode=ParseResponse(GETScript( token, scriptId ),"code");
    
    //verify if it is a new script
    for(i=0;i<scripts.size();i++){
      if(scripts[i].scriptId==scriptId && scripts[i].scriptStr==tempCode ){ //if there is already this script
        Serial.println("Script Unchanged: "+scriptId);
        scripts[i].valid=true; // set it to valid because it is already in the API
        goto cnt; //is already present so do nothing an pass to retrieve next script
      }
      else if(scripts[i].scriptId==scriptId){ //if there is already this script but the code has changed
        Serial.println("Script changed: "+scriptId);
        scripts[i].valid=false;// delete the old version of the script and then create the new version of it  
        //scripts.erase(scripts.begin()+i); // delete the old version of the script and then create the new version of it  
      }
    }    
    
    //create the script
    scripts.push_back( script(scriptId,tempCode, thing, device, url+"/"+ver+"/"+measurements, token, feature) );
    if(scripts.back().valid==false){//if the creation of the script has failed      
      scripts.pop_back();//remove last script
    }
    
    cnt:; //go directly there if a script already exists and it is not changed
    
    startIndex=endIndex+3;//+3 because we want to avoid: "," characters between two scripts id
  }
  
  //  delete scripts that are not valid anymore (deleted)
  for(i=0;i<scripts.size();i++){
    if(scripts[i].valid==false){
      Serial.println("Script deleted: "+scripts[i].scriptId);
      scripts.erase(scripts.begin()+i);
      i--; //since we deleted a script even if the scripts size stays unchanged every position is shifted by one
    }
    else{
      Serial.println("Script valid: "+scripts[i].scriptId);
      scripts[i].valid=false;// preset valid to false, to be reconfirmed on the next check in the API
    }
  }
  Serial.println("There are: "+String(scripts.size())+" scripts");
}


void executeScripts(){
  for(i=0;i<scripts.size();i++){
    scripts[i].execute();
  }
}

void setToken(String token){
  for(i=0;i<scripts.size();i++){
    scripts[i].setToken(token);
  }
}

String GETScript(String token, String script){
   HTTPClient https;
   https.begin(url+"/"+ver+"/"+scps+"?filter={\"_id\":\""+script+"\"}"); //Specify the URL and certificate

   https.addHeader("Authorization",token);
   
   httpsCode = https.GET();//the body is empty
   if (httpsCode > 0) { //Check for the returning code
        response = https.getString();
        Serial.println(httpsCode);
        Serial.println(response);
   }
   else {
     response="none";
     Serial.printf("[HTTPS] GET Script... failed, error: %s\n", https.errorToString(httpsCode).c_str());
   }
   https.end(); //Free the resources

   return response;
}


String POSTvalues (String token, String values, String feature, String script){
    HTTPClient https;
    https.begin(url+"/"+ver+"/"+measurements); //Specify the URL and certificate
   
    https.addHeader("Content-Type","application/json");
    https.addHeader("Authorization",token);
    
    httpsCode = https.POST("{\"thing\": \""+thing+"\", \"feature\": \""+feature+"\", \"device\": \""+device+"\", \"script\": \""+script+"\", \"samples\": {\"values\":["+values+"]}}" );//this is the body
    if (httpsCode > 0) { //Check for the returning code
        response = https.getString();
        Serial.println(httpsCode);
        Serial.println(response);
    }
    else {
      response="none";
      Serial.printf("[HTTPS] POST NewMeas... failed, error: %s\n", https.errorToString(httpsCode).c_str());
    }

    https.end(); //Free the resources
    
    return response;
}


String ParseToken(String token){
  return token.substring( token.indexOf("J"), token.lastIndexOf("\"") );
}

int beginOfValue;
int endOfValue;
//NOT WORKS WITH CUSTOM BECAUSE IT DELETES ANY WHITESPACES
String ParseResponse( String response, String fieldName ){
  
  if( response.indexOf(fieldName) ==-1){
    Serial.println(fieldName+" field is not present!");
    return "";
  }
  response.replace(" ","");//delete whitespace
  beginOfValue = response.indexOf( ":", response.indexOf(fieldName) )+1;//find starting index of field value
  
  switch ( response.charAt( beginOfValue ) ){
    case '\"':
      endOfValue = response.indexOf('\"',beginOfValue+1);// start looking for the last delimiter from the next value
      break;
    case '(':
      endOfValue = FindEndIndex('(',')', beginOfValue+1,response);
      break;
    case '[':
      endOfValue = FindEndIndex('[',']', beginOfValue+1,response);
      break;
    case '{':
      endOfValue = FindEndIndex('{','}', beginOfValue+1,response);
      break;
  }
    
  
  return response.substring( beginOfValue+1, endOfValue);
}

int nextClose;
int nextOpen;
String delimiters="";

int FindEndIndex (char first,char last, int start,String response){
  while(true){
    nextOpen = response.indexOf( first, start );
    nextClose = response.indexOf( last, start );
    //if no more open/close bracket is found it returns -1 
    if(nextOpen==-1)
      nextOpen=nextClose+1;
    if(nextClose==-1)
      nextClose=nextOpen+1;
      
    //if the next one is a close bracket and no bracket in the stack
    if ( nextClose<nextOpen && delimiters.length()==0 ){
       break;
    }
      //if the next one is a close bracket and an open bracket is at the end of the stack  delimiters.charAt(delimiters.length()-1)==first){
    else if( nextClose<nextOpen && delimiters.length()!=0){
       delimiters.remove(delimiters.length()-1);
       start=nextClose+1;
    }
    //if the next one is a open bracket
    else{
      delimiters.concat(first);
      start=nextOpen+1;
    }
  }
  return nextClose;
}
