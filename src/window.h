//window.h

#ifndef window_h
#define window_h
#include "operation.h"

class window : public operation{
  private:
  //variable
  char function;
  double initial;
  double accumulator;
  double result;
  int windowSize;
  int counter;

  //methods
  void parseArgument(String);
  double calculate(double);
  public:
  //constructors
  window(String);
  
  //methods
  double* execute() ;
};
//constructors

window::window(String opName):operation(opName){
  parseArgument( opName.substring( opName.indexOf("(")+1, opName.indexOf(")")) );
  counter=0;    
  accumulator = initial; //initialize
}

//methods
double* window::execute() {
  if(&input!=NULL ){
    accumulator = calculate(input);
    counter++;
  }
  if(counter >= windowSize){
    result=accumulator;
    //reinitialize 
    accumulator = initial;
    counter=0;
    
    return new double(result);
  }
  return NULL;//this should block the execution of the next operation
}

void window::parseArgument(String arguments){
  arguments.replace(" ","");//delete whitespace
  //example "*,2,4"
  int firstIndex = arguments.indexOf(",");
  int endIndex;
  
  //first argument is the operation type
  function=arguments.charAt(0);
  if(function!='+' && function!='*' && function!='-' && function!='/' && function!='^'){
    valid=false;
    return;
  }
  //second argument is the accumulator
  endIndex = arguments.indexOf(",",firstIndex+1);
  if(!isaNumber(arguments.substring(firstIndex+1,endIndex)))
  {
    valid=false;
    return;
  }
  initial=arguments.substring(firstIndex+1,endIndex).toDouble();

  //third argument is the size
  firstIndex = endIndex+1;
  endIndex=arguments.length();
  if(!isaNumber(arguments.substring(firstIndex,endIndex)))
  {
    valid=false;
    return;
  }
  windowSize=arguments.substring(firstIndex,endIndex).toInt();
}
double window::calculate(double input) {
  switch(function){
    case '+':
      return accumulator+input;
      break;
    case '*':
      return accumulator*input;
      break;
    case '-':
      return accumulator-input;
      break;
    case '/':
      return accumulator/input;
      break;
    default: //this is a free choice
      return accumulator+input;
      break;
  }
}



#endif 