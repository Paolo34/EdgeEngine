//accept.h

#ifndef reception_h
#define reception_h

#include <time.h>
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
  //destructor
   ~reception();

  //setters
  void setInterval(int);
  int getInterval();
  
  //methods
  sample* execute();
  
};

//constructors
reception::reception(String opName):operation(opName){
  valid=true;
  interval=parseIntervalToSec(opName.substring( opName.indexOf("(")+1, opName.indexOf(")") ));
  initializeCounter();
}
 reception:: ~reception(){
 }


//methods
void reception::initializeCounter(){
  startInstant = (double)clock() / CLOCKS_PER_SEC;
}

sample* reception::execute(){
  
  if( ( ((double)clock() / CLOCKS_PER_SEC) -startInstant ) < interval ){ //if not elapsed enough time 
    delete input; // free memory from this copy of sample because it is useless 
    return NULL;//this should block the execution of the next operation
  }
  initializeCounter(); // reinitialize counter
  return input;
}

int reception::parseIntervalToSec( String numString){
  int numberValue=0; 
  
  if(numString!=""){ // if there is no time interval we assign 0 because there is not deltaTime between two measurements
    char lastChar = numString.charAt(numString.length()-1);
    if (lastChar>'9' || lastChar<'0'){ // if the last char is not a number we try to interpret it as a measure unit
      //check the validity of the interval, there must be only numbers 
      if(!isaNumber(numString.substring(0,numString.length()-1)))
      {
        valid=false;
        return numberValue;
      }
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
      default:// if  char non contemplated
        valid=false;
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
