//slidingWindow.h

#ifndef slidingWindow_h
#define slidingWindow_h
#include "operation.h"
#include "sample.h"

class slidingWindow : public operation{
  private:
  //variable
  char function;
  double initial;
  double accumulator;
  vector<sample*> samples;
  int windowSize;
  int counter;

  //methods
  void parseArgument(String);
  double calculate(vector<sample*>);
  
  public:
  //constructors
  slidingWindow(String);
  //destructor
  ~slidingWindow();
  
  //methods
  sample* execute() ;
};
//constructors

slidingWindow::slidingWindow(String opName):operation(opName){
  valid=true;
  parseArgument( opName.substring( opName.indexOf("(")+1, opName.indexOf(")")) );
  counter=0;    
  accumulator = initial; //initialize
}
slidingWindow:: ~slidingWindow(){
  for(int i=0;i<samples.size();i++){
    delete samples[i];
  }
  // for ( sample* sam : samples ){
  //    delete sam;//call destructor for each sample
  //  }
   samples.clear();
}

//methods
sample* slidingWindow::execute() {

  if(input!=NULL ){
    samples.push_back(input);
  }
    
  if(input!=NULL && counter < windowSize){ // untill we have not enough samples 
    counter++;
  }
  if(input!=NULL && counter >= windowSize){ // when the samples are enough (at regime)
    accumulator = calculate(samples);
    sample* output=new sample(*input);// we cannot corrupt the input because it is stored in the slidingWindow samples
    output->value=accumulator; //beacuse we want a sample (with all its info) with the script resulting value
    output->startDate=samples.front()->startDate; //take startDate from the first sample of the slidingWindow
    delete samples[0]; // free memory from this copy of sample because it is useless now
    samples.erase( samples.begin() );//remove first sample from the vector

    return output;
  }
  return NULL; // this should block the execution of the next operation
}

void slidingWindow::parseArgument(String arguments){
  arguments.replace(" ","");//delete whitespace
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
  if(!isaNumber(arguments.substring(firstIndex+1,endIndex)))
  {
    valid=false;
    return;
  }
  windowSize=arguments.substring(firstIndex,endIndex).toInt();
}

double slidingWindow::calculate(vector<sample*> samples) {
  accumulator=initial;
  int i;
  switch(function){
    case '+':
      for(i=0;i<samples.size();i++){
        accumulator+=samples[i]->value;
      }
      break;
    case '*':
      for(i=0;i<samples.size();i++){
        accumulator*=samples[i]->value;
      }
      break;
    case '-':
      for(i=0;i<samples.size();i++){
        accumulator-=samples[i]->value;
      }
      break;
    case '/':
      for(i=0;i<samples.size();i++){
        accumulator/=samples[i]->value;
      }
      break;
    default: //this is a free choice
      for(i=0;i<samples.size();i++){
        accumulator+=samples[i]->value;
      }
      break;
  }
  return accumulator;
}




#endif 
