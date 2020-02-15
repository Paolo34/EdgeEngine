//send.h

#ifndef send_h
#define send_h
using std::string;
#include <string>

#include "operation.h"


class postVal : public operation{
  private:
  //variables
  string token;
  string thing;
  string device;
  string feature;
  string scriptId;
  string url;
  APIRest* Api;
  int numOfSamples;
  int counter;

  //methods
  int parseNumOfSamples(string);

  public:
  //constructors
  postVal(string,string,string,string,string,string,string);
  //destructor
   ~postVal();
  
  //methods
  sample* execute();

  //variables
  vector<sample*> batch;
  
  //setters
  void setToken(string);
  void setFeature(string);
  void setScriptId(string);
};
//constructors
postVal::postVal(string opName, string thing, string device, string url, string token, string feature, string scriptId):operation(opName){
  valid=true;
  this->thing=thing;
  this->device=device;
  this->url=url;
  this->token=token;
  this->feature=feature;
  this->scriptId=scriptId;
  numOfSamples = parseNumOfSamples(opName.substr( opName.find("(")+1, opName.find(")")-(opName.find("(")+1) ));
  batch.reserve(numOfSamples);// allocate in advance what need, because dynamically it is done in power of 2 (2,4,8,16,32,..) and so waste memory

  counter=0; 
  Api=APIRest::getInstance();
}

 postVal::~postVal(){
   for(int i=0;i<batch.size();i++){
    delete batch[i];
  }
   batch.clear();
 }

//setters
 void postVal::setToken(string token){
   this->token=token;
 }
 void postVal::setFeature(string feature){
   this->feature=feature;
 }
 void postVal::setScriptId(string scriptId){
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
        Api->POSTMeasurement(*batch[j], token);// pass a copy 
        delete batch[j];// deallocate it
    }
    batch.clear();//remove all samples
    return new sample("");//return empty sample pointer, for testing purpose
  }
  return NULL;//this means the input is not been POSTed
}

int postVal::parseNumOfSamples( string numString){
  int numberValue=1; 
  
  if(numString!="")// if there is no number we assign 1 because we post one measurement at a time 
  {
	if(!isaNumber(numString))
    {
      valid=false;
      return numberValue;
    }
    numberValue = atoi(numString.c_str());
  }	 
  return numberValue;
}

#endif 
