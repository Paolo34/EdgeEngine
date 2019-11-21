//sample.h

#ifndef sample_h
#define sample_h

class sample{
  private:
  //variables
  String feature;
  double value;
  
  //methods
  
  public:
  
  
  //methods 
  void setValue(double);
  
  //constructor
  sample(String);
  
};

sample::sample(String feature){
  this->feature=feature;
}

void sample::setValue(double value){
  this->value=value;
}


#endif 
