//operation.h

#ifndef operation_h
#define operation_h

using std::string;
#include <string>

#include "sample.h"

class operation{
  private:
 
  protected:
  //variables
  string opName;
  string code;
  sample* input=NULL;
  //methods
  boolean isaNumber(string);
  void deleteSpaces(string&);
  
  public:
  //variables
  boolean valid;

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

operation::operation(string opName){
  this->opName=opName;
  valid=false;
}
//copy constructor
operation::operation(const operation &op){
  this->opName= op.opName;
  valid=false;
}
operation::~operation(){
}

void operation::setInput(sample* input){
  this->input=input;
}
string operation::getName() const{
  return opName;
}
void operation::setCode(string code){
  this->code=code;
}

sample* operation::execute(){
  Serial.println("base execution");
  return NULL;
  //must be overridden
}
void operation::setToken(string token){
  Serial.println("base setToken execution");
}

boolean operation::isaNumber(string numberStr){
  if( numberStr.empty()){
    return false;
  }
  for (int i = 0; i < numberStr.length(); i++){
    char c=numberStr.at(i);
    if((c>'9' || c<'0') && c!='.')// only numbers and point are valid
    {
      return false;
    }
  }
  
  return true;
}

void operation::deleteSpaces(string& str){
  int pos=0;
  while ( ( pos=str.find(" ") ) !=-1){
    str.erase(pos,1);//delete space
  }
}
#endif 
