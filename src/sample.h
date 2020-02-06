//sample.h

#ifndef sample_h
#define sample_h

class sample{
  private:
  //variables
  
  //methods
  
  public:
  //variables
  double value;
  String startDate;
  String endDate;
  String url;
  String thing;
  String feature;
  String device;
  String scriptId;
  
  //methods 
  
  
  //constructors
  sample(String);
  sample(const sample&);
  
};

sample::sample(String feature){
  this->feature=feature;
}
//copy constructor
sample::sample(const sample &sam){
  this->value=sam.value;
  this->startDate=sam.startDate;
  this->endDate=sam.endDate;
  this->url=sam.url;
  this->thing=sam.thing;
  this->feature=sam.feature;
  this->device=sam.device;
  this->scriptId=sam.scriptId;
}



#endif 
