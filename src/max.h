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
  
  //methods
  double* execute() ;
  
  //getters
  double getMax();
};
//constructors

maxVal::maxVal(String opName):operation(opName){
  if(opName!="max()")
    valid=false;
  maxValue=-9999999999;
}
maxVal::maxVal( String opName,double initValue):operation(opName){
  if(opName!="max()")
    valid=false;
  maxValue=initValue;
}

//methods
double* maxVal::execute() {
  if(&input!=NULL && input>maxValue){
    maxValue=input;
    return new double(maxValue);
  }
  return NULL;//this should block the execution of the next operation
}

double maxVal::getMax(){
	return maxValue;
}

#endif 