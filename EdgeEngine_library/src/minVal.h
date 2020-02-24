/*
  minVal.h - Class for filter values of samples less that the minimum.
  Created by Francesco Tornatore, February 24, 2020.
  Released into the public domain.
*/
#ifndef minVal_h
#define minVal_h
#include <string>
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

#endif 
