//max.h

#ifndef maxVal_h
#define maxVal_h
#include "operation.h"

class maxVal : public operation{
  private:
  double maxValue;
  
  public:
  //constructors
  maxVal(String);
  maxVal(String,double);
  //destructor
  ~maxVal();
  //methods
  sample* execute() ;
  
  //getters
  double getMax();
};
//constructors

maxVal::maxVal(String opName):operation(opName){
  valid=true;
  if(opName!="max()")
    valid=false;
  maxValue=-9999999999;
}
maxVal::maxVal( String opName,double initValue):operation(opName){
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
  return NULL;//this should block the execution of the next operation
}

double maxVal::getMax(){
	return maxValue;
}

#endif 
