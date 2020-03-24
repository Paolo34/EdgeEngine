/*
  maxVal.h - Class for filter values of samples less that the maximum.
  Created by Francesco Tornatore, February 24, 2020.
  Released into the public domain.
*/

#include "maxVal.h"


//constructors

maxVal::maxVal(string opName):operation(opName){
  valid=true;
  if(opName!="max()")
    valid=false;
  maxValue=-9999999999;
}
maxVal::maxVal( string opName,float initValue):operation(opName){
  valid=true;
  if(opName!="max()")
    valid=false;
  maxValue=initValue;
}
maxVal:: ~maxVal(){
}

//methods
sample* maxVal::execute() {
  if(input!=NULL && input->value > maxValue){
    maxValue=input->value;
    return input;
  }
  delete input; // free memory from this copy of sample because it is useless 
  input=NULL;
  return NULL;//this should block the execution of the next operation
}

float maxVal::getMax(){
	return maxValue;
}