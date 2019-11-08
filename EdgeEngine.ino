#include <WiFi.h>
#include <HTTPClient.h>

#define MILLIS_PER_MIN 60000
#define MILLIS_PER_SEC 1000
const char* ssid = "TORNATOREwifi";
const char* password =  "finalborgo";
String token;


int httpsCode;
String response;
String script;
double startMin=0;
double startSec=0;
double minsElapsed=0;
double secsElapsed=5;
boolean gotScript=false;
/*
const char* ssid = "TIM-91746045";
const char* password = "1Oj3eyR5qHD3jAaT5Jfj1Ooh";
*/
/*
const char* ssid = "S7Chicco";
const char* password = "LLLLLLLL";
*/
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
/**
 * setup
 */
void setup() {
  Serial.begin(115200);
  delay(2000);
  WiFi.begin(ssid, password); 
  while (WiFi.status() != WL_CONNECTED) {
    delay(2000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");

}

void loop() {
  if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status
    
    token=POSTLogin();//Authentication 
    
    startMin=millis();//start conuters
    startSec=millis();
    
    if(!gotScript){// retrieve the script only once
      script=GETScript(token);
      Serial.println(script);
      gotScript=true;
   }
    
   while( minsElapsed<=29.5 ){//every 29.5 mins authenticate
      
      if( secsElapsed >=5){//every 5 seconds Post a new measurement
        POSTNewMeasurement(token);
        minsElapsed=(double)( millis()-startMin )/MILLIS_PER_MIN;
        startSec=millis();
      }
      
      secsElapsed=(double)( millis()-startSec )/MILLIS_PER_SEC;
    }
  }
  
}

String POSTLogin (){
    HTTPClient https;
    https.begin("https://api.atmosphere.tools/v1/login", root_ca); //Specify the URL and certificate
    
    https.addHeader("Content-Type","application/json");//; charset=utf-8
    httpsCode = https.POST("{\"username\": \"admin\",\"password\": \"admin\"}");//this is the body
 
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
String GETScript(String token){
   HTTPClient https;
   https.begin("https://api.atmosphere.tools/v1/scripts/script_sample_1", root_ca); //Specify the URL and certificate

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

   return ParseResponse(response,"custom");
}
String POSTNewMeasurement (String token){
    HTTPClient https;
    https.begin("https://api.atmosphere.tools/v1/measurements", root_ca); //Specify the URL and certificate
   
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
}

String ParseToken(String token){
  return token.substring( token.indexOf("J"), token.lastIndexOf("\"") );
}

//NOT WORKS WITH CUSTOM BECAUSE IT DELETE ANY WHITESPACES
String ParseResponse( String response, String fieldName ){
  response.replace(" ","");//delete whitespace
  
  int beginOfValue = response.indexOf( ":", response.indexOf(fieldName) )+1;//find starting index of field value
  
  if(beginOfValue==-1)
    return "field name is wrong";
    
  char firstChar = response.charAt( beginOfValue );//this is a delimiter: " or [ or {
  int endOfValue;
  
  switch (firstChar){
    case '\"':
      endOfValue = response.indexOf(firstChar,beginOfValue+1);// start looking for the last delimiter from the next value
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
