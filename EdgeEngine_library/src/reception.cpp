/*
  reception.cpp - Class for filter samples based on acquistion time.
  Created by Francesco Tornatore, February 24, 2020.
  Released into the public domain.
*/
#include "reception.h"

#include <stdlib.h>
#include <time.h>


//constructors
reception::reception(string opName):operation(opName){
  valid=true;
  interval=parseIntervalToSec(opName.substr( opName.find("(")+1, opName.find(")")-(opName.find("(")+1) ));
  if(valid){
    initializeCounter();
    startInstant-=interval;// the very first value has not to wait the time interval
    //otherwise the first value is acquired after "interval" from the script creation
  }
}
  
reception:: ~reception(){
}

//methods
void reception::initializeCounter(){
  startInstant = (float)clock() / CLOCKS_PER_SEC;
}

sample* reception::execute(){
  
  if( ( ((float)clock() / CLOCKS_PER_SEC) -startInstant ) < interval ){ //if not elapsed enough time 
    delete input; // free memory from this copy of sample because it is useless 
    input=NULL;
    return NULL;//this should block the execution of the next operation
  }
  initializeCounter(); // reinitialize counter
  return input;
}

int reception::parseIntervalToSec( string numString){
  int numberValue=0; 
  
  if(!numString.empty()){ // if there is no time interval we assign 0 because there is not deltaTime between two measurements
    char lastChar = numString.at(numString.length()-1);
    if (lastChar>'9' || lastChar<'0'){ // if the last char is not a number we try to interpret it as a measure unit
      //check the validity of the interval, there must be only numbers 
      if(!isaNumber(numString.substr(0,numString.length()-1)))
      {
        valid=false;
        return numberValue;
      }
      numberValue = atoi( numString.substr(0,numString.length()-1).c_str() ); // if conversion fails a 0 is returned
      
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
      numberValue = atoi( numString.substr(0,numString.length()).c_str() );
    }
  }
  
  return numberValue;
}
int reception::getInterval(){
	return interval;
}