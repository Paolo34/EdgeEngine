//accept.h

#ifndef reception_h
#define reception_h

#include "operation.h"
#define MILLIS_PER_SEC 1000

class reception : public operation{
  private:
  //variables
  int interval;// always in seconds
  double startInstant; // in milliseconds

  //methods
  void initializeCounter();
  
  public:
  //constructors
  reception(String);

  //setters
  void setInterval(int);
  
  //methods
  double* execute();
  int parseIntervalToSec(String);
  
};

//constructors
reception::reception(String opName):operation(opName){
  interval=parseIntervalToSec(opName.substring( opName.indexOf("(")+1, opName.indexOf(")") ));
  initializeCounter();
}


//methods
void reception::initializeCounter(){
  startInstant = millis();
}

double* reception::execute(){
  
  if( (double)( millis()-startInstant )/MILLIS_PER_SEC < interval){ //if not elapsed enough time 
    
    return NULL;//this should block the execution of the next operation
  }
  initializeCounter(); // reinitialize counter
  return new double(input);
}

int reception::parseIntervalToSec( String numString){
  int numberValue=0;
  char unit;
  if (numString.charAt(numString.length()-1)>'9'){//if the last char is not a number we try to interpret it as a measure unit
    numberValue = ( numString.substring(0,numString.length()-1) ).toInt();
    unit=numString.charAt(numString.length()-1);
    
    switch( unit ){
    case 's'://do nothing is already in Second
      break;
    case 'm':// minutes
      //add here milliseconds?????????????? if add millisecond interval has to become a double
      numberValue*=60;
      break;
    case 'h':// hours
      numberValue*=60*60;
      break;
    case 'd':// days
      numberValue*=60*60*24;
      break;
    default:// if no measure unit is indicate, interpreted as Seconds
      break;
    }
  }
  else{
    numberValue = ( numString.substring(0,numString.length()) ).toInt();
  }
  return numberValue;
}
#endif 
