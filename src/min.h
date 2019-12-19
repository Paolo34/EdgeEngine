//min.h

#ifndef minVal_h
#define minVal_h
#include "operation.h"

class minVal : public operation{
  private:
  double minValue;
  
  public:
  //constructors
  minVal(String);
  minVal(String,double);
  
  //methods
  double* execute();
  
  //getters
  double getMin();
};
//constructors
minVal::minVal(String opName):operation(opName){
  minValue=9999999999;
}
minVal::minVal(String opName, double initValue ):operation(opName){
  minValue=initValue;
}

//methods
double* minVal::execute(){
  if(&input!=NULL && input<minValue){
    minValue=input;
    return new double(minValue);
  }
  return NULL;//this should block the execution of the next operation
}

double minVal::getMin(){
	return minValue;
}
#endif 
