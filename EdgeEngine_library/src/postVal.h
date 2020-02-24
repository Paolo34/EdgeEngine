/*
  postVal.h - Class for send value of sample to Cloud API.
  Created by Francesco Tornatore, February 24, 2020.
  Released into the public domain.
*/
#ifndef postVal_h
#define postVal_h
#include <string>
using std::string;
#include <vector>
using std::vector;
#include "operation.h"
#include "APIRest.h"

class postVal : public operation{
  private:
  //variables
  string token;
  string thing;
  string device;
  string feature;
  string scriptId;
  string url;
  APIRest* Api;
  int numOfSamples;
  int counter;

  //methods
  void parseNumOfSamples(string,int);

  public:
  //constructors
  postVal(string,string,string,string,string,string,string,int);
  //destructor
   ~postVal();
  
  //methods
  sample* execute();

  //variables
  vector<sample*> batch;
  
  //setters
  void setToken(string);
  void setFeature(string);
  void setScriptId(string);
};

#endif 
