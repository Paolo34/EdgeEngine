/*
  stdDeviation.h - Class for calculate standard deviation of value of n samples .
  Created by Francesco Tornatore, February 24, 2020.
  Released into the public domain.
*/
#ifndef stdDeviation_h
#define stdDeviation_h
#include <string>
using std::string;
#include <vector>
using std::vector;

#include "operation.h"
#include "sample.h"

class stdDeviation : public operation{
  private:
  //variable
  vector<sample*> samples;
  int numOfSamples;
  int counter;

  //methods
  void parseNumOfSamples(string,int);
  float calculate(vector<sample*>);
  
  public:
  //constructors
  stdDeviation(string,int);
  //destructor
  ~stdDeviation();
  
  //methods
  sample* execute() ;
};



#endif 

