#include <WiFi.h>
#include <HTTPClient.h>

#define MILLIS_PER_MIN 60000
#define MILLIS_PER_SEC 1000

const char* ssidWifi = "TIM-91746045";
const char* passWifi = "1Oj3eyR5qHD3jAaT5Jfj1Ooh";
/*
const char* ssidWifi = "TORNATOREwifi";
const char* passWifi =  "finalborgo";
*/
const String username = "riccardo-office-temperature-sensor-username";
const String password =  "riccardo-office-temperature-sensor-password";
const String url = "http://students.atmosphere.tools";
const String ver = "v1";
const String login = "login";
const String devs = "devices";
const String scps = "scripts";
const String feature="temperature";
const String measurements = "measurements";
const String id = "temperature-sensor-riccardo-office";
const String device = "temperature-sensor-riccardo-office";
const String thing = "riccardo-office";

String token;
int httpsCode;
String response;
String scripts;
String scriptsCode;
String scriptsCounter;
String measureInterval;

double startLogCount=0;
double startGetCount=0;
//double startPostCount=0;

int tokenDuration=1800;//30 minutes= 1800 seconds
int period=10;


double logCount=(double)tokenDuration;
double getCount=(double)period;
//double postCount=(double)postInterval;


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
        
  token = POSTLogin(username, password); // Authentication
  startLogCount = millis();
  
  response = GETDescr(token); // Get the virtual description
  period = ParseResponse(response,"period")!="" ? ParseResponse(response,"period").toInt():600;
  scripts = ParseResponse(response,"scripts");
  scriptsCode = retrieveScriptsCode(token, scripts);
  startGetCount = millis();
  
  /*POSTvalues(token,"2","temperature","average-hourly-temperature"); // Post measurement
  startPostCount = millis();*/
}

void loop() {
  if ((WiFi.status() == WL_CONNECTED)) //Check the current connection status
  { 
    /*postCount = (double)( millis()-startPostCount )/MILLIS_PER_SEC;
    if( postCount >= postInterval ){
      startPostCount = millis();
      POSTvalues(token,"2","temperature","average-hourly-temperature"); // Post measurement
    }*/
    
    getCount = (double)( millis()-startGetCount )/MILLIS_PER_SEC;
    if( getCount >= period ){
      startGetCount = millis(); 
      response = GETDescr(token); // Get the scripts
      period = ParseResponse(response,"period")!="" ? ParseResponse(response,"period").toInt():5;
      scripts = ParseResponse(response,"scripts");
      scriptsCode= retrieveScriptsCode(token, scripts);
    }
    
    logCount = (double)( millis()-startLogCount )/MILLIS_PER_SEC;      
    if( logCount >= tokenDuration ){ //every "tokenDuration" authenticate
      startLogCount = millis();
      token = POSTLogin(username, password); //Authentication
    }  
   
    
    executeScripts(scriptsCode);
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

//DA COLLAUDARE
String retrieveScriptsCode(String token, String scripts){
  int startIndex=0;
  int endIndex=0;
  String code;
  String tempCode;
  
  while( scripts.indexOf(",", startIndex)!=-1){
    endIndex=scripts.indexOf(",",startIndex);
    tempCode=GETScript( token, scripts.substring(startIndex+1,endIndex) );
    
    measureInterval.concat( ParseResponse(tempCode,feature) + "," );//get send interval
    code.concat( tempCode + ",");//get all code
    
    startIndex=endIndex+1;
  }
  measureInterval.remove( measureInterval.length()-1 ); //remove the last ","
  code.remove( code.length()-1 ); //remove the last ","
  
  return code;
}
//DA COLLAUDARE
void executeScripts(String scriptsCode){
  int startIndex=0;
  int endIndex=0;
  int counterIndex=0;
  
  while( scriptsCode.indexOf(",", startIndex)!=-1){
    endIndex=scriptsCode.indexOf(",",startIndex);
    
    executeCode( scriptsCode.substring(startIndex+1,endIndex), 3 ); ////////////da cambiare il 3
    
    startIndex=endIndex+1;
  }
}
void executeCode(String stringCode,int interval){
  //DA IMPLEMENTARE
  
}
//DA COLLAUDARE
int ParseIntervalToSec(String interval){
  int numberValue = ( interval.substring(0,interval.length()-1) ).toInt();
  
  switch( interval.charAt(interval.length()-1) ){
    case 's'://do nothing is already in Second
      break;
    case 'm':// minutes
      numberValue*60;
      break;
    case 'h':// hours
      numberValue*60*60;
      break;
    case 'd':// days
      numberValue*60*60*24;
      break;
    default:// if no measure unit is indicate, interpreted as Seconds
      break;
  }
  return numberValue;
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
/*String POSTNewMeasurement (String token){
    HTTPClient https;
    https.begin(url+"/"+ver+"/"+measurements, root_ca); //Specify the URL and certificate
   
    https.addHeader("Content-Type","application/json");
    https.addHeader("Authorization",token);
    
    httpsCode = https.POST("{\"thing\": \"Smart_Home\", \"tag\": \"Edge_Device\", \"feature\": \"Heating\", \"device\": \"Thermostat\", \"samples\": {\"numbers\":[1]}}" );//this is the body
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
}*/

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

//NOT WORKS WITH CUSTOM BECAUSE IT DELETE ANY WHITESPACES
String ParseResponse( String response, String fieldName ){
  
  if( response.indexOf(fieldName) ==-1){
    Serial.println("field name is not present!");
    return "";
  }
  response.replace(" ","");//delete whitespace
  int beginOfValue = response.indexOf( ":", response.indexOf(fieldName) )+1;//find starting index of field value
   
  char firstChar = response.charAt( beginOfValue );//this is a delimiter: " or [ or {
  int endOfValue;
  
  switch (firstChar){
    case '\"':
      endOfValue = response.indexOf(firstChar,beginOfValue+1);// start looking for the last delimiter from the next value
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


int FindEndIndex (char first,char last, int startIndex,String response){
  String delimiters="";
  
  int nextClose;
  int nextOpen;
  while(true){
    nextOpen = response.indexOf( first, startIndex );
    nextClose = response.indexOf( last, startIndex );
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
       Serial.println(delimiters);
       delimiters.remove(delimiters.length()-1);
       Serial.println(delimiters);
       startIndex=nextClose+1;
    }
    //if the next one is a open bracket
    else{
      delimiters.concat(first);
      startIndex=nextOpen+1;
    }
  }
  return nextClose;
}

double filterOp(String condition,double value){
  
}

double maxValue=0;
double maxOp(double value){
  if(value>maxValue){
    maxValue=value;
    return value;
  }
  return NULL;
}
double mapOp(String function,double value){
  
}
double windowOp(String function,double initial,double windowSize,double value){
  
}
double slidingWindowOp(String function,double initial,double windowSize,double value){
  
}
