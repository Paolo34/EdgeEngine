//map.h

#ifndef map_h
#define map_h

using std::string;
#include <string>

#include "operation.h"


class mapVal : public operation{
  private:
  //variable
  char function;
  double operand;

  //methods
  void parseArgument(string);
  double calculate(double);
  
  public:
  //constructors
  mapVal(string);
  //destructor
   ~mapVal();
  
  //methods
  sample* execute();
};
//constructors

mapVal::mapVal(string opName):operation(opName){
  valid=true;
  parseArgument( opName.substr( opName.find("(")+1, opName.find(")")-(opName.find("(")+1)) );
}
mapVal:: ~mapVal(){
}

//methods
sample* mapVal::execute() {
  if(input!=NULL ){
    input->value=calculate(input->value);
    return input;
  }
  delete input;
  return NULL;//this should block the execution of the next operation
}

void mapVal::parseArgument(string arguments){
  deleteSpaces(arguments);
  //arguments example : "a/6" so charAt(0) is useless
  //first argument is the operation type
  function=arguments.at(1);
  if(function!='+' && function!='*' && function!='-' && function!='/' && function!='^'){
    valid=false;
    return;
  }
  if(!isaNumber(arguments.substr(2,arguments.length()-2)))
  {
    valid=false;
    return;
  }
  //second argument is the operand
  operand=atof( arguments.substr(2,arguments.length()-2).c_str() );
}

double mapVal::calculate(double input) {
  switch(function){
    case '+':
      return input+operand;
      break;
    case '*':
      return input*operand;
      break;
    case '-':
      return input-operand;
      break;
    case '/':
      return input/operand;
      break;
      case '^':
      return pow(input,operand);
      break;
    default: //this is a free choice
      return input;
      break;
  }
}


#endif 
