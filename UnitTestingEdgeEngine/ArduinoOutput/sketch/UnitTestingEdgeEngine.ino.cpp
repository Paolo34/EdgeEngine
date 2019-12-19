#include <Arduino.h>
#line 1 "c:\\Users\\chicc\\Desktop\\Università\\5\\TESI MAGISTRALE\\ESP32\\EdgeEngine\\UnitTestingEdgeEngine\\UnitTestingEdgeEngine.ino"
// UNIT TESTING OF EDGE ENGINE

#include <AUnit.h>
using std::vector;
#include "src/edgine.h"
#include "src/sample.h"
#include "src/connection.h"

test(operation){
  operation* op= new operation("operation");
  assertEqual(op->getName(),"operation");
}
test(reception){
  reception* rec= new reception("accept(10)");
  assertNotEqual(rec->getInterval(),0);
  assertEqual(rec->getInterval(),10);
  
  rec= new reception("accept(7m)");
  assertEqual(rec->getInterval(),420);
  
  rec= new reception("accept(1h)");
  assertEqual(rec->getInterval(),3600);
  
  rec= new reception("accept(3d)");
  rec->setInput(3);
  assertEqual(rec->getInterval(),259200);
  assertEqual(rec->execute(),NULL);
  
  rec= new reception("accept(0)");
  rec->setInput(3);
  assertEqual(rec->getInterval(),0);
  assertEqual(*(rec->execute()),(double)3);

  
}
test(mapVal){
  mapVal* mapV = new mapVal("map(a/6)");
}

test(maxVal){
  maxVal* maxV= new maxVal("max()");
  maxV->setInput(-10);
  assertEqual(*(maxV->execute()),(double)-10);
  maxV->setInput(-110);
  assertEqual(maxV->execute(),NULL);
  maxV->setInput(0);
  assertEqual(*(maxV->execute()),(double)0);
  
  maxV =new maxVal("max()",10);
  maxV->setInput(-10);
  assertEqual(*(maxV->execute()),(double)10);
  maxV->setInput(110);
  assertEqual(*(maxV->execute()),(double)110);
  maxV->setInput(0);
  assertEqual(maxV->execute(),NULL);
}
test(minVal){
  minVal* minV = new minVal("min()");
  minV->setInput(8);
  assertEqual(*(minV->execute()),(double)8);
  minV->setInput(-2340);
  assertEqual(*(minV->execute()),(double)-2340);
  minV->setInput(0);
  assertEqual(minV->execute(),NULL);
  
  minV = new minVal("min()",10);
  minV->setInput(20);
  assertEqual(minV->execute(),NULL);
  minV->setInput(110);
  assertEqual(minV->getMin(),(double)10);
  minV->setInput(0);
  assertEqual(minV->getMin(),(double)0);
  assertEqual(*(minV->execute()),(double)0);
}
test(postVal){
  postVal* postV =new postVal("send()","riccardo-office","temperature-sensor-riccardo-office",
                      "http://students.atmosphere.tools","token","temperature","average-temperature");
                      
}

test(edgine){
  edgine* Edge=edgine::getInstance();

  options opts;
  opts.username = "riccardo-office-temperature-sensor-username";
  opts.password =  "riccardo-office-temperature-sensor-password";
  //route
  opts.url = "http://students.atmosphere.tools";
  opts.ver = "v1";
  opts.login = "login";
  opts.devs = "devices";
  opts.scps = "scripts";
  opts.measurements = "measurements";
  opts.dateUrl= "URL OF THE DATE";
  //Edgine identifiers
  opts.thing = "riccardo-office";
  opts.device = "temperature-sensor-riccardo-office";
  opts.id = "temperature-sensor-riccardo-office";

  Edge->init(opts);
}
void setup() {
  delay(1000); // wait for stability on some boards to prevent garbage Serial
  Serial.begin(115200); // ESP8266 default of 74880 not supported on Linux
  while (!Serial); // for the Arduino Leonardo/Micro only
}

void loop() {
  aunit::TestRunner::run();
}

