/*
  mapVal.h - Class for mapping value of samples with +,-,*,/,^.
  Created by Francesco Tornatore, February 24, 2020.
  Released into the public domain.
*/
#ifndef map_h
#define map_h
#include <string>
using std::string;
#include "operation.h"


class mapVal : public operation{
  private:
  //variable
  char function;
  double operand;

  //methods
  void parseArgument(string);
  double calculate(double);
  
  public:
  //constructors
  mapVal(string);
  //destructor
   ~mapVal();
  
  //methods
  sample* execute();
};


#endif 
