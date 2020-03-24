/*
  mapVal.cpp - Class for mapping value of samples with +,-,*,/,^.
  Created by Francesco Tornatore, February 24, 2020.
  Released into the public domain.
*/
#include "mapVal.h"


#include <stdlib.h>
#include <math.h>

//constructors

mapVal::mapVal(string opName):operation(opName){
  valid=true;
  parseArgument( opName.substr( opName.find("(")+1, opName.find(")")-(opName.find("(")+1)) );
}
mapVal:: ~mapVal(){
}

//methods
sample* mapVal::execute() {
  if(input!=NULL ){
    input->value=calculate(input->value);
    return input;
  }
  delete input;
  input=NULL;
  return NULL;//this should block the execution of the next operation
}

void mapVal::parseArgument(string arguments){
  if(arguments.empty()){// if it is empty is a invalid operation
    valid=false;
    return;
  }
  deleteSpaces(arguments);
  //arguments example : "a/6" so at(0) is useless
  //first argument is the operation type
  function=arguments.at(1);
  if(function!='+' && function!='*' && function!='-' && function!='/' && function!='^'){
    valid=false;
    return;
  }
  if(!isaNumber(arguments.substr(2,arguments.length()-2)))
  {
    valid=false;
    return;
  }
  //second argument is the operand
  operand=atof( arguments.substr(2,arguments.length()-2).c_str() );
}

float mapVal::calculate(float input) {
  switch(function){
    case '+':
      return input+operand;
      break;
    case '*':
      return input*operand;
      break;
    case '-':
      return input-operand;
      break;
    case '/':
      return input/operand;
      break;
      case '^':
      return pow(input,operand);
      break;
    default: //this is a free choice
      return input;
      break;
  }
}
