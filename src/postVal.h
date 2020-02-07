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

  //methods
  int parseNumOfSamples(String);

  public:
  //constructors
  postVal(String,String,String,String,String,String,String);
  //destructor
   ~postVal();
  
  //methods
  sample* execute();

  //variables
  vector<sample*> batch;
  
  //setters
  void setToken(String);
  void setFeature(String);
  void setScriptId(String);
};
//constructors
postVal::postVal(String opName, String thing, String device, String url, String token, String feature, String scriptId):operation(opName){
  valid=true;
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

 postVal::~postVal(){
   for(int i=0;i<batch.size();i++){
    delete batch[i];
  }
  //  for ( auto sam : batch ){
  //    delete sam;//call destructor for each sample
  //  }
   batch.clear();
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
  
  if(input!=NULL && counter < numOfSamples){ // untill we have not enough values 
    
    input->url=url;
    input->thing=thing;
    input->device=device;
    input->scriptId=scriptId;
    batch.push_back(input);// save the input in a local batch

    counter++;
  }
  
  if( input!=NULL && counter >= numOfSamples){
    counter=0;
    // j is useful to count the number of iteration equal to batch size; 
    // since after post the first element we erase it, the next one shift to the first position so access batch[0] till end
    for(int j=0; j<numOfSamples; j++){
      //if( Api->POSTMeasurement(*batch[j], token) )
        Api->POSTMeasurement(*batch[j], token);
        delete batch[j];// if the value has been correctly POSTed delete it
    }
    batch.clear();//remove all samples
    
    return NULL;
  }
  return NULL;//this means the input is not been POSTed
}

int postVal::parseNumOfSamples( String numString){
  int numberValue=1; 
  
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
