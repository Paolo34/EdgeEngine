/*
  filter.h - Class for filter sample values within intervals.
  Created by Francesco Tornatore, February 24, 2020.
  Released into the public domain.
*/

#ifndef filter_h
#define filter_h
#include <string>
using std::string;
#include "operation.h"

class filter : public operation{
  private:
  //variable
  string function;
  float operand1;
  float operand2;
  //methods
  void parseArgument(string);
  
  public:
  //constructors
  filter(string);
  //destructor
   ~filter();
  
  //methods
  sample* execute();
};


#endif 
