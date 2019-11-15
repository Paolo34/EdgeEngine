//min.h

#ifndef minVal_h
#define minVal_h
#include "operation.h"

class minVal : public operation{
  private:
  double minValue;
  
  public:
  //constructors
  minVal();
  minVal(double);
  
  //methods
  double execute(double value);
};
//constructors
minVal::minVal(String opName):operation(opName){
  minValue=10000;
}
minVal::minVal(double initValue, String opName):operation(opName){
  minValue=initValue;
}

//methods
double minVal::execute(double value){
  if(value<minValue){
    minValue=value;
    return minValue;
  }
  return NULL;//this should block the execution of the next operation
}

#endif 
