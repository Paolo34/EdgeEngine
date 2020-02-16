//median.h

#ifndef median_h
#define median_h
using std::string;
using std::vector;
#include <algorithm>
#include "operation.h"
#include "sample.h"

class median : public operation{
  private:
  //variable
  vector<sample*> samples;
  vector<double> orderedValues;
  int numOfSamples;
  int counter;

  //methods
  int parseNumOfSamples(string);
  double calculate(vector<sample*>);
  
  public:
  //constructors
  median(string);
  //destructor
  ~median();
  
  //methods
  sample* execute() ;
};
//constructors

median::median(string opName):operation(opName){
  valid=true;
  Serial.println( opName.substr( opName.find("(")+1, opName.find(")")-(opName.find("(")+1)).c_str());
  parseNumOfSamples( opName.substr( opName.find("(")+1, opName.find(")")-(opName.find("(")+1)) );
  orderedValues.reserve(numOfSamples);
  samples.reserve(numOfSamples);// allocate in advance what needed, because dynamically it is done in power of 2 (2,4,8,16,32,..) and so waste memory
  counter=0;    
}
median:: ~median(){
  for(int i=0;i<samples.size();i++){
    delete samples[i];
  }
  samples.clear();
  orderedValues.clear();
}

//methods
sample* median::execute() {

  if(input!=NULL ){
    samples.push_back(input);
  }
    
  if(input!=NULL && counter < numOfSamples){ // untill we have not enough samples 
    counter++;
  }
  if(input!=NULL && counter >= numOfSamples){ // when the samples are enough (at regime)
    sample* output=new sample(*input);// we cannot corrupt the input because it is stored in the median samples
    output->value = calculate(samples); //beacuse we want a sample (with all its info) with the script resulting value
    output->startDate=samples.front()->startDate; //take startDate from the first sample of the median
    delete samples[0]; // free memory from this copy of sample because it is useless now
    samples.erase( samples.begin() );//remove first sample from the vector

    return output;//output will be deallocated by next operations
  }
  return NULL; // this should block the execution of the next operation
}

int median::parseNumOfSamples(string numString){
  int numberValue=0; 
  
  if(numString!="") {// if there is an argument is a valid operation
	if(isaNumber(numString)){// if there is only digits is a valid operation
        numberValue = atoi(numString.c_str());      
    }
    else{
        valid=false;
    }
  }	
  else{// if there is no argument is an invalid operation
    valid=false;
  } 
  return numberValue;
}

double median::calculate(vector<sample*> samples) {
    //ORDINARE I DATI
    for (int i = 0; i < numOfSamples; i++)
    {
       orderedValues.at(i)=samples[i]->value;
    }
    std::sort(orderedValues.begin(),orderedValues.end());

    if(numOfSamples%2==1)//if numOfSamples is odd 
        return orderedValues[(numOfSamples)/2];
    else//if numOfSamples is even 
    {
        return ( orderedValues[numOfSamples/2-1] + orderedValues[numOfSamples/2] )/2;
    }

}




#endif 
