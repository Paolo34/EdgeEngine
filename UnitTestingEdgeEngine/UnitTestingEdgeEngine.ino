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
    sample* sam;

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

      sam= new sample("temperature");
      sam->date="2019-12-14T12:25:06.324Z";
      sam->device=opts.device;
      sam->url=opts.url;
      sam->thing=opts.thing;
      sam->scriptId="scriptId";

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
testF(Settings,reception){
  reception* rec= new reception("accept(10)");
  assertTrue(rec->valid);
  assertEqual(rec->getName(),"accept(10)");
  sam->value=3436;
  rec->setInput(sam);
  assertNotEqual(rec->getInterval(),0);
  assertEqual(rec->getInterval(),10);
  assertEqual(rec->execute(),NULL);
  
  rec= new reception("accept(7m)");
  rec->setInput(sam);
  assertEqual(rec->getInterval(),420);
  assertEqual(rec->execute(),NULL);
  
  rec= new reception("accept(1h)");
  rec->setInput(sam);
  assertEqual(rec->getInterval(),3600);
  assertEqual(rec->execute(),NULL);
  
  rec= new reception("accept(3d)");
  rec->setInput(sam);
  assertEqual(rec->getInterval(),259200);
  assertEqual(rec->execute(),NULL);
  
  rec= new reception("accept(0)");
  sam->value=3;
  rec->setInput(sam);
  assertEqual(rec->getInterval(),0);
  assertEqual((rec->execute())->value,(double)3);

  //if time has typo 
  rec= new reception("accept(aaa)");
  assertFalse(rec->valid);
  rec= new reception("accept(10aa)");
  assertFalse(rec->valid);
}

testF(Settings,mapVal){
  mapVal* mapV = new mapVal("map(a/6)");
  assertTrue(mapV->valid);
  assertEqual(mapV->getName(),"map(a/6)");  
  sam->value=6;
  mapV->setInput(sam);
  assertEqual((mapV->execute())->value,(double)1);
  sam->value=23;
  mapV->setInput(sam);  
  assertEqual((mapV->execute())->value,(double)(23)/(double)(6));

  mapV= new mapVal("map(a+101)");
  sam->value=9;
  mapV->setInput(sam);
  assertEqual((mapV->execute())->value,(double)(9+101));
  sam->value=25;
  mapV->setInput(sam);  
  assertEqual((mapV->execute())->value,(double)(25+101));

  mapV= new mapVal("map(a*3)");
  sam->value=9;
  mapV->setInput(sam);
  assertEqual((mapV->execute())->value,(double)(3*9));
  sam->value=1;
  mapV->setInput(sam);  
  assertEqual((mapV->execute())->value,(double)(3*1));

  mapV= new mapVal("map(a-101)");
  sam->value=3;
  mapV->setInput(sam);
  assertEqual((mapV->execute())->value,(double)(3-101));
  sam->value=205;
  mapV->setInput(sam);  
  assertEqual((mapV->execute())->value,(double)(205-101));

  mapV= new mapVal("map(a^4)");
  sam->value=9;
  mapV->setInput(sam);
  assertEqual((mapV->execute())->value,(double)(pow(9,4)));
  sam->value=45;
  mapV->setInput(sam);  
  assertEqual((mapV->execute())->value,(double)(pow(45,4)));

  mapV= new mapVal("map(a4)");
  assertFalse(mapV->valid);
  mapV= new mapVal("map(a?4)");
  assertFalse(mapV->valid);
  mapV= new mapVal("map(a/22a)");
  assertFalse(mapV->valid);
  mapV= new mapVal("map(a/a22)");
  assertFalse(mapV->valid);
}

testF(Settings,maxVal){
  maxVal* maxV= new maxVal("max()");
  assertTrue(maxV->valid);
  assertEqual(maxV->getName(),"max()");  
  sam->value=-10;
  maxV->setInput(sam);
  assertEqual((maxV->execute())->value,(double)-10);
  sam->value=-110;
  maxV->setInput(sam);
  assertEqual(maxV->execute(),NULL);
  sam->value=0;
  maxV->setInput(sam);
  assertEqual((maxV->execute())->value,(double)0);
  
  maxV =new maxVal("max()",10);
  sam->value=10;
  maxV->setInput(sam);
  assertEqual(maxV->execute(),NULL);
  sam->value=110;
  maxV->setInput(sam);
  assertEqual((maxV->execute())->value,(double)110);
  sam->value=0;
  maxV->setInput(sam);
  assertEqual(maxV->execute(),NULL);
  maxV =new maxVal("max(a)");
  assertFalse(maxV->valid);
}
testF(Settings,minVal){
  minVal* minV = new minVal("min()");
  assertTrue(minV->valid);
  assertEqual(minV->getName(),"min()");  
  sam->value=8;
  minV->setInput(sam);
  assertEqual((minV->execute())->value,(double)8);
  sam->value=-2340;
  minV->setInput(sam);
  assertEqual((minV->execute())->value,(double)-2340);
  sam->value=0;
  minV->setInput(sam);
  assertEqual(minV->execute(),NULL);
  
  minV = new minVal("min()",10);
  sam->value=20;
  minV->setInput(sam);
  assertEqual(minV->execute(),NULL);
  sam->value=110;
  minV->setInput(sam);
  assertEqual(minV->execute(),NULL);
  sam->value=0;
  minV->setInput(sam);
  assertEqual((minV->execute())->value,(double)0);
  minV =new minVal("min(19)");
  assertFalse(minV->valid);
}

testF(Settings,window){
  window* wind = new window("window(+,1,10)");
  assertTrue(wind->valid);
  assertEqual(wind->getName(),"window(+,1,10)");  
  for (int i = 1; i < 10; i++)
  {
    sam->value=i;
    wind->setInput(sam);
    assertEqual(wind->execute(),NULL);
  }
  sam->value=10;
  wind->setInput(sam);
  assertEqual((wind->execute())->value,(double) 56);

  for (int i = 1; i < 10; i++)
  {
    sam->value=3;
    wind->setInput(sam);
    assertEqual(wind->execute(),NULL);
  }
  sam->value=3;
  wind->setInput(sam);
  assertEqual((wind->execute())->value,(double)31);

  wind= new window("window(*,2,4)");
  for (int i = 1; i < 4; i++)
  {
    sam->value=i;
    wind->setInput(sam);
    assertEqual(wind->execute(),NULL);
  }
  sam->value=10;
  wind->setInput(sam);
  assertEqual((wind->execute())->value,(double) 2*1*2*3*10);

  wind= new window("window(-,7,5)");
  for (int i = 1; i < 5; i++)
  {
    sam->value=i;
    wind->setInput(sam);
    assertEqual(wind->execute(),NULL);
  }
  sam->value=5;
  wind->setInput(sam);
  assertEqual((wind->execute())->value,(double)-8);

   wind= new window("window(/,1,5)");
  for (int i = 1; i < 5; i++)
  {
    sam->value=i;
    wind->setInput(sam);
    assertEqual(wind->execute(),NULL);
  }
  sam->value=1;
  wind->setInput(sam);
  assertEqual((wind->execute())->value,(double)1/1/2/3/4/1);

  wind = new window("window(?,1,10)");
  assertFalse(wind->valid);
  wind = new window("window(*,a,10)");
  assertFalse(wind->valid);
  wind = new window("window(+,1,1e0)");
  assertFalse(wind->valid);
  wind = new window("window(*,110)");
  assertFalse(wind->valid);
}

testF(Settings,slidingWindow){
  slidingWindow* slidWind = new slidingWindow("slidingWindow(+,1,5)");
  assertTrue(slidWind->valid);
  assertEqual(slidWind->getName(),"slidingWindow(+,1,5)");  
  for (int i = 1; i < 5; i++)
  {
    sam->value=i;
    slidWind->setInput(sam);
    assertEqual(slidWind->execute(),NULL);
  }
  sam->value=33;
  slidWind->setInput(sam);
  assertEqual((slidWind->execute())->value,(double)44);
  sam->value=3;
  slidWind->setInput(sam);
  assertEqual((slidWind->execute())->value,(double)46);
  sam->value=0;
  slidWind->setInput(sam);
  assertEqual((slidWind->execute())->value,(double)44);

  slidWind = new slidingWindow("slidingWindow(-,34,3)");
  for (int i = 1; i < 3; i++)
  {
    sam->value=i;
    slidWind->setInput(sam);
    assertEqual(slidWind->execute(),NULL);
  }
  sam->value=3;
  slidWind->setInput(sam);
  assertEqual((slidWind->execute())->value,(double)28);
  sam->value=3;
  slidWind->setInput(sam);
  assertEqual((slidWind->execute())->value,(double)26);
  sam->value=0;
  slidWind->setInput(sam);
  assertEqual((slidWind->execute())->value,(double)28);

  slidWind = new slidingWindow("slidingWindow(*,1,6)");
  for (int i = 1; i < 6; i++)
  {
    sam->value=i;
    slidWind->setInput(sam);
    assertEqual(slidWind->execute(),NULL);
  }
  sam->value=10;
  slidWind->setInput(sam);
  assertEqual((slidWind->execute())->value,(double)1200);
  sam->value=3;
  slidWind->setInput(sam);
  assertEqual((slidWind->execute())->value,(double)3600);
  sam->value=1;
  slidWind->setInput(sam);
  assertEqual((slidWind->execute())->value,(double)1800);

  slidWind = new slidingWindow("slidingWindow(/,100,6)");
  for (int i = 1; i < 6; i++)
  {
    sam->value=i;
    slidWind->setInput(sam);
    assertEqual(slidWind->execute(),NULL);
  }
  sam->value=10;
  slidWind->setInput(sam);
  assertEqual((slidWind->execute())->value,(double)100/1/2/3/4/5/10);
  sam->value=3;
  slidWind->setInput(sam);
  assertEqual((slidWind->execute())->value,(double)100/2/3/4/5/10/3);
  sam->value=1;
  slidWind->setInput(sam);
  assertEqual((slidWind->execute())->value,(double)100/3/4/5/10/3/1);

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
  sam->value=1;
  assertFalse(scrp.execute(sam));
  assertFalse(scrp.execute(sam));
  assertFalse(scrp.execute(sam));
  assertFalse(scrp.execute(sam));
  assertFalse(scrp.execute(sam));
  assertTrue(scrp.execute(sam));// only after 6 input the value is sent to the api
  assertTrue(scrp.execute(sam));

  scrp= script("average-temperature","temperature().window(+, 0, 4).map(a/4).send()",
                            opts.thing.c_str(), opts.device.c_str(), "url", token.c_str(), "temperature, humidity");
  assertTrue(scrp.valid);
  assertEqual(scrp.feature,"temperature");
  assertEqual(scrp.scriptId,"average-temperature");
  assertEqual(scrp.scriptStr,"temperature().window(+, 0, 4).map(a/4).send()");
  assertEqual(scrp.interval,"");
  assertEqual(scrp.feature,"temperature");
  assertFalse(scrp.execute(sam));
  assertFalse(scrp.execute(sam));
  assertFalse(scrp.execute(sam));
  assertTrue(scrp.execute(sam));// only after 4 input the value is sent to the api
  assertFalse(scrp.execute(sam));
  assertFalse(scrp.execute(sam));
  assertFalse(scrp.execute(sam));
  assertTrue(scrp.execute(sam));// only after 4 input the value is sent to the api


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
  sam->value=5;
  postV->setInput(sam);
  assertEqual((postV->execute()->value),(double)5);
  assertEqual(postV->batch.size(),(size_t)(0));

  postV =new postVal("send(5)", opts.thing.c_str(), opts.device.c_str(), opts.url.c_str(),token.c_str(),
                             "temperature", "average-temperature" );
  assertEqual(postV->getName(),"send(5)");
  assertTrue(postV->valid);
  sam->value=5;
  postV->setInput(sam);
  assertEqual(postV->execute(),NULL);
  assertEqual(postV->batch.size(),(size_t)(1));
  sam->value=3;
  postV->setInput(sam);
  assertEqual(postV->execute(),NULL);
  assertEqual(postV->batch.size(),(size_t)(2));
  sam->value=0;
  postV->setInput(sam);
  assertEqual(postV->execute(),NULL);
  assertEqual(postV->batch.size(),(size_t)(3));
  sam->value=34;
  postV->setInput(sam);
  assertEqual(postV->execute(),NULL);
  assertEqual(postV->batch.size(),(size_t)(4));
  sam->value=5;
  postV->setInput(sam);
  assertEqual((postV->execute()->value),(double)5);
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

testF(Settings,sample){
  sam->value=19;
  assertEqual(sam->value,(double)19);
  assertEqual(sam->feature,"temperature");
}

testF(Settings,edgine){
  edgine* Edge=edgine::getInstance();
  Edge->init(opts);// in the mocked init we retrieve 2 scripts: "temperature().send(5)"" and 
                      // "temperature(10).window(+, 0, 60).map(a/60).send()""
  sam->value=19;
  vector<sample> samples;
  samples.push_back(*sam);//use this sample all the times
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

  boolean success= Api->POSTMeasurement(*sam,token.c_str());
  assertTrue(success);
  success= Api->POSTMeasurement(*sam,"token");
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
