//feature.h

#ifndef feature_h
#define feature_h

class feature{
  private:
  //methods
  void parseFeature(String);
  
  public:
  //variables
  String nameStr;
  
  //constructor
  feature(String);
  
};

feature::feature(String featureStr){
  parseFeature(featureStr);
}

void feature::parseFeature(String nameStr){
  this->nameStr = nameStr;
}


#endif 
