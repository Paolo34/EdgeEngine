//accept.h

#ifndef isAccepted_h
#define isAccepted_h

#include "operation.h"
#define MILLIS_PER_SEC 1000

class isAccepted : public operation{
  private:
  //variables
  int interval;// always in seconds
  double startInstant; // in milliseconds

  //methods
  void initializeCounter();
  
  public:
  //constructors
  isAccepted(String);

  //setters
  void setInterval(int);
  
  //methods
  double execute();
  int parseIntervalToSec(String);
  
};

//constructors
isAccepted::isAccepted(String opName):operation(opName){
  interval=parseIntervalToSec(opName.substring( opName.indexOf("(")+1, opName.indexOf(")") ));
  initializeCounter();
}


//methods
void isAccepted::initializeCounter(){
  startInstant = millis();
}

double isAccepted::execute(){
  
  if( (double)( millis()-startInstant )/MILLIS_PER_SEC < interval){ //if not elapsed enough time 
    //Serial.println("time elapsed: "+(int)( millis()-startInstant )/MILLIS_PER_SEC );
    return NULL;//this should block the execution of the next operation
  }
  //Serial.println("time elapsed: "+(int)( millis()-startInstant )/MILLIS_PER_SEC );
  //Serial.println("accepted value: "+(int)input);
  initializeCounter(); // reinitialize counter
  return input;
}

int isAccepted::parseIntervalToSec( String numString){
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
