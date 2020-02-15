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
  double calculate(double);
  
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
    input->value=calculate(input->value);
    return input;
  }
  return NULL;//this should block the execution of the next operation
}

void filter::parseArgument(string arguments){
  size_t pos=0;
  while ( ( pos=arguments.find(" ") ) !=-1){
    arguments.erase(pos);//delete whitespace
  }
  //arguments example : "a<6" or "a<=6" so charAt(0) is useless
  //first argument is the operation type
  
  char func1=arguments.at(1);
  if( func1!='<' && func1!='>' && func1!='=' ){
    valid=false;
    return;
  }
  // function.append()
  // char func2=arguments.charAt(2);
  // if( !isaNumber(func2) ){
  //     if(func2!='='){
          
  //     }
  //     else{
  //       valid=false;
  //       return;
  //     }
  // }
  

  if(!isaNumber(arguments.substr(3,arguments.length()-3)))
  {
    valid=false;
    return;
  }
  //second argument is the operand
  operand=atof( arguments.substr(2,arguments.length()-2).c_str() );
}

double filter::calculate(double input) {
  //ex: a<5
  if(function=="<"){
        return input<operand;
  }
  else if(function=="<"){

  }
  else if(function=="<"){

  }
  else if(function=="<"){

  }
  else if(function=="<"){

  }
  else{ //this is a free choice
    return input;
  }
  
}


#endif 
