/*
  script.cpp - Class for model scripts which will be executed by Edge Engine.
  Created by Francesco Tornatore, February 24, 2020.
  Released into the public domain.
*/
#include "script.h"

#include <time.h> 

#include "reception.h"
#include "maxVal.h"
#include "minVal.h"
#include "window.h"
#include "slidingWindow.h"
#include "mapVal.h"
#include "postVal.h"
#include "filter.h"
#include "median.h"
#include "average.h"
#include "stdDeviation.h"
#include "sample.h"


script::script( string scriptId,string scriptStr, string thing, string device, string url, string token, string features, int sendBufferSize,int scriptStatementMaxSize,int statementBufferSize){
  this->scriptId=scriptId;
  this->scriptStr=scriptStr; //save the string
  this->thing=thing;
  this->device=device;
  this->url=url;
  this->token=token;
  this->featuresAllowed=features;
  this->sendBufferSize=sendBufferSize;
  this->scriptStatementMaxSize=scriptStatementMaxSize;
  this->statementBufferSize=statementBufferSize;
  valid=false;
  
  parseScript(scriptStr);
  if(!valid){
    Serial.print("The script was not created: ");
    Serial.println(scriptId.c_str());
  //IF SOMETHING GOES WRONG AND THE SCRIPT IS NOT CREATED GIVE SOME ERROR
  }
  else{
    Serial.print("New script: ");
    Serial.println(scriptId.c_str());
  }
}

script::~script(){
  for(int i=0;i<operations.size();i++){
    delete operations[i];
  }
  operations.clear();

  if (nextInput)//if nextInput is pointing to something 
  {
    delete nextInput;
    nextInput=NULL;
  }
}

void script::setToken(string token){
   this->token=token;
   operations[operations.size()-1]->setToken(token);//the last one is the send operation which needs the token
 }
void script::parseScript(string scriptString){
  int startIndex=0;
  int endIndex=1;
  int counter=0;
  deleteSpaces(scriptString);
  endIndex = scriptString.find("(",startIndex); 
  feature =  scriptString.substr(startIndex,endIndex-startIndex);//the first is the feature
  //Check if this feature is supported else return
  if(!isAllowed(feature, featuresAllowed)){
    Serial.print(feature.c_str());
    Serial.println(" is not allowed!");

    return;
  }
  
  startIndex = endIndex+1;
  
  endIndex = scriptString.find(")",startIndex); //the second is the time interval
  string interval = scriptString.substr(startIndex,endIndex-startIndex);
  operations.push_back( createOperation("accept("+interval+")") );//the first operation is always "accept" which verify the time elapsed
  if(!operations[counter]->valid){
    return; //end here the parsing
  }
	  
  counter++;
  startIndex = endIndex+2;//+2 because there is also the point in the syntax
  
  while( endIndex!=-1 ){
    
    endIndex = scriptString.find(".",startIndex+1); // start the search from the next charater
    operations.push_back( createOperation(scriptString.substr(startIndex,endIndex-startIndex)) ); //Add element at the end
    
    if(!operations[counter]->valid || counter>scriptStatementMaxSize){ // if something is wrong in the script or there are too many operations
      return;//end here the parsing
    }
    counter++;
      
    startIndex = endIndex+1;
  }
  valid=true;
}

bool script::isAllowed(string feature,string featuresAllowed){
  int startIndex=featuresAllowed.find(feature);
  if(startIndex!=-1){
	  int endIndex=featuresAllowed.find("\"",startIndex);
    // if( featuresAllowed.substr(startIndex, (endIndex!=-1? endIndex-startIndex : featuresAllowed.length()-startIndex) )== feature )

	  if( featuresAllowed.substr(startIndex, endIndex-startIndex )== feature )
		  return true;
  }
  return false;
}

//These are the allowed opeartions; we have to add manually every operation we want to implement
operation* script::createOperation(string op){
  int endIndex=op.find("(");
  string opName=op.substr(0,endIndex);
  
  if(opName=="accept"){
    return new reception(op);
  }
  else if(opName=="min"){
    return new minVal(op);
    
  }else if(opName=="max"){
    return new maxVal(op);
    
  }else if(opName=="send"){
    return new postVal(op,thing, device, url, token, feature, scriptId,sendBufferSize);    
  }
  else if(opName=="window"){
    return new window(op,statementBufferSize);    
  }
  else if(opName=="slidingWindow"){
    return new slidingWindow(op,statementBufferSize);    
  }
  else if(opName=="map"){
    return new mapVal(op);    
  }
  else if(opName=="filter"){
    return new filter(op);    
  }
  else if(opName=="average"){
    return new average(op,statementBufferSize);    
  }
  else if(opName=="median"){
    return new median(op,statementBufferSize);    
  }
  else if(opName=="stdDeviation"){
    return new stdDeviation(op,statementBufferSize);    
  }
  else{
    Serial.print("wrong operation: ");
    Serial.println(op.c_str());
    return new operation(op);// if is not among the allowed operations
  }
}

bool script::execute(sample* value){
  
  nextInput = new sample(*value);// pass a copy of the sample beacuse the script may modify it

  for(int i=0; i<operations.size(); i++){
    operations[i]->setInput(nextInput);
    nextInput = operations[i]->execute();
    if(nextInput==NULL)
      return false;// if an operation return NULL stop executing the script
    
  }
  delete nextInput;
  nextInput=NULL;
  return true;
}

void script::deleteSpaces(string& str){
  int pos=0;
  while ( ( pos=str.find(" ") ) !=-1){
    str.erase(pos,1);//delete whitespace
  }
}
