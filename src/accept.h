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
  int parseIntervalToSec(String);
  
  public:
  //constructors
  reception(String);

  //setters
  void setInterval(int);
  int getInterval();
  
  //methods
  double* execute();
  
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
  char lastChar = numString.charAt(numString.length()-1);
  
  if(numString!=""){ // if there is no time interval we assign 0 because there is not deltaTime between two measurements
    if (lastChar>'9' || lastChar<'0'){ // if the last char is not a number we try to interpret it as a measure unit
      numberValue = ( numString.substring(0,numString.length()-1) ).toInt(); // if conversion fails a 0 is returned
      
      switch( lastChar ){
      case 's'://do nothing is already in Second
        break;
      case 'm':// minutes
        numberValue*=60;
        break;
      case 'h':// hours
        numberValue*=60*60;
        break;
      case 'd':// days
        numberValue*=60*60*24;
        break;
      default:// if no measure unit or char non contemplated, interpreted as Seconds
        break;
      }
    }
    else{
      numberValue = ( numString.substring(0,numString.length()) ).toInt();
    }
  }
  
  return numberValue;
}
int reception::getInterval(){
	return interval;
}
#endif 
