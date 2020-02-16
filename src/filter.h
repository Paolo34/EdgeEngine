//filter.h

#ifndef filter_h
#define filter_h

using std::string;
#include <string>

#include "operation.h"

class filter : public operation{
  private:
  //variable
  string function;
  double operand;

  //methods
  void parseArgument(string);
  
  public:
  //constructors
  filter(string);
  //destructor
   ~filter();
  
  //methods
  sample* execute();
};
//constructors

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
    if(function=="<" && input->value<operand)
        return input;
    else if(function==">" && input->value>operand)
        return input;
    else if(function=="<=" && input->value<=operand)
        return input;
    else if(function==">=" && input->value>=operand)
        return input;
    else if(function=="==" && input->value==operand)
        return input;
  }
  delete input;
  return NULL;//this should block the execution of the next operation
}

void filter::parseArgument(string arguments){
  deleteSpaces(arguments);
  //arguments example : "a<6" or "a<=6" so charAt(0) is useless
  //first argument is the operation type
  
  char func=arguments.at(1);
  if( func!='<' && func!='>' && func!='=' ){
    valid=false;
    return;
  }
  function=func;
  func=arguments.at(2);
  if(func=='='){//if it is a 2-chars function
    function.push_back(func);
    if(!isaNumber(arguments.substr(3,arguments.length()-3)))
    {
      valid=false;
      return;
    }
    //second argument is the operand
    operand=atof( arguments.substr(3,arguments.length()-3).c_str() ); 
  }
  else{//if it is a 1-char function
    if(!isaNumber(arguments.substr(2,arguments.length()-2)))
    {
      valid=false;
      return;
    }
    //second argument is the operand
    operand=atof( arguments.substr(2,arguments.length()-2).c_str() ); 
  }
}


#endif 
