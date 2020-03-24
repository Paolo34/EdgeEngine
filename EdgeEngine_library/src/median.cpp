/*
  median.cpp - Class for calculate median value of n samples .
  Created by Francesco Tornatore, February 24, 2020.
  Released into the public domain.
*/

#include "median.h"

#include <stdlib.h>
#include <algorithm>

//constructors

median::median(string opName,int maxNumOfSamples):operation(opName){
  valid=true;
  parseNumOfSamples( opName.substr( opName.find("(")+1, opName.find(")")-(opName.find("(")+1)),maxNumOfSamples );
  if(valid){
    orderedValues.resize(numOfSamples);
    samples.reserve(numOfSamples);// allocate in advance what needed, because dynamically it is done in power of 2 (2,4,8,16,32,..) and so waste memory
    counter=0;   
  }
   
}
median:: ~median(){
  for(int i=0;i<samples.size();i++){
    delete samples[i];
    samples[i]=NULL;
  }
  samples.clear();
  orderedValues.clear();
}

//methods
sample* median::execute() {

  if(input!=NULL ){
    samples.push_back(input);
  }
    
  if(input!=NULL && counter < numOfSamples){ // untill we have not enough samples 
    counter++;
  }
  if(input!=NULL && counter >= numOfSamples){ // when the samples are enough (at regime)
    sample* output=new sample(*input);// we cannot corrupt the input because it is stored in the median samples
    output->value = calculate(samples); //beacuse we want a sample (with all its info) with the script resulting value
    output->startDate=samples.front()->startDate; //take startDate from the first sample of the median
    delete samples[0]; // free memory from this copy of sample because it is useless now
    samples[0]=NULL;
    samples.erase( samples.begin() );//remove first sample from the vector

    return output;//output will be deallocated by next operations
  }
  return NULL; // this should block the execution of the next operation
}

void median::parseNumOfSamples(string numString,int maxNumOfSamples){
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

float median::calculate(vector<sample*> samples) {
    
    //ORDINARE I DATI
    for (int i = 0; i < numOfSamples; i++)
    {
       orderedValues[i]=samples[i]->value;
    }
    std::sort(orderedValues.begin(),orderedValues.end());

    if(numOfSamples%2==1){//if numOfSamples is odd 
      return orderedValues[(numOfSamples)/2];
    }
    else//if numOfSamples is even 
    {
      return ( orderedValues[numOfSamples/2-1] + orderedValues[numOfSamples/2] )/2;
    }

}
