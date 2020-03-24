/*
  reception.h - Class for filter samples based on acquistion time.
  Created by Francesco Tornatore, February 24, 2020.
  Released into the public domain.
*/
#ifndef reception_h
#define reception_h
#include <string>
using std::string;
#include "operation.h"
#define MILLIS_PER_SEC 1000

class reception : public operation{
  private:
  //variables
  int interval;// always in seconds
  float startInstant; // in milliseconds
  
  //methods
  void initializeCounter();
  int parseIntervalToSec(string);
  
  public:
  //constructors
  reception(string);
  //destructor
   ~reception();

  //setters
  void setInterval(int);
  int getInterval();
  
  //methods
  sample* execute();
  
};

#endif 
