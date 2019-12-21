//map.h

#ifndef map_h
#define map_h
#include "operation.h"

class mapVal : public operation{
  private:
  //variable
  char function;
  double operand;
  int i;
  char c;

  //methods
  void parseArgument(String);
  double calculate(double);
  
  public:
  //constructors
  mapVal(String);
  
  //methods
  double* execute();
};
//constructors

mapVal::mapVal(String opName):operation(opName){
  parseArgument( opName.substring( opName.indexOf("(")+1, opName.indexOf(")")) );
}

//methods
double* mapVal::execute() {
  if(&input!=NULL ){
    return new double(calculate(input));
  }
 
  return NULL;//this should block the execution of the next operation
}

void mapVal::parseArgument(String arguments){
  arguments.replace(" ","");//delete whitespace
  //arguments example : "a/6" so charAt(0) is useless
  //first argument is the operation type
  function=arguments.charAt(1);
  if(function!='+' && function!='*' && function!='-' && function!='/' && function!='^'){
    valid=false;
    return;
  }
  if(!isaNumber(arguments.substring(2,arguments.length())))
  {
    valid=false;
    return;
  }
  //second argument is the operand
  operand=arguments.substring(2,arguments.length()).toDouble();
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