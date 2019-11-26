//script.h

#ifndef script_h
#define script_h

using std::vector;
#include <time.h> 

#include "operation.h"
#include "accept.h"
#include "max.h"
#include "min.h"
#include "window.h"
#include "slidingWindow.h"
#include "map.h"
#include "send.h"
#include "sample.h"



class script{
  private:
  //variables
  String token;
  String thing;
  String device;
  String featuresAllowed;// features allowed for this device
  String url;
  
  
  //methods
  void parseScript(String);
  int parseIntervalToSec(String);
  operation* createOperation(String);
  boolean isAllowed(String, String);
  
  public:
  //variables
  boolean valid;
  String feature; //feature of this script
  vector<operation*> operations;
  String scriptStr;
  String scriptId;
  String interval;
  double* nextInput;
  //constructor
  script(String,String,String,String,String,String,String);
  
  //methods
  void execute(double); 
  
  //setters
  void setToken(String);
};

script::script( String scriptId,String scriptStr, String thing, String device, String url, String token, String features){
  this->scriptId=scriptId;
  this->scriptStr=scriptStr; //save the string
  this->thing=thing;
  this->device=device;
  this->url=url;
  this->token=token;
  this->featuresAllowed=features;
  valid=false;
  
  parseScript(scriptStr);
  if(!valid){
    Serial.println("The script was not created: "+scriptId);
  //IF SOMETHING GOES WRONG AND THE SCRIPT IS NOT CREATED GIVE SOME ERROR
  }
  else{
    Serial.println("New script: "+scriptId);
  }
}
void script::setToken(String token){
   this->token=token;
   operations[operations.size()-1]->setToken(token);//the last one is the send operation which needs the token
 }
void script::parseScript(String scriptString){
  int startIndex=0;
  int endIndex=1;
  int counter=0;
   
  scriptString.replace(" ","");//delete whitespace
  
  endIndex = scriptString.indexOf("(",startIndex); 
  feature =  scriptString.substring(startIndex,endIndex);//the first is the feature

  if(!isAllowed(feature, featuresAllowed)){
    Serial.println(feature+" is not allowed!");
    return;
  }
    
  //HERE CHECK IF THIS FEATURE IS SUPPORTED ELSE RETURN
  startIndex = endIndex+1;
  
  endIndex = scriptString.indexOf(")",startIndex); //the second is the time interval
  interval = scriptString.substring(startIndex,endIndex);
  operations.push_back( createOperation("accept("+interval+")") );//the first operation is always "accept" which verify the time elapsed
  counter++;
  startIndex = endIndex+2;//+2 because there is also the point in the syntax
  
  while( endIndex!=-1 ){
  
    endIndex = scriptString.indexOf(".",startIndex+1); // start the search from the next charater

    operations.push_back( createOperation(scriptString.substring(startIndex,endIndex)) ); //Add element at the end
    
    if(!operations[counter]){ // if something is wrong in the script
      operations.clear();// Removes all elements from the vector (which are destroyed), leaving the container with a size of 0.
      return;//end here the parsing
    }
    counter++;
      
    startIndex = endIndex+1;
  }
  valid=true;
  //created=true;
}

boolean script::isAllowed(String feature,String featuresAllowed){

  return featuresAllowed.indexOf(feature)!=-1? true:false;
  
}

//These are the allowed opeartions; we have to add manually every operation we want to implement
operation* script::createOperation(String op){
  int endIndex=op.indexOf("(");
  String opName=op.substring(0,endIndex);
  
  if(opName.compareTo("accept")==0){
    return new reception(op);
  }
  else if(opName.compareTo("min")==0){
    return new minVal(op);
    
  }else if(opName.compareTo("max")==0){
    return new maxVal(op);
    
  }else if(opName.compareTo("send")==0){
    return new postVal(op,thing, device, url, token, feature, scriptId);    
  }
  else if(opName.compareTo("window")==0){
    return new window(op);    
  }
  else if(opName.compareTo("slidingWindow")==0){
    return new slidingWindow(op);    
  }
  else if(opName.compareTo("map")==0){
    return new mapVal(op);    
  }
  else{
    Serial.println("wrong operation: "+op);
    return NULL;// if is not among the allowed operations
  }
}

void script::execute(double value){
  
  nextInput=new double(value);
  
  for(int i=0;i<operations.size();i++){
    operations[i]->setInput(*nextInput);
    nextInput = operations[i]->execute();
    if(nextInput==NULL)
      return;// if an operation return NULL stop executing the script
  }
}

#endif 
