//operation.h

#ifndef operation_h
#define operation_h
#include "sample.h"

class operation{
  private:
 
  protected:
  //variables
  String opName;
  String code;
  sample* input=NULL;
  //methods
  boolean isaNumber(String);
  
  public:
  //variables
  boolean valid;

  //constructors
  operation(String);
  operation(const operation&);
  //destructor
  virtual ~operation();
  
  //setters
  void setInput(sample*);
  void setCode(String);
  virtual void setToken(String);
  
  //getters
  String getName() const;
  
  //methods
  virtual sample* execute();
};

operation::operation(String opName){
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
String operation::getName() const{
  return opName;
}
void operation::setCode(String code){
  this->code=code;
}

sample* operation::execute(){
  Serial.println("base execute");
  return NULL;
  //must be overridden
}
void operation::setToken(String token){
  Serial.println("base execute");
}

boolean operation::isaNumber(String numberStr){
  for (int i = 0; i < numberStr.length(); i++)
  {
    char c=numberStr.charAt(i);
    if(c>'9' || c<'0')
    {
      return false;
    }
  }
  return true;
}
#endif 
