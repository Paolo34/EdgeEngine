/*
  operation.h - Base Class for each operation performed on samples.
  Created by Francesco Tornatore, February 24, 2020.
  Released into the public domain.
*/
#ifndef operation_h
#define operation_h
#include <string>
using std::string;
#include "sample.h"

class operation{
  private:
 
  protected:
  //variables
  string opName;
  string code;
  sample* input=NULL;
  //methods
  bool isaNumber(string);
  void deleteSpaces(string&);
  
  public:
  //variables
  bool valid;

  //constructors
  operation(string);
  operation(const operation&);
  //destructor
  virtual ~operation();
  
  //setters
  void setInput(sample*);
  void setCode(string);
  virtual void setToken(string);
  
  //getters
  string getName() const;
  
  //methods
  virtual sample* execute();
};

#endif 
