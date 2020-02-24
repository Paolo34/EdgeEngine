/*
  window.h - Class for perform a calculus on a window of n samples.
  Created by Francesco Tornatore, February 24, 2020.
  Released into the public domain.
*/
#ifndef window_h
#define window_h

#include <Arduino.h>
#include "operation.h"
#include "sample.h"

class window : public operation{
  private:
  //variable
  char function;
  double initial;
  double accumulator;
  double result;
  int windowSize;
  int counter;
  string startDate;

  //methods
  void parseArgument(string,int);
  double calculate(double);
  public:
  //constructors
  window(string,int);
  //destructor
  ~window();
  
  //methods
  sample* execute() ;
};



#endif 
