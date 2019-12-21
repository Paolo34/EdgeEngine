//send.h

#ifndef send_h
#define send_h
#include "operation.h"

class postVal : public operation{
  private:
  //variables
  String token;
  String thing;
  String device;
  String feature;
  String scriptId;
  String url;
  APIRest* Api;
  int numOfSamples;
  int counter;
  int numberValue;
  int j;

  //methods
  int parseNumOfSamples(String);

  public:
  //constructors
  postVal(String,String,String,String,String,String,String);
  
  //methods
  double* execute();

  //variables
  vector<measureData> batch;
  
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
  numOfSamples = parseNumOfSamples(opName.substring( opName.indexOf("(")+1, opName.indexOf(")") ));
  counter=0; 
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
  
  if(&input!=NULL && counter < numOfSamples){ // untill we have not enough values 
    measureData datum; // we want to save datum that will be sent alone or in group
    datum.value=input;
    datum.date=Api->getActualDate();
    datum.url=url;
    datum.thing=thing;
    datum.feature=feature;
    datum.device=device;
    datum.scriptId=scriptId;
    batch.push_back(datum);// save the datum in a local batch
    counter++;
  }
  
  if( &input!=NULL && counter >= numOfSamples){
    counter=0;
    // j is useful to count the number of iteration equal to batch size; 
    // since after post the first element we erase it, the next one shift to the first position so access batch[0] till end
    for(j=0; j<numOfSamples; j++){
      Api->POSTMeasurement(batch[0].url, token, batch[0].thing, batch[0].feature, batch[0].device, batch[0].scriptId, batch[0].value, batch[0].date);
      batch.erase( batch.begin() );
    }
    
  }
  return NULL;//this should block the execution of the next operation
}

int postVal::parseNumOfSamples( String numString){
  numberValue=1; 
  
  if(numString!="") // if there is no number we assign 1 because we post one measurement at a time 
    if(!isaNumber(numString))
    {
      valid=false;
      return numberValue;
    }
    numberValue = numString.toInt();
  
  return numberValue;
}

#endif 