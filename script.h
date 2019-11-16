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
#include "send.h"


class script{
  private:
  //variables
  //vector<operation> allowedOperations; 
  boolean created=false;
  String token;
  String thing;
  String device;
  String feat;// feature is also a class
  String scriptId;
  String url;
  
  //methods
  void parseScript(String);
  int parseIntervalToSec(String);
  operation createOperation(String);
  
  public:
  //variables
  feature* myFeature;
  vector<operation> operations;
  String scriptStr;
  String interval;
  
  //constructor
  script(String,String,String,String,String,String,String);
  
  //methods
  boolean isCreated();
  void execute();
  
};

script::script( String scriptStr, String thing, String device, String url, String token, String feat, String scriptId){
  this->scriptStr=scriptStr; //save the string
  this->thing=thing;
  this->device=device;
  this->url=url;
  this->token=token;
  this->feat=feat;
  this->scriptId=scriptId;
  
  /* initialize random seed: */
  srand (time(NULL));
  
  parseScript(scriptStr);
  if(!created){
    Serial.println("The script was not created!");
  //IF SOMETHING GOES WRONG AND THE SCRIPT IS NOT CREATED GIVE SOME ERROR
  }
  else{
    Serial.println("The script was created succesfully!");
  }
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
    if(operations[counter].getName()=="none"){//if something is wrong in the script
      operations.clear();// Removes all elements from the vector (which are destroyed), leaving the container with a size of 0.
      return;//end here the parsing
    }
    counter++;
      
    startIndex = endIndex+1;
  }

  created=true;
}

//this way we have to add manually every operation we want to implement
operation script::createOperation(String op){
  int endIndex=op.indexOf("(");
  String opName=op.substring(0,endIndex);

  if(opName.compareTo("accept")==0){
    isAccepted acc =isAccepted(opName);
    acc.setCode(op);
    operation* opPtr=&acc;
    return *opPtr;    
    
  }else if(opName.compareTo("min")==0){
    minVal minV =minVal(opName);
    operation* opPtr=&minV;
    return *opPtr; 
    
  }else if(opName.compareTo("max")==0){
    maxVal maxV =maxVal(opName);
    operation* opPtr=&maxV;
    return *opPtr;
    
  }else if(opName.compareTo("send")==0){
    postVal postV =postVal(opName,thing, device, url, token, feat, scriptId);
    operation* opPtr=&postV;
    return *opPtr;
    
  }else{
    Serial.println("wrong operation: "+op);
    return operation("none");// if is not among the allowed operations
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
}*/

boolean script::isCreated(){
  return created;
}

void script::execute(){
  double nextInput=rand() % 30 + 1;
  
  for(int i=0;i<operations.size();i++){
    operations[i].setInput(nextInput);
    nextInput = operations[i].execute();
    if(nextInput==NULL)
      return;// if an operation return NULL stop executing the script
  }
}

#endif 
