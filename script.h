//script.h

#ifndef script_h
#define script_h

#include "operation.h"
#include "feature.h"

class script{
  private:
  //variables
  operation* allowedOperations;
  boolean created=false;
  
  //methods
  void parseScript(String);
  int parseIntervalToSec(String);
  operation createOperation(String);
  
  public:
  //variables
  feature myFeature;
  operation* operations;
  String scriptStr;
  int interval;
  
  //constructor
  script(String, operation[]);
  
  
  //methods
  boolean isCreated();
  
};

script::script( String scriptStr, operation allowedOperations[] ){
  this->scriptStr=scriptStr; //save the string
  this->allowedOperations=allowedOperations;
  parseScript(scriptStr);
}

void script::parseScript(String scriptString){
  int startIndex=1;
  int endIndex=1;
  int counter=0;
   
  scriptString.replace(" ","");//delete whitespace
  
  endIndex = scriptString.indexOf("(",startIndex); 
  myFeature = feature( scriptString.substring(startIndex,endIndex) );//the first is the feature
  //HERE CHECK IF THIS FEATURE IS SUPPORTED ELSE RETURN
  startIndex = endIndex+1;
  
  endIndex = scriptString.indexOf(")",startIndex); //the second is the time interval
  interval = parseIntervalToSec( scriptString.substring(startIndex,endIndex) );
  //HERE CHECK IF THE INTREVALS CORRECT ELSE RETURN
  startIndex = endIndex+2;//+2 because there is also the point in the syntax
   
  while( startIndex < scriptString.length()-1 ){
  
    endIndex = scriptString.indexOf(".",startIndex+1); // start the search from the next charater
    
    operations[counter] = createOperation( scriptString.substring(startIndex,endIndex) );
    if(operations[counter].opName=="none"){
      return;//if the operation has wrong name
    }
      counter++;
      
    startIndex = endIndex+1;
  }
  created=true;
}

int script::parseIntervalToSec( String numString){
  int numberValue=0;
  char unit;
  numberValue = ( numString.substring(0,numString.length()-1) ).toInt();
  unit=numString.charAt(numString.length()-1);
  
  switch( unit ){
    case 's'://do nothing is already in Second
      break;
    case 'm':// minutes
      //add here milliseconds??????????????
      numberValue*=60;
      break;
    case 'h':// hours
      numberValue*=60*60;
      break;
    case 'd':// days
      numberValue*=60*60*24;
      break;
    default:// if no measure unit is indicate, interpreted as Seconds
      break;
  }
  return numberValue;
}

operation script::createOperation(String op){
  int endIndex=op.indexOf("(");
  
  for (int i=0; i<sizeof(allowedOperations); i++){
    if( (op.substring(0,endIndex)).compareTo(allowedOperations[i].opName) == 0 ){//if are the same
      operation newOp = allowedOperations[i];
      newOp.setArgument( op.substring(endIndex+1,op.length()-1) );// avoid ( and )
      return newOp;
    }
  }
  Serial.println("wrong operation inside the script");
  return operation("none");// if is not among the allowed operations
}

boolean script::isCreated(){
  return created;
}

#endif 
