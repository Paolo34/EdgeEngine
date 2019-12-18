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
  vector<double> values;
  int windowSize;
  int counter;
  int i;

  //methods
  void parseArgument(String);
  double calculate(vector<double>);
  
  public:
  //constructors
  slidingWindow(String);
  
  //methods
  double* execute() ;
};
//constructors

slidingWindow::slidingWindow(String opName):operation(opName){
  parseArgument( opName.substring( opName.indexOf("(")+1, opName.indexOf(")")) );
  counter=0;    
  accumulator = initial; //initialize
}

//methods
double* slidingWindow::execute() {
  if(&input!=NULL ){
    values.push_back(input);
  }
    
  if(&input!=NULL && counter < windowSize){ // untill we have not enough values 
    counter++;
  }
  if(&input!=NULL && counter >= windowSize){ // when the value are enough (at regime)
    accumulator = calculate(values);//add last value to the window
    values.erase( values.begin() );//delete first value from the queue
    
    return new double(accumulator);
  }
  return NULL; // this should block the execution of the next operation
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
double slidingWindow::calculate(vector<double> values) {
  accumulator=initial;
  switch(function){
    case '+':
      for(i=0;i<values.size();i++){
        accumulator+=values[i];
      }
      break;
    case '*':
      for(i=0;i<values.size();i++){
        accumulator*=values[i];
      }
      break;
    case '-':
      for(i=0;i<values.size();i++){
        accumulator-=values[i];
      }
      break;
    case '/':
      for(i=0;i<values.size();i++){
        accumulator/=values[i];
      }
      break;
    default: //this is a free choice
      for(i=0;i<values.size();i++){
        accumulator+=values[i];
      }
      break;
  }
  return accumulator;
}




#endif 
