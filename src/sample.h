//sample.h

#ifndef sample_h
#define sample_h

using std::string;

class sample{
  private:
  //variables
  
  //methods
  
  public:
  //variables
  double value;
  string startDate;
  string endDate;
  string url;
  string thing;
  string feature;
  string device;
  string scriptId;
  
  //methods 
  
  
  //constructors
  sample(string);
  sample(const sample&);
  
};

sample::sample(string feature){
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
