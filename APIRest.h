//APIRest.h

#ifndef APIRest_h
#define APIRest_h

#include <HTTPClient.h>

//SINGLETON
class APIRest{

  private:

  //variables
  int httpsCode;
  String response;
  
  //constructor
  APIRest();
  
  //variables
  static APIRest* instance;
  
  //methods
  
  public:
  //variables
  
  //methods
  static APIRest* getInstance();
  String POSTLogin(String,String,String);
  String POSTMeasurement(String,String,String,String,String,String,double);
  String GETDescr(String,String);
  String GETScript(String,String);
  
};

APIRest* APIRest::instance=NULL;

APIRest* APIRest::getInstance(){
  if(instance==NULL){
    instance= new APIRest();
  }
  return instance;
}

APIRest::APIRest(){
}

String APIRest::POSTLogin (String url, String username, String password){
  HTTPClient https;
  https.begin(url); //Specify the URL and certificate
  
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
  
  return response;
}

String APIRest::GETDescr(String url, String token){
   HTTPClient https;
   https.begin(url); //Specify the URL and certificate

   https.addHeader("Authorization",token);
   
   httpsCode = https.GET();//the body is empty
   if (httpsCode > 0) { //Check for the returning code
        response = https.getString();
        Serial.println(httpsCode);
        Serial.println(response);
   }
   else {
     response="none";
     Serial.printf("[HTTPS] GET Description... failed, error: %s\n", https.errorToString(httpsCode).c_str());
   }
   https.end(); //Free the resources

   return response;
}

String APIRest::GETScript(String url, String token){
   HTTPClient https;
   https.begin(url); //Specify the URL and certificate

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

String APIRest::POSTMeasurement(String url,String token,String thing,String feature,String device,String scriptId,double input){
  HTTPClient https;
  https.begin(url); //Specify the URL and certificate
  https.addHeader("Content-Type","application/json");
  https.addHeader("Authorization",token);
  int httpsCode = https.POST("{\"thing\": \""+thing+"\", \"feature\": \""+feature+"\", \"device\": \""+device+"\", \"script\": \""+scriptId+"\", \"samples\": {\"values\":"+input+"}}" );//this is the body
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

#endif
