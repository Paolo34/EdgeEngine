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
  sample* execute();

  //variables
  vector<sample> batch;
  
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
sample* postVal::execute() {
  
  if(&input!=NULL && counter < numOfSamples){ // untill we have not enough values 
    
    input->url=url;
    input->thing=thing;
    input->device=device;
    input->scriptId=scriptId;
    batch.push_back(*input);// save the input in a local batch

    counter++;
  }
  
  if( &input!=NULL && counter >= numOfSamples){
    counter=0;
    // j is useful to count the number of iteration equal to batch size; 
    // since after post the first element we erase it, the next one shift to the first position so access batch[0] till end
    for(j=0; j<numOfSamples; j++){
      Api->POSTMeasurement(batch[0], token);
      batch.erase( batch.begin() );//delete the first value from the batch
    }
     return input;//return last input
  }
  return NULL;//this should block the execution of the next operation
}

int postVal::parseNumOfSamples( String numString){
  numberValue=1; 
  
  if(numString!="")// if there is no number we assign 1 because we post one measurement at a time 
  {
	if(!isaNumber(numString))
    {
      valid=false;
      return numberValue;
    }
    numberValue = numString.toInt();
  }	 
  return numberValue;
}

#endif 
