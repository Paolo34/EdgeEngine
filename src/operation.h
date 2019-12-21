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
  int i;
  char c;
  //methods
  boolean isaNumber(String);
  
  public:
  //variables
  boolean valid;

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
  valid=true;
}
//copy constructor
operation::operation(const operation &op){
  this->opName= op.opName;
  valid=true;
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

boolean operation::isaNumber(String numberStr){
  for (i = 0; i < numberStr.length(); i++)
  {
    c=numberStr.charAt(i);
    if(c>'9' || c<'0')
    {
      return false;
    }
  }
  return true;
}
#endif 