//window.h

#ifndef window_h
#define window_h

using std::string;
#include <string>

#include "operation.h"
#include "sample.h"

class window : public operation{
  private:
  //variable
  char function;
  double initial;
  double accumulator;
  double result;
  int windowSize;
  int counter;
  string startDate;

  //methods
  void parseArgument(string);
  double calculate(double);
  public:
  //constructors
  window(string);
  //destructor
  ~window();
  
  //methods
  sample* execute() ;
};
//constructors

window::window(string opName):operation(opName){
  valid=true;
  parseArgument( opName.substr( opName.find("(")+1, opName.find(")")-(opName.find("(")+1)) );
  counter=0;    
  accumulator = initial; //initialize
}
window:: ~window(){
}

//methods
sample* window::execute() {
  if(input!=NULL ){
    accumulator = calculate(input->value);
    if(counter==0)
      startDate=input->startDate;//save the startDate of the first sample
    counter++;
  }
  if(counter >= windowSize){
    result=accumulator;
    //reinitialize 
    accumulator = initial;
    counter=0;
    input->value=result;
    input->startDate=startDate;
    return input;
  }
  delete input;// free memory from this copy of sample because it is useless 
  return NULL;//this should block the execution of the next operation
}

void window::parseArgument(string arguments){
  int pos=0;
  while ( ( pos=arguments.find(" ") ) !=-1){
    arguments.erase(pos);//delete whitespace
  }
  //example "*,2,4"
  int firstIndex = arguments.find(",");
  int endIndex;
  
  //first argument is the operation type
  function=arguments.at(0);
  if(function!='+' && function!='*' && function!='-' && function!='/' && function!='^'){
    valid=false;
    return;
  }
  //second argument is the accumulator
  endIndex = arguments.find(",",firstIndex+1);
  if( !isaNumber(arguments.substr(firstIndex+1,endIndex-(firstIndex+1))) )
  {
    valid=false;
    return;
  }
  initial=atof( arguments.substr(firstIndex+1,endIndex-(firstIndex+1)).c_str() );

  //third argument is the size
  firstIndex = endIndex+1;
  endIndex=arguments.length();
  if(!isaNumber(arguments.substr(firstIndex,endIndex-firstIndex)))
  {
    valid=false;
    return;
  }
  windowSize=atoi( arguments.substr(firstIndex,endIndex-firstIndex).c_str() );
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
