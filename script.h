//script.h

#ifndef script_h
#define script_h

using std::vector;
#include <time.h> 

#include "feature.h"
#include "operation.h"
#include "accept.h"
#include "max.h"
#include "min.h"
#include "window.h"
#include "slidingWindow.h"
#include "map.h"
#include "send.h"



class script{
  private:
  //variables
  //vector<operation> allowedOperations; 
  String token;
  String thing;
  String device;
  String feat;// feature is also a class
  String url;
  
  //methods
  void parseScript(String);
  int parseIntervalToSec(String);
  operation* createOperation(String);
  
  public:
  //variables
 // boolean created=false;
  boolean valid;
  feature* myFeature;
  vector<operation*> operations;
  String scriptStr;
  String scriptId;
  String interval;
  
  //constructor
  script(String,String,String,String,String,String,String);
  
  //methods
  //boolean isCreated();
  void execute(); 
  
  //setters
  void setToken(String);
};

script::script( String scriptId,String scriptStr, String thing, String device, String url, String token, String feat){
  this->scriptId=scriptId;
  this->scriptStr=scriptStr; //save the string
  this->thing=thing;
  this->device=device;
  this->url=url;
  this->token=token;
  this->feat=feat;
  valid=false;
  /* initialize random seed: */
  srand (time(NULL));
  
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
  myFeature = new feature( scriptString.substring(startIndex,endIndex) );//the first is the feature
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

//These are the allowed opeartions; we have to add manually every operation we want to implement
operation* script::createOperation(String op){
  int endIndex=op.indexOf("(");
  String opName=op.substring(0,endIndex);
  
  if(opName.compareTo("accept")==0){
    return new isAccepted(op);
    //opPtr->setCode(op);
  }
  else if(opName.compareTo("min")==0){
    return new minVal(op);
    
  }else if(opName.compareTo("max")==0){
    return new maxVal(op);
    
  }else if(opName.compareTo("send")==0){
    return new postVal(op,thing, device, url, token, feat, scriptId);    
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
/*
operation script::createOperation(String op){
  int endIndex=op.indexOf("(");
  
  for (int i=0; i<sizeof(allowedOperations); i++){
    if( op.substring(0,endIndex).compareTo(allowedOperations[i].getName()) == 0 ){//if have the same name are the same operation
      operation newOp = allowedOperations[i]; //copy constructor
      newOp.setCode(op);
      return newOp;
    }
  }
  Serial.println("wrong operation inside the script");
  return operation("none");// if is not among the allowed operations
}
boolean script::isCreated(){
  return created;
}
*/

void script::execute(){
  
  //double nextInput=rand() % 30 + 1;// to be substituted with sensor measurement
  double nextInput=10;
  
  for(int i=0;i<operations.size();i++){
    operations[i]->setInput(nextInput);
    nextInput = operations[i]->execute();

    if(nextInput==NULL)
      return;// if an operation return NULL stop executing the script
  }
}

#endif 
