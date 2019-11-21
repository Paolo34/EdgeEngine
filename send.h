//send.h

#ifndef send_h
#define send_h
#include "operation.h"

class postVal : public operation{
  private:
  String token;
  String thing;
  String device;
  String feature;
  String scriptId;
  String url;

  public:
  //constructors
  postVal(String);
  postVal(String,String,String,String,String,String,String);
  
  //methods
  double execute();

  //setters
  void setToken(String);
  void setFeature(String);
  void setScriptId(String);
};
//constructors
postVal::postVal(String opName, String thing, String device, String url, String token, String feature, String scriptId):operation(opName){
  this->thing=thing;
  this->device=device;
  this->url=url;
  this->token=token;
  this->feature=feature;
  this->scriptId=scriptId;
}
//setters
 void postVal::setToken(String token){
   this->token=token;
 }
 void postVal::setFeature(String feature){
   this->feature=feature;
 }
 void postVal::setScriptId(String scriptId){
   this->scriptId=scriptId;
 }

//methods
double postVal::execute() {
  
  if( input!=NULL ){
    HTTPClient https;
    https.begin(url); //Specify the URL and certificate
    https.addHeader("Content-Type","application/json");
    https.addHeader("Authorization",token);
    int httpsCode = https.POST("{\"thing\": \""+thing+"\", \"feature\": \""+feature+"\", \"device\": \""+device+"\", \"script\": \""+scriptId+"\", \"samples\": {\"values\":"+input+"}}" );//this is the body
    if (httpsCode > 0) { //Check for the returning code
        Serial.println(httpsCode);
        Serial.println(https.getString());
    }
    else {
      Serial.printf("[HTTPS] POST NewMeas... failed, error: %s\n", https.errorToString(httpsCode).c_str());
    }

    https.end(); //Free the resources
    
    return httpsCode;
  }
  return NULL;//this should block the execution of the next operation
}


#endif 
