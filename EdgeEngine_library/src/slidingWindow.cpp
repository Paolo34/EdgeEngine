/*
  slidingWindow.cpp - Class for perform a calculus on a window of n samples.
  Created by Francesco Tornatore, February 24, 2020.
  Released into the public domain.
*/
#include "slidingWindow.h"

#include <stdlib.h>

//constructors

slidingWindow::slidingWindow(string opName,int maxWindowBuffer):operation(opName){
  valid=true;
  parseArgument( opName.substr( opName.find("(")+1, opName.find(")")-(opName.find("(")+1)),maxWindowBuffer );
  if(valid){
    samples.reserve(windowSize);// allocate in advance what needed, because dynamically it is done in power of 2 (2,4,8,16,32,..) and so waste memory
    counter=0;    
    accumulator = initial; //initialize
  }
  
}
slidingWindow:: ~slidingWindow(){
  for(int i=0;i<samples.size();i++){
    delete samples[i];
    samples[i]=NULL;
  }
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
    samples[0]=NULL;
    samples.erase( samples.begin() );//remove first sample from the vector

    return output;//output will be deallocated by next operations
  }
  return NULL; // this should block the execution of the next operation
}

void slidingWindow::parseArgument(string arguments,int maxWindowBuffer){
  if(arguments.empty()){
    valid=false;
    return;
  }
  
  deleteSpaces(arguments);
  int firstIndex = arguments.find(",");
  int endIndex;
  
  //first argument is the operation type
  function=arguments.at(0);
  if(function!='+' && function!='*' && function!='-' && function!='/'){
    valid=false;
    return;
  }
  //second argument is the accumulator
  endIndex = arguments.find(",",firstIndex+1);
  if(!isaNumber(arguments.substr(firstIndex+1,endIndex-(firstIndex+1))))
  {
    valid=false;
    return;
  }
  initial=atof( arguments.substr(firstIndex+1,endIndex-(firstIndex+1)).c_str() );
  //third argument is the size
  firstIndex = endIndex+1;
  endIndex=arguments.length();
  if( !isaNumber(arguments.substr(firstIndex,endIndex-(firstIndex))) )
  {
    valid=false;
    return;
  }
  windowSize=atoi( arguments.substr(firstIndex,endIndex-firstIndex).c_str() );
  if(windowSize>maxWindowBuffer){
    valid=false;
    return;
  }
  
}

float slidingWindow::calculate(vector<sample*> samples) {
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

