/*
  maxVal.h - Class for filter values of samples less that the maximum.
  Created by Francesco Tornatore, February 24, 2020.
  Released into the public domain.
*/
#ifndef maxVal_h
#define maxVal_h
#include <string>
using std::string;
#include "operation.h"

class maxVal : public operation{
  private:
  double maxValue;
  
  public:
  //constructors
  maxVal(string);
  maxVal(string,double);
  //destructor
  ~maxVal();
  //methods
  sample* execute() ;
  
  //getters
  double getMax();
};


#endif 
