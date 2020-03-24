/*
  average.h - Class for calculate average value of n samples .
  Created by Francesco Tornatore, February 24, 2020.
  Released into the public domain.
*/
#ifndef average_h
#define average_h

#include <vector>
using std::vector;
#include <string>
using std::string;
#include "operation.h"
#include "sample.h"

class average : public operation{
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
  average(string,int);
  //destructor
  ~average();
  
  //methods
  sample* execute() ;
};

#endif 
