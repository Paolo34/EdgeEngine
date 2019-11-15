//max.h

#ifndef maxVal_h
#define maxVal_h
#include "operation.h"

class maxVal : public operation{
  private:
  double maxValue;
  
  public:
  //constructors
  maxVal();
  maxVal(double);
  
  //methods
  double execute(double value);
};
//constructors
maxVal::maxVal(String opName):operation(opName){
  maxValue=-10000;
}
maxVal::maxVal(double initValue, String opName):operation(opName){
  maxValue=initValue;
}

//methods
double maxVal::execute(double value){
  if(value>maxValue){
    maxValue=value;
    return maxValue;
  }
  return NULL;//this should block the execution of the next operation
}

#endif 
