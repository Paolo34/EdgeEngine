/*
  filter.cpp - Class for filter sample values within intervals.
  Created by Francesco Tornatore, February 24, 2020.
  Released into the public domain.
*/

#include "filter.h"

#include <stdlib.h>

//constructor
filter::filter(string opName):operation(opName){
  valid=true;
  parseArgument( opName.substr( opName.find("(")+1, opName.find(")")-(opName.find("(")+1)) );
}
filter:: ~filter(){
}

//methods
sample* filter::execute() {
  if(input!=NULL ){
      //ex: filter(a<5)
    if(function=="<" && input->value<operand1)
        return input;
    else if(function==">" && input->value>operand1)
        return input;
    else if(function=="<=" && input->value<=operand1)
        return input;
    else if(function==">=" && input->value>=operand1)
        return input;
    else if(function=="==" && input->value==operand1)
        return input;
    else if(function=="C" && (input->value>=operand1 && input->value<=operand2) )
      return input;
    else if(function=="/C" && (input->value<=operand1 || input->value>=operand2) )
      return input;
  }
  delete input;
  input=NULL;
  return NULL;//this should block the execution of the next operation
}

void filter::parseArgument(string arguments){
  if(arguments.empty()){
    valid=false;
    return;
  }
  deleteSpaces(arguments);
  //arguments example : "a<6" or "a<=6" so charAt(0) is useless
  //first argument is the operation type
  
  char func=arguments.at(1);
  if( func=='<' || func=='>' || func=='=' ){
    function=func;
    if(arguments.size()<3){
      valid=false;
      return;
    }
    func=arguments.at(2);
    if(func=='='){//if it is a 2-chars function
      function.push_back(func);
      if(!isaNumber(arguments.substr(3,arguments.length()-3)))
      {
        valid=false;
        return;
      }
      //second argument is the operand1
      operand1=atof( arguments.substr(3,arguments.length()-3).c_str() ); 
    }
    else{//if it is a 1-char function
      if(!isaNumber(arguments.substr(2,arguments.length()-2)))
      {
        valid=false;
        return;
      }
      //second argument is the operand1
      operand1=atof( arguments.substr(2,arguments.length()-2).c_str() ); 
    }
  }
  else if(func=='C'){//disjoint interval
    int separator;
    function=func;
    if(arguments.size()<3){
      valid=false;
      return;
    }
    if( arguments.at(2)=='[' ){
      if( (separator=arguments.find(',',2))!=-1 && isaNumber(arguments.substr(3,separator-3)) ){
        operand1=atof(arguments.substr(3,separator-3).c_str());
        if( arguments.at(arguments.length()-1)==']' && isaNumber(arguments.substr(separator+1,arguments.length()-1-(separator+1))) ){
          operand2= atof( arguments.substr(separator+1,arguments.length()-1-(separator+1)).c_str() );
          if(operand1<operand2)
            return;
        }
      }
    }
    valid= false;
    return;
  }
  else if (func=='/'){
    int separator;
    function=func;
    if(arguments.size()<4){
      valid=false;
      return;
    }
    func=arguments.at(2);
    if(func=='C'){
      function.push_back(func);
      if( arguments.at(3)=='[' ){
        if( (separator=arguments.find(',',3))!=-1 && isaNumber(arguments.substr(4,separator-4)) ){
          operand1=atof(arguments.substr(4,separator-4).c_str());
          if( arguments.at(arguments.length()-1)==']' && isaNumber(arguments.substr(separator+1,arguments.length()-1-(separator+1))) ){
            operand2= atof( arguments.substr(separator+1,arguments.length()-1-(separator+1)).c_str() );
            if(operand1<operand2)
              return;
          }
        }
      }
    }
    valid= false;
    return;
  }
  else{
    valid=false;
    return;
  }

}
