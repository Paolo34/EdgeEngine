/*
  stdDeviation.cpp - Class for calculate standard deviation of value of n samples .
  Created by Francesco Tornatore, February 24, 2020.
  Released into the public domain.
*/

#include "stdDeviation.h"

#include <stdlib.h>
#include <math.h>

//constructors

stdDeviation::stdDeviation(string opName,int maxNumOfSamples):operation(opName){
  valid=true;
  parseNumOfSamples( opName.substr( opName.find("(")+1, opName.find(")")-(opName.find("(")+1)),maxNumOfSamples );
  if(valid){
    samples.reserve(numOfSamples);// allocate in advance what needed, because dynamically it is done in power of 2 (2,4,8,16,32,..) and so waste memory
    counter=0; 
  }
     
}
stdDeviation:: ~stdDeviation(){
  for(int i=0;i<samples.size();i++){
    delete samples[i];
    samples[i]=NULL;
  }
   samples.clear();
}

//methods
sample* stdDeviation::execute() {

  if(input!=NULL ){
    samples.push_back(input);
  }
    
  if(input!=NULL && counter < numOfSamples){ // untill we have not enough samples 
    counter++;
  }
  if(input!=NULL && counter >= numOfSamples){ // when the samples are enough (at regime)
    sample* output=new sample(*input);// we cannot corrupt the input because it is stored in the stdDeviation samples
    output->value = calculate(samples); //beacuse we want a sample (with all its info) with the script resulting value
    output->startDate=samples.front()->startDate; //take startDate from the first sample of the stdDeviation
    delete samples[0]; // free memory from this copy of sample because it is useless now
    samples[0]=NULL;
    samples.erase( samples.begin() );//remove first sample from the vector

    return output;//output will be deallocated by next operations
  }
  return NULL; // this should block the execution of the next operation
}

void stdDeviation::parseNumOfSamples(string numString,int maxNumOfSamples){
  int numberValue=0; 
  if(numString.empty()){// if it is empty is a invalid operation
    valid=false;
    return;
  }
  
	if(isaNumber(numString)){// if there is only digits is a valid operation
    numberValue = atoi(numString.c_str());      
  }
  else{
    valid=false;
    return;
  }
  if(numberValue>maxNumOfSamples){
    valid=false;
    return;
  }
  numOfSamples= numberValue;
}

float stdDeviation::calculate(vector<sample*> samples) {
    float avg=0;
    float res=0;
    for(int i=0;i<samples.size();i++){
    avg+=samples[i]->value;
    }
    avg/=numOfSamples;

    for(int i=0;i<samples.size();i++){
        res+= pow(samples[i]->value - avg, 2);
    }
    res/=numOfSamples;
    return sqrt(res);
}

