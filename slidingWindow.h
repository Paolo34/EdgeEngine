//slidingWindow.h

#ifndef slidingWindow_h
#define slidingWindow_h
#include "operation.h"

class slidingWindow : public operation{
  private:
  //variable
  char function;
  double initial;
  double accumulator;
  int windowSize;
  int counter;

  //methods
  void parseArgument(String);
  double calculate(double);
  double calculateInverse(double);
  
  public:
  //constructors
  slidingWindow(String);
  
  //methods
  double execute() ;
};
//constructors

slidingWindow::slidingWindow(String opName):operation(opName){
  parseArgument( opName.substring( opName.indexOf("(")+1, opName.indexOf(")")) );
  counter=0;    
  accumulator = initial; //initialize
}

//methods
double slidingWindow::execute() {
  if(input!=NULL && counter < windowSize){ // untill we have not enough values
    accumulator = calculate(input);
    counter++;
  }
  if(input!=NULL && counter >= windowSize){ // when the value are enough (at regime)
    accumulator = calculate(input);//add last value to the window
    accumulator = calculateInverse(input);//remove last value from the window
    
    return accumulator;
  }
  return NULL;//this should block the execution of the next operation
}

void slidingWindow::parseArgument(String arguments){
  arguments.replace(" ","");//delete whitespace
  int firstIndex = arguments.indexOf(",");
  int endIndex;
  
  //first argument is the operation type
  function=arguments.charAt(0);
  
  //second argument is the accumulator
  endIndex = arguments.indexOf(",",firstIndex+1);
  initial=arguments.substring(firstIndex,endIndex).toDouble();

  //third argument is the size
  firstIndex = endIndex+1;
  endIndex=arguments.length();
  windowSize=arguments.substring(firstIndex,endIndex).toInt();
}
double slidingWindow::calculate(double input) {
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
double slidingWindow::calculateInverse(double input) {
  switch(function){
    case '+':
      return accumulator-input;
      break;
    case '*':
      return accumulator/input;
      break;
    case '-':
      return accumulator+input;
      break;
    case '/':
      return accumulator*input;
      break;
    default: //this is a free choice
      return accumulator-input;
      break;
  }
}



#endif 
