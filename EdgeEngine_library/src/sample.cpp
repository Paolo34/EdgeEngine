/*
  sample.cpp - Class for model sample.
  Created by Francesco Tornatore, February 24, 2020.
  Released into the public domain.
*/

#include "sample.h"




sample::sample(string feature){
  this->feature=feature;
}
//copy constructor
sample::sample(const sample &sam){
  this->value=sam.value;
  this->startDate=sam.startDate;
  this->endDate=sam.endDate;
  this->url=sam.url;
  this->thing=sam.thing;
  this->feature=sam.feature;
  this->device=sam.device;
  this->scriptId=sam.scriptId;
}
