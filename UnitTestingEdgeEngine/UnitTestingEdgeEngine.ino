// UNIT TESTING OF EDGE ENGINE

#include <AUnit.h>
using std::vector;
#include "src/edgine.h"
#include "src/sample.h"
#include "src/connection.h"

using namespace aunit;

class Settings :public TestOnce{
  protected:
    options opts;
    APIRest* Api;
    connection* conn;
    String ssidWIFI;
    String passWIFI;
    String token;

    void setup() override{
      opts.username = "username";
      opts.password =  "password";
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

      ssidWIFI="ssid";
      passWIFI="pass";
      token="JWT token";

      Api=APIRest::getInstance();
      Api->TESTING=true;
          
      conn=connection::getInstance();
      conn->TESTING=true;
    }
};

test(operation){
  operation* op= new operation("operation");
  assertEqual(op->getName(),"operation");
  assertTrue(op->valid);
  
}
test(reception){
  
  reception* rec= new reception("accept(10)");
  assertTrue(rec->valid);
  assertEqual(rec->getName(),"accept(10)");
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

  //if time has typo 
  rec= new reception("accept(aaa)");
  assertFalse(rec->valid);
  rec= new reception("accept(10aa)");
  assertFalse(rec->valid);
}

test(mapVal){
  mapVal* mapV = new mapVal("map(a/6)");
  assertTrue(mapV->valid);
  assertEqual(mapV->getName(),"map(a/6)");  
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
  assertFalse(mapV->valid);
  mapV= new mapVal("map(a?4)");
  assertFalse(mapV->valid);
  mapV= new mapVal("map(a/22a)");
  assertFalse(mapV->valid);
  mapV= new mapVal("map(a/a22)");
  assertFalse(mapV->valid);
}

test(maxVal){
  maxVal* maxV= new maxVal("max()");
  assertTrue(maxV->valid);
  assertEqual(maxV->getName(),"max()");  
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
  maxV =new maxVal("max(a)");
  assertFalse(maxV->valid);
}
test(minVal){
  minVal* minV = new minVal("min()");
  assertTrue(minV->valid);
  assertEqual(minV->getName(),"min()");  
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
  minV =new minVal("min(19)");
  assertFalse(minV->valid);
}

test(window){
  window* wind = new window("window(+,1,10)");
  assertTrue(wind->valid);
  assertEqual(wind->getName(),"window(+,1,10)");  
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

  wind = new window("window(?,1,10)");
  assertFalse(wind->valid);
  wind = new window("window(*,a,10)");
  assertFalse(wind->valid);
  wind = new window("window(+,1,1e0)");
  assertFalse(wind->valid);
  wind = new window("window(*,110)");
  assertFalse(wind->valid);
}

test(slidingWindow){
  slidingWindow* slidWind = new slidingWindow("slidingWindow(+,1,5)");
  assertTrue(slidWind->valid);
  assertEqual(slidWind->getName(),"slidingWindow(+,1,5)");  
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

  slidWind = new slidingWindow("slidingWindow(?,1,10)");
  assertFalse(slidWind->valid);
  slidWind = new slidingWindow("slidingWindow(*,a,10)");
  assertFalse(slidWind->valid);
  slidWind = new slidingWindow("slidingWindow(+,1,1e0)");
  assertFalse(slidWind->valid);
  slidWind = new slidingWindow("slidingWindow(*,*,110)");
  assertFalse(slidWind->valid);
  slidWind = new slidingWindow("slidingWindow()");
  assertFalse(slidWind->valid);
}

testF(Settings,script){
  script scrp= script("average-temperature","temperature(10).slidingWindow(+, 0, 6).map(a/6).send()",
                            opts.thing.c_str(), opts.device.c_str(), "url", token.c_str(), "temperature, humidity");
  assertTrue(scrp.valid);
  assertEqual(scrp.feature,"temperature");
  assertEqual(scrp.scriptId,"average-temperature");
  assertEqual(scrp.scriptStr,"temperature(10).slidingWindow(+, 0, 6).map(a/6).send()");
  assertEqual(scrp.interval,"10");
  assertEqual(scrp.feature,"temperature");

  vector<operation*> ops=scrp.operations;
  int size=ops.size();
  assertEqual(size, 4); 
  assertEqual(ops[0]->getName(),"accept(10)");
  assertEqual(ops[1]->getName(),"slidingWindow(+,0,6)");//spaces are deleted by the parser
  assertEqual(ops[2]->getName(),"map(a/6)");
  assertEqual(ops[3]->getName(),"send()");

  scrp= script("average-temperature","temperature().slidingWindow(+, 0, 6).map(a/6).send()",
                            opts.thing.c_str(), opts.device.c_str(), "url", token.c_str(), "temperature, humidity");
  assertTrue(scrp.valid);
  assertEqual(scrp.feature,"temperature");
  assertEqual(scrp.scriptId,"average-temperature");
  assertEqual(scrp.scriptStr,"temperature().slidingWindow(+, 0, 6).map(a/6).send()");
  assertEqual(scrp.interval,"");
  assertEqual(scrp.feature,"temperature");
  assertFalse(scrp.execute(1));
  assertFalse(scrp.execute(2));
  assertFalse(scrp.execute(3));
  assertFalse(scrp.execute(4));
  assertFalse(scrp.execute(5));
  assertTrue(scrp.execute(6));// only after 6 input the value is sent to the api
  assertTrue(scrp.execute(7));

  scrp= script("average-temperature","temperature().window(+, 0, 4).map(a/4).send()",
                            opts.thing.c_str(), opts.device.c_str(), "url", token.c_str(), "temperature, humidity");
  assertTrue(scrp.valid);
  assertEqual(scrp.feature,"temperature");
  assertEqual(scrp.scriptId,"average-temperature");
  assertEqual(scrp.scriptStr,"temperature().window(+, 0, 4).map(a/4).send()");
  assertEqual(scrp.interval,"");
  assertEqual(scrp.feature,"temperature");
  assertFalse(scrp.execute(1));
  assertFalse(scrp.execute(2));
  assertFalse(scrp.execute(3));
  assertTrue(scrp.execute(4));// only after 4 input the value is sent to the api
  assertFalse(scrp.execute(5));
  assertFalse(scrp.execute(6));
  assertFalse(scrp.execute(7));
  assertTrue(scrp.execute(4));// only after 4 input the value is sent to the api


  scrp= script("average-temperature","temperatur(10).slidingWindow(+, 0, 6).map(a/6).send()",
                            opts.thing.c_str(), opts.device.c_str(), "url", token.c_str(), "temperature");
  assertFalse(scrp.valid); // typo in "temperatur" -> script not valid
  scrp= script("average-temperature","temperatures(10).slidingWindow(+, 0, 6).map(a/6).send()",
                            opts.thing.c_str(), opts.device.c_str(), "url", token.c_str(), "temperature");
  assertFalse(scrp.valid); // typo in "temperatures" -> script not valid
  scrp= script("average-temperature","temperature(10).sliding(+, 0, 6).map(a/6).send()",
                            opts.thing.c_str(), opts.device.c_str(), "url", token.c_str(), "temperature");
  assertFalse(scrp.valid); // "sliding" is not an operations -> script not valid
  scrp= script("average-temperature","temperature(10a).sliding(+, 0, 6).map(a/6).send()",
                            opts.thing.c_str(), opts.device.c_str(), "url", token.c_str(), "temperature");
  assertFalse(scrp.valid); // typo in "temperature(10a)" -> script not valid
  scrp= script("average-temperature","temperature(10a).sliding(+, 0, 6).map(a/6).send",
                            opts.thing.c_str(), opts.device.c_str(), "url", token.c_str(), "temperature");
  assertFalse(scrp.valid); // typo in "send" -> script not valid
  scrp= script("average-temperature","temperature.sliding(+, 0, 6).map(a/6).send()",
                            opts.thing.c_str(), opts.device.c_str(), "url", token.c_str(), "temperature");
  assertFalse(scrp.valid); // typo in "temperature" -> script not valid
}

testF(Settings,postVal){
  postVal* postV =new postVal("send()", opts.thing.c_str(), opts.device.c_str(), opts.url.c_str(),token.c_str(),
                             "temperature", "average-temperature" );
  assertEqual(postV->getName(),"send()");
  assertTrue(postV->valid);
  postV->setInput(5);
  assertEqual(*(postV->execute()),(double)5);
  assertEqual(postV->batch.size(),(size_t)(0));

  postV =new postVal("send(5)", opts.thing.c_str(), opts.device.c_str(), opts.url.c_str(),token.c_str(),
                             "temperature", "average-temperature" );
  assertEqual(postV->getName(),"send(5)");
  assertTrue(postV->valid);
  postV->setInput(5);
  assertEqual(postV->execute(),NULL);
  assertEqual(postV->batch.size(),(size_t)(1));
  postV->setInput(3);
  assertEqual(postV->execute(),NULL);
  assertEqual(postV->batch.size(),(size_t)(2));
  postV->setInput(0);
  assertEqual(postV->execute(),NULL);
  assertEqual(postV->batch.size(),(size_t)(3));
  postV->setInput(34);
  assertEqual(postV->execute(),NULL);
  assertEqual(postV->batch.size(),(size_t)(4));
  postV->setInput(5);
  assertEqual(*(postV->execute()),(double)5);
  assertEqual(postV->batch.size(),(size_t)(0));  

  postV =new postVal("send(aa)", opts.thing.c_str(), opts.device.c_str(), opts.url.c_str(),token.c_str(),
                             "temperature", "average-temperature" );
  assertFalse(postV->valid); 
  postV =new postVal("send(1aa)", opts.thing.c_str(), opts.device.c_str(), opts.url.c_str(),token.c_str(),
                             "temperature", "average-temperature" );
  assertFalse(postV->valid);  
  postV =new postVal("send(1a1)", opts.thing.c_str(), opts.device.c_str(), opts.url.c_str(),token.c_str(),
                             "temperature", "average-temperature" );
  assertFalse(postV->valid);            
}

test(sample){
  sample sam= sample("temperature");
  sam.setValue(19);
  assertEqual(sam.getValue(),(double)19);
  assertEqual(sam.feature,"temperature");
}

testF(Settings,edgine){
  edgine* Edge=edgine::getInstance();
  Edge->init(opts);// in the mocked init we retrieve 2 scripts: "temperature().send(5)"" and 
                      // "temperature(10).window(+, 0, 60).map(a/60).send()""
  sample sam= sample("temperature");
  sam.setValue(19);
  vector<sample> samples;
  samples.push_back(sam);//use this sample all the times
  for (int i = 0; i < 4; i++)
  { // for 4 times no data are sent to the API
    assertEqual(Edge->evaluate(samples), 0);
  }
  assertEqual(Edge->evaluate(samples), 1); // 5th time the first script send the datum
  assertEqual(Edge->evaluate(samples), 0);

}

testF(Settings,APIRest){
  String response=Api->POSTLogin(opts.url.c_str()+String("/")+opts.ver.c_str()+String("/")+opts.login.c_str(), opts.username.c_str(), opts.password.c_str());
  assertEqual(response, " {\"token\": \"JWT token\"}");
  response=Api->POSTLogin(opts.url.c_str()+String("/")+opts.ver.c_str()+String("/")+opts.login.c_str(), "aa","bb");
  assertEqual(response, "none");

  response= Api->GETDate(opts.url,token);
  assertEqual(response,"2019-12-14T12:25:06.324Z");
  response= Api->GETDate(opts.url.c_str(),"token");
  assertEqual(response,"none");

  response=Api->GETDescr(opts.url,token);
  assertEqual(response,"{"
        "\"features\": ["
          " \"temperature\""
        "],"
        "\"tags\": [],"
        "\"scripts\": ["
          "\"average-hourly-temperature\","
            "\"group-temperature\""
        "],"
        "\"visibility\": \"private\","
        "\"period\": 10,"
        "\"_id\": \"temperature-sensor-riccardo-office\","
        "\"owner\": {"
            "\"_id\": \"5dcec66bc67ed54963bc865c\","
            "\"username\": \"riccardo-office-temperature-sensor-username\","
            "\"type\": \"provider\""
        "}"
      "}");
  response=Api->GETDescr(opts.url.c_str(),"token");
  assertEqual(response,"none");

  response=Api->GETScript(opts.url.c_str()+String("/v1/scripts?filter={\"_id\":\"group-temperature\"}"),token.c_str());
  assertEqual(response,"{\"docs\": ["
					  "{"
						  "\"visibility\": \"private\","
						  "\"tags\": [],"
						  "\"_id\": \"group-temperature\","
						  "\"code\": \"temperature().send(5)\","
						  "\"owner\": {"
							  "\"_id\": \"5dcec66bc67ed54963bc865c\","
							  "\"username\": \"riccardo-office-temperature-sensor-username\","
							  "\"type\": \"provider\"}"
					  "}"
					"],"
					"\"totalDocs\": 1, \"limit\": 10, \"hasPrevPage\": false, \"hasNextPage\": false, \"page\": 1, \"totalPages\": 1, \"pagingCounter\": 1, \"prevPage\": null, \"nextPage\": null}");
  response=Api->GETScript(opts.url.c_str()+String("/v1/scripts?filter={\"_id\":\"average-hourly-temperature\"}"),token.c_str());
  assertEqual(response,"{\"docs\": ["
					  "{"
						  "\"visibility\": \"private\","
						  "\"tags\": [],"
						  "\"_id\": \"average-hourly-temperature\","
						  "\"code\": \"temperature(10).window(+, 0, 60).map(a/60).send()\","
						  "\"owner\": {"
							  "\"_id\": \"5dcec66bc67ed54963bc865c\","
							  "\"username\": \"riccardo-office-temperature-sensor-username\","
							  "\"type\": \"provider\"}"
						  "}"
					  "],"
					"\"totalDocs\": 1, \"limit\": 10, \"hasPrevPage\": false, \"hasNextPage\": false, \"page\": 1, \"totalPages\": 1, \"pagingCounter\": 1, \"prevPage\": null, \"nextPage\": null}");
    
  response=Api->GETScript(opts.url.c_str(),"token");
  assertEqual(response,"none");

  boolean success= Api->POSTMeasurement(opts.url.c_str(),token.c_str(),opts.thing.c_str(),
                          "temperature",opts.device.c_str(),"group-temperature",(double)5);
  assertTrue(success);
  success= Api->POSTMeasurement(opts.url.c_str(),"token",opts.thing.c_str(),
                          "temperature",opts.device.c_str(),"group-temperature",(double)5);
  assertFalse(success);

  success=Api->POSTError(opts.url.c_str(),token.c_str(),opts.thing.c_str(),
                          "temperature",opts.device.c_str(),"group-temperature");
  assertTrue(success);
  success=Api->POSTError(opts.url.c_str(),"token",opts.thing.c_str(),
                          "temperature",opts.device.c_str(),"group-temperature");
  assertFalse(success);

}

testF(Settings, connection){
  assertFalse(conn->isConnected());
  conn->setupConnection(ssidWIFI.c_str(),passWIFI.c_str());
  assertTrue(conn->isConnected());
}

void setup() {
  delay(1000); // wait for stability on some boards to prevent garbage Serial
  Serial.begin(115200);
  while (!Serial); 
}

void loop() {
  TestRunner::run();
}
