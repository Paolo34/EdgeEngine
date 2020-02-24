/*
  script.h - Class for model scripts which will be executed by Edge Engine.
  Created by Francesco Tornatore, February 24, 2020.
  Released into the public domain.
*/
#ifndef script_h
#define script_h
#include <string>
using std::string;
#include <vector>
using std::vector;
#include "operation.h"


class script{
  private:
  //variables
  string token;
  string thing;
  string device;
  string featuresAllowed;// features allowed for this device
  string url;
  int sendBufferSize, scriptStatementMaxSize, statementBufferSize;
  
  //methods
  void parseScript(string);
  operation* createOperation(string);
  bool isAllowed(string, string);
  void deleteSpaces(string&);
  
  public:
  //variables
  bool valid;
  string feature; //feature of this script
  vector<operation*> operations;
  string scriptStr;
  string scriptId;
  sample* nextInput=NULL;
  //constructor
  script(string,string,string,string,string,string,string,int,int,int);
  //destructor
  ~script();
  
  //methods
  bool execute(sample*); 
  
  //setters
  void setToken(string);
};

#endif 
