//accept.h

#ifndef isAccepted_h
#define isAccepted_h
#include "operation.h"

class isAccepted : public operation{
  private:
  
  public:
  //constructors
  isAccepted();
  
  //methods
  double execute();
};
//constructors
isAccepted::isAccepted(String opName):operation(opName){
  
}

//methods
double isAccepted::execute(){
  
  return NULL;//this should block the execution of the next operation
}

#endif 
