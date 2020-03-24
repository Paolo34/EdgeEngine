/*
  slidingWindow.h - Class for perform a calculus on a window of n samples.
  Created by Francesco Tornatore, February 24, 2020.
  Released into the public domain.
*/
#ifndef slidingWindow_h
#define slidingWindow_h
#include <string>
using std::string;
#include <vector>
using std::vector;
#include "operation.h"
#include "sample.h"

class slidingWindow : public operation{
  private:
  //variable
  char function;
  float initial;
  float accumulator;
  vector<sample*> samples;
  int windowSize;
  int counter;

  //methods
  void parseArgument(string,int);
  float calculate(vector<sample*>);
  
  public:
  //constructors
  slidingWindow(string,int);
  //destructor
  ~slidingWindow();
  
  //methods
  sample* execute() ;
};



#endif 
