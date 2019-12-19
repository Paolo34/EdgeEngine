// UNIT TESTING OF EDGE ENGINE

#include <AUnit.h>
using std::vector;
#include "src/edgine.h"
#include "src/sample.h"
#include "src/connection.h"

using namespace aunit;


class Options :public TestOnce{
  protected:
    options opts;
    void setup() override{
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
    }
};

test(operation){
  operation* op= new operation("operation");
  assertEqual(op->getName(),"operation");
  
}
test(reception){
  
  reception* rec= new reception("accept(10)");
  rec->setInput(3436);
  assertNotEqual(rec->getInterval(),0);
  assertEqual(rec->getInterval(),10);
  assertEqual(rec->execute(),NULL);
  
  rec= new reception("accept(7m)");
  rec->setInput(3436);
  assertEqual(rec->getInterval(),420);
  assertEqual(rec->execute(),NULL);
  
  rec= new reception("accept(1h)");
  rec->setInput(36);
  assertEqual(rec->getInterval(),3600);
  assertEqual(rec->execute(),NULL);
  
  rec= new reception("accept(3d)");
  rec->setInput(3);
  assertEqual(rec->getInterval(),259200);
  assertEqual(rec->execute(),NULL);
  
  rec= new reception("accept(0)");
  rec->setInput(3);
  assertEqual(rec->getInterval(),0);
  assertEqual(*(rec->execute()),(double)3);

  //if time has typo time interval is set to 0
  rec= new reception("accept(aaa)");
  rec->setInput(3);
  assertEqual(rec->getInterval(),0);
  assertEqual(*(rec->execute()),(double)3);
  
}

test(mapVal){
  mapVal* mapV = new mapVal("map(a/6)");
  mapV->setInput(6);
  assertEqual(*(mapV->execute()),(double)1);
  mapV->setInput(23);  
  assertEqual(*(mapV->execute()),(double)(23)/(double)(6));

  mapV= new mapVal("map(a+101)");
  mapV->setInput(9);
  assertEqual(*(mapV->execute()),(double)(9+101));
  mapV->setInput(25);  
  assertEqual(*(mapV->execute()),(double)(25+101));

  mapV= new mapVal("map(a*3)");
  mapV->setInput(9);
  assertEqual(*(mapV->execute()),(double)(3*9));
  mapV->setInput(1);  
  assertEqual(*(mapV->execute()),(double)(3*1));

  mapV= new mapVal("map(a-101)");
  mapV->setInput(3);
  assertEqual(*(mapV->execute()),(double)(3-101));
  mapV->setInput(205);  
  assertEqual(*(mapV->execute()),(double)(205-101));

  mapV= new mapVal("map(a^4)");
  mapV->setInput(9);
  assertEqual(*(mapV->execute()),(double)(pow(9,4)));
  mapV->setInput(45);  
  assertEqual(*(mapV->execute()),(double)(pow(45,4)));

  mapV= new mapVal("map(a4)");
  mapV->setInput(1);
  assertEqual(*(mapV->execute()),(double)1);
  mapV->setInput(0);  
  assertEqual(*(mapV->execute()),(double)0);

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
  assertEqual(maxV->execute(),NULL);
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
  assertEqual(minV->execute(),NULL);
  minV->setInput(0);
  assertEqual(*(minV->execute()),(double)0);
}
test(window){
  window* wind = new window("window(+,1,10)");
  for (int i = 1; i < 10; i++)
  {
    wind->setInput(i);
    assertEqual(wind->execute(),NULL);
  }
  wind->setInput(10);
  assertEqual(*(wind->execute()),(double) 56);

  for (int i = 1; i < 10; i++)
  {
    wind->setInput(3);
    assertEqual(wind->execute(),NULL);
  }
  wind->setInput(3);
  assertEqual(*(wind->execute()),(double)31);

  wind= new window("window(*,2,4)");
  for (int i = 1; i < 4; i++)
  {
    wind->setInput(i);
    assertEqual(wind->execute(),NULL);
  }
  wind->setInput(10);
  assertEqual(*(wind->execute()),(double) 2*1*2*3*10);

  wind= new window("window(-,7,5)");
  for (int i = 1; i < 5; i++)
  {
    wind->setInput(i);
    assertEqual(wind->execute(),NULL);
  }
  wind->setInput(5);
  assertEqual(*(wind->execute()),(double)-8);

   wind= new window("window(/,1,5)");
  for (int i = 1; i < 5; i++)
  {
    wind->setInput(i);
    assertEqual(wind->execute(),NULL);
  }
  wind->setInput(1);
  assertEqual(*(wind->execute()),(double)1/1/2/3/4/1);
}

test(slidingWindow){
  slidingWindow* slidWind = new slidingWindow("slidingWindow(+,1,5)");
  for (int i = 1; i < 5; i++)
  {
    slidWind->setInput(i);
    assertEqual(slidWind->execute(),NULL);
  }
  slidWind->setInput(33);
  assertEqual(*(slidWind->execute()),(double)44);
  slidWind->setInput(3);
  assertEqual(*(slidWind->execute()),(double)46);
  slidWind->setInput(0);
  assertEqual(*(slidWind->execute()),(double)44);

  slidWind = new slidingWindow("slidingWindow(-,34,3)");
  for (int i = 1; i < 3; i++)
  {
    slidWind->setInput(i);
    assertEqual(slidWind->execute(),NULL);
  }
  slidWind->setInput(3);
  assertEqual(*(slidWind->execute()),(double)28);
  slidWind->setInput(3);
  assertEqual(*(slidWind->execute()),(double)26);
  slidWind->setInput(0);
  assertEqual(*(slidWind->execute()),(double)28);

  slidWind = new slidingWindow("slidingWindow(*,1,6)");
  for (int i = 1; i < 6; i++)
  {
    slidWind->setInput(i);
    assertEqual(slidWind->execute(),NULL);
  }
  slidWind->setInput(10);
  assertEqual(*(slidWind->execute()),(double)1200);
  slidWind->setInput(3);
  assertEqual(*(slidWind->execute()),(double)3600);
  slidWind->setInput(1);
  assertEqual(*(slidWind->execute()),(double)1800);

  slidWind = new slidingWindow("slidingWindow(/,100,6)");
  for (int i = 1; i < 6; i++)
  {
    slidWind->setInput(i);
    assertEqual(slidWind->execute(),NULL);
  }
  slidWind->setInput(10);
  assertEqual(*(slidWind->execute()),(double)100/1/2/3/4/5/10);
  slidWind->setInput(3);
  assertEqual(*(slidWind->execute()),(double)100/2/3/4/5/10/3);
  slidWind->setInput(1);
  assertEqual(*(slidWind->execute()),(double)100/3/4/5/10/3/1);
}

testF(Options,script){
  script* scrp=new script("average-temperature","temperature(10).slidingWindow(+, 0, 6).map(a/6).send()",
                            opts.thing.c_str(), opts.device.c_str(), "url", "token", "temperature");
  assertTrue(scrp->valid);
  assertEqual(scrp->feature,"temperature");
  assertEqual(scrp->scriptId,"average-temperature");
  assertEqual(scrp->scriptStr,"temperature(10).slidingWindow(+, 0, 6).map(a/6).send()");
  assertEqual(scrp->interval,"10");
  assertEqual(scrp->feature,"temperature");

}

testF(Options,postVal){
  postVal* postV =new postVal("send()", opts.thing.c_str(), opts.device.c_str(), opts.url.c_str(),"token",
                             "temperature", "average-temperature" );
                  
}

testF(Options,edgine){
  edgine* Edge=edgine::getInstance();
  Edge->init(opts);
}


void setup() {
  delay(1000); // wait for stability on some boards to prevent garbage Serial
  Serial.begin(115200); // ESP8266 default of 74880 not supported on Linux
  while (!Serial); // for the Arduino Leonardo/Micro only
}

void loop() {
  TestRunner::run();
}
