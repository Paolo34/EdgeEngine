//min.h

#ifndef minVal_h
#define minVal_h

using std::string;

#include "operation.h"

class minVal : public operation{
  private:
  double minValue;
  
  public:
  //constructors
  minVal(string);
  minVal(string,double);
  //destructor
  ~minVal();
  //methods
  sample* execute();
  
  //getters
  double getMin();
};
//constructors
minVal::minVal(string opName):operation(opName){
  valid=true;
  if(opName!="min()")
    valid=false;
  minValue=9999999999;
}
minVal::minVal(string opName, double initValue ):operation(opName){
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
  return NULL;//this should block the execution of the next operation
}

double minVal::getMin(){
	return minValue;
}
#endif 
