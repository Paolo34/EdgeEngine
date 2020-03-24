/*
  minVal.cpp - Class for filter values of samples less that the minimum.
  Created by Francesco Tornatore, February 24, 2020.
  Released into the public domain.
*/
#include "minVal.h"


//constructors
minVal::minVal(string opName):operation(opName){
  valid=true;
  if(opName!="min()")
    valid=false;
  minValue=9999999999;
}
minVal::minVal(string opName, float initValue ):operation(opName){
  valid=true;
  if(opName!="min()")
    valid=false;
  minValue=initValue;
}
minVal:: ~minVal(){
}

//methods
sample* minVal::execute(){
  if(input!=NULL && input->value < minValue){
    minValue=input->value;
    return input;
  }
  delete input;// free memory from this copy of sample because it is useless 
  input=NULL;
  return NULL;//this should block the execution of the next operation
}

float minVal::getMin(){
	return minValue;
}