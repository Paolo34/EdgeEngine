//sample.h

#ifndef sample_h
#define sample_h

class sample{
  private:
  //variables
  
  double value;
  
  //methods
  
  public:
  //variables
  String feature;
  
  //methods 
  void setValue(double);
  double getValue();
  
  //constructor
  sample(String);
  
};

sample::sample(String feature){
  this->feature=feature;
}

void sample::setValue(double value){
  this->value=value;
}
double sample::getValue(){
  return value;
}


#endif 
