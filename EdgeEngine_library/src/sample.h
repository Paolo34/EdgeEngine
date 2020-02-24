/*
  sample.h - Class for model sample.
  Created by Francesco Tornatore, February 24, 2020.
  Released into the public domain.
*/
#ifndef sample_h
#define sample_h
#include <string>
using std::string;
class sample{
  private:
  //variables
  
  //methods
  
  public:
  //variables
  double value;
  string startDate;
  string endDate;
  string url;
  string thing;
  string feature;
  string device;
  string scriptId;
  
  //methods 
  
  
  //constructors
  sample(string);
  sample(const sample&);
  
};

#endif 
