/*
  median.h - Class for calculate median value of n samples .
  Created by Francesco Tornatore, February 24, 2020.
  Released into the public domain.
*/
#ifndef median_h
#define median_h
#include <string>
using std::string;
#include <vector>
using std::vector;
#include "operation.h"
#include "sample.h"

class median : public operation{
  private:
  //variable
  vector<sample*> samples;
  vector<float> orderedValues;
  int numOfSamples;
  int counter;

  //methods
  void parseNumOfSamples(string,int);
  float calculate(vector<sample*>);
  
  public:
  //constructors
  median(string,int);
  //destructor
  ~median();
  
  //methods
  sample* execute() ;
};


#endif 
