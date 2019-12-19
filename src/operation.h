//operation.h

#ifndef operation_h
#define operation_h

class operation{
  private:
 
  protected:
  //variables
  String opName;
  String code;
  double input;
  
  public:
  //variables
  

  //constructors
  operation(String);
  operation(const operation&);
  
  //setters
  void setInput(double);
  void setCode(String);
  virtual void setToken(String);
  
  //getters
  String getName() const;
  
  //methods
  virtual double* execute();
};

operation::operation(String opName){
  this->opName=opName;
}
//copy constructor
operation::operation(const operation &op){
  this->opName= op.opName;
}

void operation::setInput(double input){
  this->input=input;  
}
String operation::getName() const{
  return opName;
}
void operation::setCode(String code){
  this->code=code;
}

double* operation::execute(){
  Serial.println("base execute");
  return NULL;
  //must be overridden
}
void operation::setToken(String token){
  Serial.println("base execute");
}

#endif 
