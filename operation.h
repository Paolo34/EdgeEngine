//operation.h

#ifndef operation_h
#define operation_h

class operation{
  private:
  //variables
  String argument;
  
  public:
  //variables
  String opName;
  
  //constructors
  operation(String);
  operation(const operation&);

  //setters
  void setArgument(String);
  //methods
  virtual void parseArgument();//virtual method to be implementedin each derivated class
  
};

operation::operation(String opName){
  this->opName=opName;
}
//copy constructor
operation::operation(const operation &op){
  this->opName= op.opName;
}
void operation::setArgument(String argument){
  this->argument=argument;
  parseArgument();
}



#endif 
