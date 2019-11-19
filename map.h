//map.h

#ifndef map_h
#define map_h
#include "operation.h"

class mapVal : public operation{
  private:
  //variable
  char function;
  double operand;

  //methods
  void parseArgument(String);
  double calculate(double);
  
  public:
  //constructors
  mapVal(String);
  
  //methods
  double execute() ;
};
//constructors

mapVal::mapVal(String opName):operation(opName){
  parseArgument( opName.substring( opName.indexOf("(")+1, opName.indexOf(")")) );
}

//methods
double mapVal::execute() {
  if(input!=NULL ){
    return calculate(input);
  }
 
  return NULL;//this should block the execution of the next operation
}

void mapVal::parseArgument(String arguments){
  arguments.replace(" ","");//delete whitespace
  
  //first argument is the operation type
  function=arguments.charAt(1);
  
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
