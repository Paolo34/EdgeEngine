/*
  postVal.cpp - Class for send value of sample to Cloud API.
  Created by Francesco Tornatore, February 24, 2020.
  Released into the public domain.
*/

#include "postVal.h"

#include <stdlib.h>

//constructors
postVal::postVal(string opName, string thing, string device, string url, string token, string feature, string scriptId,int batchMaxSize):operation(opName){
  valid=true;
  this->thing=thing;
  this->device=device;
  this->url=url;
  this->token=token;
  this->feature=feature;
  this->scriptId=scriptId;
  parseNumOfSamples(opName.substr( opName.find("(")+1, opName.find(")")-(opName.find("(")+1) ), batchMaxSize);
  if(valid){
    batch.reserve(numOfSamples);// allocate in advance what need, because dynamically it is done in power of 2 (2,4,8,16,32,..) and so waste memory
    counter=0; 
    Api=APIRest::getInstance();
  }
  
}

 postVal::~postVal(){
   for(int i=0;i<batch.size();i++){
    delete batch[i];
    batch[i]=NULL;
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
        batch[j]=NULL;
    }
    batch.clear();//remove all samples
    
    return new sample("");//return empty sample pointer, only for testing purpose
  }
  return NULL;//this means the input is not been POSTed
}

void postVal::parseNumOfSamples( string numString,int batchMaxSize){
  int numberValue=1; 
  
  if(!numString.empty())// if there is no number we assign 1 because we post one measurement at a time 
  {
    if(!isaNumber(numString))
      {
        valid=false;
        return;
      }
      numberValue = atoi(numString.c_str());
      if(numberValue > batchMaxSize){
        valid=false;
        return;
      }
    }	 
  numOfSamples= numberValue;
}
