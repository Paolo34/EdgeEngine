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
  APIRest* Api;

  public:
  //constructors
  postVal(String);
  postVal(String,String,String,String,String,String,String);
  
  //methods
  double* execute();

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
  Api=APIRest::getInstance();
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
double* postVal::execute() {
  
  if( &input!=NULL ){
    Api->POSTMeasurement(url,token,thing,feature,device,scriptId,input);
  }
  return NULL;//this should block the execution of the next operation
}


#endif 
