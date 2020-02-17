//average.h

#ifndef average_h
#define average_h
using std::string;
using std::vector;

#include <string>

#include "operation.h"
#include "sample.h"

class average : public operation{
  private:
  //variable
  vector<sample*> samples;
  int numOfSamples;
  int counter;

  //methods
  int parseNumOfSamples(string);
  double calculate(vector<sample*>);
  
  public:
  //constructors
  average(string);
  //destructor
  ~average();
  
  //methods
  sample* execute() ;
};
//constructors

average::average(string opName):operation(opName){
  valid=true;
  Serial.println( opName.substr( opName.find("(")+1, opName.find(")")-(opName.find("(")+1)).c_str());
  parseNumOfSamples( opName.substr( opName.find("(")+1, opName.find(")")-(opName.find("(")+1)) );
  if(valid){
    samples.reserve(numOfSamples);// allocate in advance what needed, because dynamically it is done in power of 2 (2,4,8,16,32,..) and so waste memory
    counter=0;  
  }
}
average:: ~average(){
  for(int i=0;i<samples.size();i++){
    delete samples[i];
  }
   samples.clear();
}

//methods
sample* average::execute() {

  if(input!=NULL ){
    samples.push_back(input);
  }
    
  if(input!=NULL && counter < numOfSamples){ // untill we have not enough samples 
    counter++;
  }
  if(input!=NULL && counter >= numOfSamples){ // when the samples are enough (at regime)
    sample* output=new sample(*input);// we cannot corrupt the input because it is stored in the average samples
    output->value = calculate(samples); //beacuse we want a sample (with all its info) with the script resulting value
    output->startDate=samples.front()->startDate; //take startDate from the first sample of the average
    delete samples[0]; // free memory from this copy of sample because it is useless now
    samples.erase( samples.begin() );//remove first sample from the vector

    return output;//output will be deallocated by next operations
  }
  return NULL; // this should block the execution of the next operation
}

int average::parseNumOfSamples(string numString){
  int numberValue=0; 
  if(numString.empty()){// if it is empty is a invalid operation
    valid=false;
    return numberValue;
  }
  
	if(isaNumber(numString)){// if there is only digits is a valid operation
        numberValue = atoi(numString.c_str());      
    }
    else{
        valid=false;
    } 
  return numberValue;
}

double average::calculate(vector<sample*> samples) {
  double sum=0;
  for(int i=0;i<samples.size();i++){
    sum+=samples[i]->value;
  }
  return sum/numOfSamples;
}




#endif 
