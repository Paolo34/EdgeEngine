/*
  operation.cpp - Base Class for each operation performed on samples.
  Created by Francesco Tornatore, February 24, 2020.
  Released into the public domain.
*/

#include "operation.h"


operation::operation(string opName){
  this->opName=opName;
  valid=false;
}
//copy constructor
operation::operation(const operation &op){
  this->opName= op.opName;
  valid=false;
}
operation::~operation(){
}

void operation::setInput(sample* input){
  this->input=input;
}
string operation::getName() const{
  return opName;
}
void operation::setCode(string code){
  this->code=code;
}

sample* operation::execute(){
  //Serial.println("base execution");
  return NULL;
  //must be overridden
}
void operation::setToken(string token){
  //Serial.println("base setToken execution");
}

bool operation::isaNumber(string numberStr){
  if( numberStr.empty()){
    return false;
  }
  for (int i = 0; i < numberStr.length(); i++){
    char c=numberStr.at(i);
    if((c>'9' || c<'0') && c!='.')// only numbers and point are valid
    {
      return false;
    }
  }
  
  return true;
}

void operation::deleteSpaces(string& str){
  int pos=0;
  while ( ( pos=str.find(" ") ) !=-1){
    str.erase(pos,1);//delete space
  }
}