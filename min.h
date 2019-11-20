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
  double execute();
};
//constructors
minVal::minVal(String opName):operation(opName){
  minValue=10000;
}
minVal::minVal(String opName, double initValue ):operation(opName){
  minValue=initValue;
}

//methods
double minVal::execute(){
  if(input!=NULL && input<minValue){
    minValue=input;
    return minValue;
  }
  return NULL;//this should block the execution of the next operation
}
#endif 