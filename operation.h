//operation.h

#ifndef operation_h
#define operation_h

class operation{
  private:
 
  protected:
  //variables
  double input=NULL;
  String opName;
  String code;
  
  public:
  //variables
  
  //constructors
  operation(String);
  operation(const operation&);
  
  //setters
  void setInput(double);
  void setCode(String);
  
  //getters
  String getName() const;
  
  //methods
  virtual double execute();
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

double operation::execute(){
  Serial.println("base execute");
  return 2;
  //must be overridden
}

#endif 
