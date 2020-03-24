// UNIT TESTING OF EDGE ENGINE

#include <AUnit.h>
using std::vector;
#include <EdgeEngine_library.h>

using namespace aunit;

class Settings :public TestOnce{
  protected:
    options opts;
    APIRest* Api;
    connection* conn;
    const char* ssidWIFI;
    const char* passWIFI;
    string token;
    sample* sam;

    //methods
    void createSample(sample* &s, float value=1){
      s= new sample("temperature");
      s->startDate="2019-12-14T12:25:06.324Z";
      s->endDate=s->startDate;
      s->device=opts.device;
      s->url=opts.url;
      s->thing=opts.thing;
      s->scriptId="scriptId";
      s->value=value;
    }

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
      opts.info= "info";
      opts.issues="issues";
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
  
  Serial.println("OPERATION");
  operation* op= new operation("operation");
  assertEqual(op->getName().c_str(),"operation");
  assertFalse(op->valid);
  delete op;
}
testF(Settings,reception){
  
  Serial.println("RECEPTION");
  reception* rec= new reception("accept(10)");
  assertTrue(rec->valid);
  assertEqual(rec->getName().c_str(),"accept(10)");
  createSample(sam,3436);
  rec->setInput(sam);
  assertNotEqual(rec->getInterval(),0);
  assertEqual(rec->getInterval(),10);
  assertEqual(rec->execute(),(float)3436);
  assertEqual(rec->execute(),NULL);// this function delete sam
  delete rec;

  rec= new reception("accept(7m)");
  createSample(sam);
  rec->setInput(sam);
  assertEqual(rec->getInterval(),420);
  assertEqual(rec->execute(),(float)1);
  assertEqual(rec->execute(),NULL);// this function delete sam
  delete rec;
  
  rec= new reception("accept(1h)");
  createSample(sam);
  rec->setInput(sam);
  assertEqual(rec->getInterval(),3600);
  assertEqual(rec->execute(),(float)1);
  assertEqual(rec->execute(),NULL);// this function delete sam
  delete rec;
  
  rec= new reception("accept(3d)");
  createSample(sam);
  rec->setInput(sam);
  assertEqual(rec->getInterval(),259200);
  assertEqual(rec->execute(),(float)1);
  assertEqual(rec->execute(),NULL);// this function delete sam
  delete rec;
  
  rec= new reception("accept(0)");
  createSample(sam,3);
  rec->setInput(sam);
  assertEqual(rec->getInterval(),0);
  assertEqual((rec->execute())->value,(float)3);
  delete sam;
  delete rec;

  //if time has typo 
  rec= new reception("accept(aaa)");
  assertFalse(rec->valid);
  delete rec;
  rec= new reception("accept(10aa)");
  assertFalse(rec->valid);
  delete rec;
}

testF(Settings,mapVal){
  
  
  Serial.println("MAPVAL");
  mapVal* mapV = new mapVal("map(a/6)");
  assertTrue(mapV->valid);
  assertEqual(mapV->getName().c_str(),"map(a/6)");  
  createSample(sam,6);
  mapV->setInput(sam);
  assertEqual((mapV->execute())->value,(float)1);
  sam->value=23;
  mapV->setInput(sam);  
  assertEqual((mapV->execute())->value,(float)(23)/(float)(6));
  delete mapV;

  mapV= new mapVal("map(a+101)");
  sam->value=9;
  mapV->setInput(sam);
  assertEqual((mapV->execute())->value,(float)(9+101));
  sam->value=25;
  mapV->setInput(sam);  
  assertEqual((mapV->execute())->value,(float)(25+101));
  delete mapV;

  mapV= new mapVal("map(a*3)");
  sam->value=9;
  mapV->setInput(sam);
  assertEqual((mapV->execute())->value,(float)(3*9));
  sam->value=1;
  mapV->setInput(sam);  
  assertEqual((mapV->execute())->value,(float)(3*1));
  delete mapV;

  mapV= new mapVal("map(a-101)");
  sam->value=3;
  mapV->setInput(sam);
  assertEqual((mapV->execute())->value,(float)(3-101));
  sam->value=205;
  mapV->setInput(sam);  
  assertEqual((mapV->execute())->value,(float)(205-101));
  delete mapV;

  mapV= new mapVal("map(a^4)");
  sam->value=9;
  mapV->setInput(sam);
  assertEqual((mapV->execute())->value,(float)(pow(9,4)));
  sam->value=45;
  mapV->setInput(sam);  
  assertEqual((mapV->execute())->value,(float)(pow(45,4)));
  delete mapV;
  delete sam;

  mapV= new mapVal("map(a4)");
  assertFalse(mapV->valid);
  delete mapV;
  mapV= new mapVal("map(a+)");
  assertFalse(mapV->valid);
  delete mapV;
  mapV= new mapVal("map(a?4)");
  assertFalse(mapV->valid);
  delete mapV;
  mapV= new mapVal("map(a/22a)");
  assertFalse(mapV->valid);
  delete mapV;
  mapV= new mapVal("map(a/a22)");
  assertFalse(mapV->valid);
  delete mapV;
  
  
}

testF(Settings,maxVal){
  
  
  Serial.println("MAXVAL");
  maxVal* maxV= new maxVal("max()");
  assertTrue(maxV->valid);
  assertEqual(maxV->getName().c_str(),"max()");  
  createSample(sam,-10);
  maxV->setInput(sam);
  assertEqual((maxV->execute())->value,(float)-10);
  sam->value=-110;
  maxV->setInput(sam);
  assertEqual(maxV->execute(),NULL);// this function delete sam
  createSample(sam,0);
  maxV->setInput(sam);
  assertEqual((maxV->execute())->value,(float)0);
  delete maxV;

  maxV =new maxVal("max()",10);
  assertTrue(maxV->valid);
  sam->value=10;
  maxV->setInput(sam);
  assertEqual(maxV->execute(),NULL);// this function delete sam
  createSample(sam,110);
  maxV->setInput(sam);
  assertEqual((maxV->execute())->value,(float)110);
  sam->value=0;
  maxV->setInput(sam);
  assertEqual(maxV->execute(),NULL);// this function delete sam
  delete maxV;

  maxV =new maxVal("max(a)");
  assertFalse(maxV->valid);
  
  
}
testF(Settings,minVal){
  Serial.println("MINVAL");
  minVal* minV = new minVal("min()");
  assertTrue(minV->valid);
  assertEqual(minV->getName().c_str(),"min()");  
  createSample(sam,8);
  minV->setInput(sam);
  assertEqual((minV->execute())->value,(float)8);
  sam->value=-2340;
  minV->setInput(sam);
  assertEqual((minV->execute())->value,(float)-2340);
  sam->value=0;
  minV->setInput(sam);
  assertEqual(minV->execute(),NULL);// this function delete sam
  delete minV;
  
  minV = new minVal("min()",10);
  createSample(sam,20);
  minV->setInput(sam);
  assertEqual(minV->execute(),NULL);// this function delete sam
  createSample(sam,110);
  minV->setInput(sam);
  assertEqual(minV->execute(),NULL);// this function delete sam
  createSample(sam,0);
  minV->setInput(sam);
  assertEqual((minV->execute())->value,(float)0);
  delete sam;
  delete minV;

  minV =new minVal("min(19)");
  assertFalse(minV->valid);
  delete minV;
}

testF(Settings,filter){
  Serial.println("FILTER");
  filter* flt = new filter("filter(a<6)");
  assertTrue(flt->valid);
  assertEqual(flt->getName().c_str(),"filter(a<6)");  
  createSample(sam,5);
  flt->setInput(sam);
  assertEqual((flt->execute())->value,(float)5);
  sam->value=-2340;
  flt->setInput(sam);
  assertEqual((flt->execute())->value,(float)-2340);
  sam->value=6;
  flt->setInput(sam);
  assertEqual(flt->execute(),NULL);// this function delete sam
  delete flt;
  
  flt = new filter("filter(a==7.3)");
  assertTrue(flt->valid);
  assertEqual(flt->getName().c_str(),"filter(a==7.3)");  
  createSample(sam,7.3);
  flt->setInput(sam);
  assertEqual((flt->execute())->value,(float)7.3);
  sam->value=-2340;
  flt->setInput(sam);
  assertEqual(flt->execute(),NULL);// this function delete sam
  createSample(sam,7);  
  flt->setInput(sam);
  assertEqual(flt->execute(),NULL);// this function delete sam
  delete flt;

  flt = new filter("filter(a>=21)");
  createSample(sam,20);
  flt->setInput(sam);
  assertEqual(flt->execute(),NULL);// this function delete sam
  createSample(sam,10);
  flt->setInput(sam);
  assertEqual(flt->execute(),NULL);// this function delete sam
  createSample(sam,21);
  flt->setInput(sam);
  assertEqual((flt->execute())->value,(float)21);
  delete sam;
  delete flt;
  
  flt = new filter("filter(aC[6,95])");
  assertTrue(flt->valid);
  assertEqual(flt->getName().c_str(),"filter(aC[6,95])");  
  createSample(sam,53);
  flt->setInput(sam);
  assertEqual((flt->execute())->value,(float)53);
  sam->value=95;
  flt->setInput(sam);
  assertEqual((flt->execute())->value,(float)95);
  sam->value=6;
  flt->setInput(sam);
  assertEqual((flt->execute())->value,(float)6);
  sam->value=88;
  flt->setInput(sam);
  assertEqual((flt->execute())->value,(float)88);
  sam->value=96;
  flt->setInput(sam);
  assertEqual(flt->execute(),NULL);// this function delete sam
  createSample(sam,5);
  flt->setInput(sam);
  assertEqual(flt->execute(),NULL);// this function delete sam
  delete flt;

  flt = new filter("filter(a/C[6,95])");
  assertTrue(flt->valid);
  assertEqual(flt->getName().c_str(),"filter(a/C[6,95])");  
  createSample(sam,6);
  flt->setInput(sam);
  assertEqual((flt->execute())->value,(float)6);
  sam->value=95;
  flt->setInput(sam);
  assertEqual((flt->execute())->value,(float)95);
  sam->value=-10;
  flt->setInput(sam);
  assertEqual((flt->execute())->value,(float)-10);
  sam->value=188;
  flt->setInput(sam);
  assertEqual((flt->execute())->value,(float)188);
  sam->value=90;
  flt->setInput(sam);
  assertEqual(flt->execute(),NULL);// this function delete sam
  createSample(sam,15);
  flt->setInput(sam);
  assertEqual(flt->execute(),NULL);// this function delete sam
  delete flt;

  
  flt =new filter("filter(19)");
  assertFalse(flt->valid);
  delete flt;
  flt =new filter("filter()");
  assertFalse(flt->valid);
  delete flt;
  flt =new filter("filter(a>)");
  assertFalse(flt->valid);
  delete flt;
  flt =new filter("filter(a>>19)");
  assertFalse(flt->valid);
  delete flt;
  flt =new filter("filter(<19)");
  assertFalse(flt->valid);
  delete flt;
  flt =new filter("filter(1>!9)");
  assertFalse(flt->valid);
  delete flt;
  flt =new filter("filter(1,9)");
  assertFalse(flt->valid);
  delete flt;
  
  flt =new filter("filter(aC[19])");
  assertFalse(flt->valid);
  delete flt;
  flt =new filter("filter(aC[1,9,])");
  assertFalse(flt->valid);
  delete flt;
  flt =new filter("filter(aC[,19])");
  assertFalse(flt->valid);
  delete flt;
  flt =new filter("filter(aC[19,55)");
  assertFalse(flt->valid);
  delete flt;
  flt =new filter("filter(aC[19,19])");
  assertFalse(flt->valid);
  delete flt;
  flt =new filter("filter(a/C[19])");
  assertFalse(flt->valid);
  delete flt;
  flt =new filter("filter(a/C[1,9,])");
  assertFalse(flt->valid);
  delete flt;
  flt =new filter("filter(a/C[,19])");
  assertFalse(flt->valid);
  delete flt;
  flt =new filter("filter(a/C[19,55)");
  assertFalse(flt->valid);
  delete flt;
  flt =new filter("filter(a/[19,19])");
  assertFalse(flt->valid);
  delete flt;
}

testF(Settings,window){
  Serial.println("WINDOW");
  window* wind = new window("window(+,1,10)",10);
  assertTrue(wind->valid);
  assertEqual(wind->getName().c_str(),"window(+,1,10)");  
  for (int i = 1; i < 10; i++)
  {
    createSample(sam,i);
    wind->setInput(sam);
    assertEqual(wind->execute(),NULL);// this function delete sam
  }
  createSample(sam,10);
  wind->setInput(sam);
  assertEqual((wind->execute())->value,(float) 56);
  delete sam;

  for (int i = 1; i < 10; i++)
  {
    createSample(sam,3);
    wind->setInput(sam);
    assertEqual(wind->execute(),NULL);// this function delete sam
  }
  createSample(sam,3);;
  wind->setInput(sam);
  assertEqual((wind->execute())->value,(float)31);
  delete sam;
  delete wind;

  wind= new window("window(*,2,4)",5);
  for (int i = 1; i < 4; i++)
  {
    createSample(sam,i);
    wind->setInput(sam);
    assertEqual(wind->execute(),NULL);// this function delete sam
  }
  createSample(sam,10);
  wind->setInput(sam);
  assertEqual((wind->execute())->value,(float) 2*1*2*3*10);
  delete sam;
  delete wind;

  wind= new window("window(-,7,5)",5);
  for (int i = 1; i < 5; i++)
  {
    createSample(sam,i);
    wind->setInput(sam);
    assertEqual(wind->execute(),NULL);// this function delete sam
  }
  createSample(sam,5);
  wind->setInput(sam);
  assertEqual((wind->execute())->value,(float)-8);
  delete sam;
  delete wind;

  wind= new window("window(/,1,5)",5);
  for (int i = 1; i < 5; i++)
  {
    createSample(sam,i);
    wind->setInput(sam);
    assertEqual(wind->execute(),NULL);// this function delete sam
  }
  createSample(sam,1);
  wind->setInput(sam);
  assertEqual((wind->execute())->value,(float)1/1/2/3/4/1);
  delete sam;
  delete wind;

  wind = new window("window(?,1,10)",10);
  assertFalse(wind->valid);
  delete wind;
  wind = new window("window(*,a,10)",10);
  assertFalse(wind->valid);
  delete wind;
  wind = new window("window(+,1,1e0)",10);
  assertFalse(wind->valid);
  delete wind;
  wind = new window("window(*,110)",10);
  assertFalse(wind->valid);
  delete wind;
  wind = new window("window(+,1,10)",9);
  assertFalse(wind->valid);
  delete wind;
  wind = new window("window(*,1,3)",2);
  assertFalse(wind->valid);
  delete wind;
}

testF(Settings,slidingWindow){
  
  Serial.println("SLIDINGWINDOW");
  slidingWindow* slidWind = new slidingWindow("slidingWindow(+,1,5)",5);
  assertTrue(slidWind->valid);
  assertEqual(slidWind->getName().c_str(),"slidingWindow(+,1,5)");  
  for (int i = 1; i < 5; i++)
  {
    createSample(sam,i);//different insances of sample are needed
    slidWind->setInput(sam);
    assertEqual(slidWind->execute(),NULL);
  }
  createSample(sam,33);//different insances of sample are needed
  slidWind->setInput(sam);
  assertEqual((slidWind->execute())->value,(float)44);
  createSample(sam,3);//different insances of sample are needed
  slidWind->setInput(sam);
  assertEqual((slidWind->execute())->value,(float)46);
  createSample(sam,0);//different insances of sample are needed
  slidWind->setInput(sam);
  assertEqual((slidWind->execute())->value,(float)44);
  delete slidWind;

  slidWind = new slidingWindow("slidingWindow(-,34,3)",3);
  for (int i = 1; i < 3; i++)
  {
    createSample(sam,i);//different insances of sample are needed
    slidWind->setInput(sam);
    assertEqual(slidWind->execute(),NULL);
  }
  createSample(sam,3);//different insances of sample are needed
  slidWind->setInput(sam);
  assertEqual((slidWind->execute())->value,(float)28);
  createSample(sam,3);//different insances of sample are needed
  slidWind->setInput(sam);
  assertEqual((slidWind->execute())->value,(float)26);
  createSample(sam,0);//different insances of sample are needed
  slidWind->setInput(sam);
  assertEqual((slidWind->execute())->value,(float)28);
  delete slidWind;

  slidWind = new slidingWindow("slidingWindow(*,1,6)",6);
  for (int i = 1; i < 6; i++)
  {
    createSample(sam,i);//different insances of sample are needed
    slidWind->setInput(sam);
    assertEqual(slidWind->execute(),NULL);
  }
  createSample(sam,10);//different insances of sample are needed
  slidWind->setInput(sam);
  assertEqual((slidWind->execute())->value,(float)1200);
  createSample(sam,3);//different insances of sample are needed
  slidWind->setInput(sam);
  assertEqual((slidWind->execute())->value,(float)3600);
  createSample(sam,1);//different insances of sample are needed
  slidWind->setInput(sam);
  assertEqual((slidWind->execute())->value,(float)1800);
  delete slidWind;

  slidWind = new slidingWindow("slidingWindow(/,100,6)",6);
  for (int i = 1; i < 6; i++)
  {
    createSample(sam,i);//different insances of sample are needed
    slidWind->setInput(sam);
    assertEqual(slidWind->execute(),NULL);
  }
  createSample(sam,10);//different insances of sample are needed
  slidWind->setInput(sam);
  assertEqual((slidWind->execute())->value,(float)100/1/2/3/4/5/10);
  createSample(sam,3);//different insances of sample are needed
  slidWind->setInput(sam);
  assertEqual((slidWind->execute())->value,(float)100/2/3/4/5/10/3);
  createSample(sam,1);//different insances of sample are needed
  slidWind->setInput(sam);
  assertEqual((slidWind->execute())->value,(float)100/3/4/5/10/3/1);
  delete slidWind;
  slidWind = new slidingWindow("slidingWindow(?,1,10)",10);
  assertFalse(slidWind->valid);
  delete slidWind;
  slidWind = new slidingWindow("slidingWindow(*,a,10)",1);
  assertFalse(slidWind->valid);
  delete slidWind;
  slidWind = new slidingWindow("slidingWindow(+,1,1e0)",1);
  assertFalse(slidWind->valid);
  delete slidWind;
  slidWind = new slidingWindow("slidingWindow(*,*,110)",110);
  assertFalse(slidWind->valid);
  delete slidWind;
  slidWind = new slidingWindow("slidingWindow()",1);
  assertFalse(slidWind->valid);
  delete slidWind;
  slidWind = new slidingWindow("slidingWindow(*,1,110)",109);
  assertFalse(slidWind->valid);
  delete slidWind;
  slidWind = new slidingWindow("slidingWindow(+,1,2)",1);
  assertFalse(slidWind->valid);
  delete slidWind;
}
testF(Settings,average){
  
  Serial.println("AVERAGE");
  average* avg = new average("average(5)",5);
  assertTrue(avg->valid);
  assertEqual(avg->getName().c_str(),"average(5)");  
  for (int i = 1; i < 5; i++)
  {
    createSample(sam,i);//different insances of sample are needed
    avg->setInput(sam);
    assertEqual(avg->execute(),NULL);
  }
  createSample(sam,33);//different insances of sample are needed
  avg->setInput(sam);
  assertEqual((avg->execute())->value,(float)43/5);
  createSample(sam,3);//different insances of sample are needed
  avg->setInput(sam);
  assertEqual((avg->execute())->value,(float)45/5);
  createSample(sam,0);//different insances of sample are needed
  avg->setInput(sam);
  assertEqual((avg->execute())->value,(float)43/5);
  delete avg;

  avg = new average("average(3)",3);
  for (int i = 1; i < 3; i++)
  {
    createSample(sam,i);//different insances of sample are needed
    avg->setInput(sam);
    assertEqual(avg->execute(),NULL);
  }
  createSample(sam,3);//different insances of sample are needed
  avg->setInput(sam);
  assertEqual((avg->execute())->value,(float)6/3);
  createSample(sam,3);//different insances of sample are needed
  avg->setInput(sam);
  assertEqual((avg->execute())->value,(float)8/3);
  createSample(sam,0);//different insances of sample are needed
  avg->setInput(sam);
  assertEqual((avg->execute())->value,(float)6/3);
  delete avg;

  avg = new average("average(6)",6);
  for (int i = 1; i < 6; i++)
  {
    createSample(sam,i);//different insances of sample are needed
    avg->setInput(sam);
    assertEqual(avg->execute(),NULL);
  }
  createSample(sam,10);//different insances of sample are needed
  avg->setInput(sam);
  assertEqual((avg->execute())->value,(float)25/6);
  createSample(sam,3);//different insances of sample are needed
  avg->setInput(sam);
  assertEqual((avg->execute())->value,(float)27/6);
  createSample(sam,1);//different insances of sample are needed
  avg->setInput(sam);
  assertEqual((avg->execute())->value,(float)26/6);
  delete avg;

  avg = new average("average(6)",6);
  for (int i = 11; i < 16; i++)
  {
    createSample(sam,i);//different insances of sample are needed
    avg->setInput(sam);
    assertEqual(avg->execute(),NULL);
  }
  createSample(sam,10);//different insances of sample are needed
  avg->setInput(sam);
  assertEqual((avg->execute())->value,(float)75/6);
  createSample(sam,3);//different insances of sample are needed
  avg->setInput(sam);
  assertEqual((avg->execute())->value,(float)67/6);
  createSample(sam,1);//different insances of sample are needed
  avg->setInput(sam);
  assertEqual((avg->execute())->value,(float)56/6);
  delete avg;
  avg = new average("average(?)",10);
  assertFalse(avg->valid);
  delete avg;
  avg = new average("average(a)",10);
  assertFalse(avg->valid);
  delete avg;
  avg = new average("average(1e0)",10);
  assertFalse(avg->valid);
  delete avg;
  avg = new average("average(*,*)",10);
  assertFalse(avg->valid);
  delete avg;
  avg = new average("average()",10);
  assertFalse(avg->valid);
  delete avg;
  avg = new average("average(3)",2);
  assertFalse(avg->valid);
  delete avg;
  avg = new average("average(5)",3);
  assertFalse(avg->valid);
  delete avg;
  
}


testF(Settings,stdDeviation){
  
  Serial.println("STDDEVIATION");
  stdDeviation* stdDev = new stdDeviation("stdDeviation(5)",5);
  assertTrue(stdDev->valid);
  assertEqual(stdDev->getName().c_str(),"stdDeviation(5)");  
  for (int i = 1; i < 5; i++)
  {
    createSample(sam,i);//different insances of sample are needed
    stdDev->setInput(sam);
    assertEqual(stdDev->execute(),NULL);
  }
  createSample(sam,33);//different insances of sample are needed
  stdDev->setInput(sam);
  assertNear((stdDev->execute())->value, (float)sqrt( ( pow(1.0-43.0/5,2)+pow(2.0-43.0/5,2)+pow(3.0-43.0/5,2)+pow(4.0-43.0/5,2)+pow(33.0-43.0/5,2) )/5 ), 1e-6 );
  createSample(sam,3);//different insances of sample are needed
  stdDev->setInput(sam);
  assertNear((stdDev->execute())->value, (float)sqrt( ( pow(3.0-45.0/5,2)+pow(2.0-45.0/5,2)+pow(3.0-45.0/5,2)+pow(4.0-45.0/5,2)+pow(33.0-45.0/5,2) )/5 ), 1e-6);
  createSample(sam,0);//different insances of sample are needed
  stdDev->setInput(sam);
  assertNear((stdDev->execute())->value, (float)sqrt( ( pow(3.0-43.0/5,2)+pow(0.0-43.0/5,2)+pow(3.0-43.0/5,2)+pow(4.0-43.0/5,2)+pow(33.0-43.0/5,2) )/5 ), 1e-6);
  delete stdDev;

  stdDev = new stdDeviation("stdDeviation(3)",3);
  for (int i = 1; i < 3; i++)
  {
    createSample(sam,i);//different insances of sample are needed
    stdDev->setInput(sam);
    assertEqual(stdDev->execute(),NULL);
  }
  createSample(sam,3);//different insances of sample are needed
  stdDev->setInput(sam);
  assertNear((stdDev->execute())->value,(float)sqrt( ( pow(1.0-6.0/3,2)+pow(2.0-6.0/3,2)+pow(3.0-6.0/3,2) )/3 ), 1e-6);
  createSample(sam,3);//different insances of sample are needed
  stdDev->setInput(sam);
  assertNear((stdDev->execute())->value,(float)sqrt( ( pow(3.0-8.0/3,2)+pow(2.0-8.0/3,2)+pow(3.0-8.0/3,2) )/3 ), 1e-6);
  createSample(sam,0);//different insances of sample are needed
  stdDev->setInput(sam);
  assertNear((stdDev->execute())->value,(float)sqrt( ( pow(3.0-6.0/3,2)+pow(0.0-6.0/3,2)+pow(3.0-6.0/3,2) )/3 ), 1e-6);
  delete stdDev;

  stdDev = new stdDeviation("stdDeviation(6)",7);
  for (int i = 11; i < 16; i++)
  {
    createSample(sam,i);//different insances of sample are needed
    stdDev->setInput(sam);
    assertEqual(stdDev->execute(),NULL);
  }
  createSample(sam,10);//different insances of sample are needed
  stdDev->setInput(sam);
  assertNear((stdDev->execute())->value, (float)sqrt( ( pow(11.0-75.0/6,2)+pow(12.0-75.0/6,2)+pow(13.0-75.0/6,2)+pow(14.0-75.0/6,2)+pow(15.0-75.0/6,2)+pow(10.0-75.0/6,2) )/6 ), 1e-6 );
  createSample(sam,3);//different insances of sample are needed
  stdDev->setInput(sam);
  assertNear((stdDev->execute())->value,(float)sqrt( ( pow(3.0-67.0/6,2)+pow(12.0-67.0/6,2)+pow(13.0-67.0/6,2)+pow(14.0-67.0/6,2)+pow(15.0-67.0/6,2)+pow(10.0-67.0/6,2) )/6 ), 1e-6);
  createSample(sam,1);//different insances of sample are needed
  stdDev->setInput(sam);
  assertNear((stdDev->execute())->value,(float)sqrt( ( pow(3.0-56.0/6,2)+pow(1.0-56.0/6,2)+pow(13.0-56.0/6,2)+pow(14.0-56.0/6,2)+pow(15.0-56.0/6,2)+pow(10.0-56.0/6,2) )/6 ), 1e-6);
  delete stdDev;
  stdDev = new stdDeviation("stdDeviation(?)",10);
  assertFalse(stdDev->valid);
  delete stdDev;
  stdDev = new stdDeviation("stdDeviation(a)",10);
  assertFalse(stdDev->valid);
  delete stdDev;
  stdDev = new stdDeviation("stdDeviation(1e0)",10);
  assertFalse(stdDev->valid);
  delete stdDev;
  stdDev = new stdDeviation("stdDeviation(*,*)",10);
  assertFalse(stdDev->valid);
  delete stdDev;
  stdDev = new stdDeviation("stdDeviation()",10);
  assertFalse(stdDev->valid);
  delete stdDev;
  stdDev = new stdDeviation("stdDeviation(4)",2);
  assertFalse(stdDev->valid);
  delete stdDev;
  stdDev = new stdDeviation("stdDeviation(15)",14);
  assertFalse(stdDev->valid);
  delete stdDev;
  
}

testF(Settings,median){
  
  Serial.println("MEDIAN");
  median* med = new median("median(5)",8);
  assertTrue(med->valid);
  assertEqual(med->getName().c_str(),"median(5)");
  for (int i = 4; i > 0; i--)
  {
    createSample(sam,i);//different insances of sample are needed
    med->setInput(sam);
    assertEqual(med->execute(),NULL);
  }
  createSample(sam,33);//different insances of sample are needed
  med->setInput(sam);
  assertEqual((med->execute())->value,(float)3);
  createSample(sam,3);//different insances of sample are needed
  med->setInput(sam);
  assertEqual((med->execute())->value,(float)3);
  createSample(sam,0);//different insances of sample are needed
  med->setInput(sam);
  assertEqual((med->execute())->value,(float)2);
  delete med;
  
  med = new median("median(3)",8);
  for (int i = 7; i < 9; i++)
  {
    createSample(sam,i);//different insances of sample are needed
    med->setInput(sam);
    assertEqual(med->execute(),NULL);
  }
  createSample(sam,3);//different insances of sample are needed
  med->setInput(sam);
  assertEqual((med->execute())->value,(float)7);
  createSample(sam,3);//different insances of sample are needed
  med->setInput(sam);
  assertEqual((med->execute())->value,(float)3);
  createSample(sam,0);//different insances of sample are needed
  med->setInput(sam);
  assertEqual((med->execute())->value,(float)3);
  delete med;

  med = new median("median(6)",8);
  for (int i = 1; i < 6; i++)
  {
    createSample(sam,i);//different insances of sample are needed
    med->setInput(sam);
    assertEqual(med->execute(),NULL);
  }
  createSample(sam,10);//different insances of sample are needed
  med->setInput(sam);
  assertEqual((med->execute())->value,(float)7/2);
  createSample(sam,3);//different insances of sample are needed
  med->setInput(sam);
  assertEqual((med->execute())->value,(float)7/2);
  createSample(sam,18);//different insances of sample are needed
  med->setInput(sam);
  assertEqual((med->execute())->value,(float)9/2);
  delete med;

  med = new median("median(6)",8);
  for (int i = 11; i < 16; i++)
  {
    createSample(sam,i);//different insances of sample are needed
    med->setInput(sam);
    assertEqual(med->execute(),NULL);
  }
  createSample(sam,10);//different insances of sample are needed
  med->setInput(sam);
  assertEqual((med->execute())->value,(float)25/2);
  createSample(sam,15);//different insances of sample are needed
  med->setInput(sam);
  assertEqual((med->execute())->value,(float)27/2);
  createSample(sam,15);//different insances of sample are needed
  med->setInput(sam);
  assertEqual((med->execute())->value,(float)29/2);
  delete med;
  med = new median("median(?)",10);
  assertFalse(med->valid);
  delete med;
  med = new median("median(a)",10);
  assertFalse(med->valid);
  delete med;
  med = new median("median(1e0)",10);
  assertFalse(med->valid);
  delete med;
  med = new median("median(*,*)",10);
  assertFalse(med->valid);
  delete med;
  med = new median("median()",10);
  assertFalse(med->valid);
  delete med;
  med = new median("median(3)",2);
  assertFalse(med->valid);
  delete med;
  med = new median("median(111)",1);
  assertFalse(med->valid);
  delete med;
}

testF(Settings,script){
  Serial.println("SCRIPT");

  script* scrp=new  script("average-temperature","temperature(10).slidingWindow(+, 0, 6).map(a/6).send()",
                            opts.thing, opts.device, "url", token, "\"temperature\", \"motion\"",1,3,6);
  assertTrue(scrp->valid);
  assertEqual(scrp->feature.c_str(),"temperature");
  assertEqual(scrp->scriptId.c_str(),"average-temperature");
  assertEqual(scrp->scriptStr.c_str(),"temperature(10).slidingWindow(+, 0, 6).map(a/6).send()");
  assertEqual(scrp->feature.c_str(),"temperature");
  vector<operation*> ops=scrp->operations;
  assertEqual(ops.size(),(size_t) 4); 
  assertEqual(ops[0]->getName().c_str(),"accept(10)");
  assertTrue(ops[0]->valid);
  assertEqual(ops[1]->getName().c_str(),"slidingWindow(+,0,6)");//spaces are deleted by the parser
  assertTrue(ops[1]->valid);
  assertEqual(ops[2]->getName().c_str(),"map(a/6)");
  assertTrue(ops[2]->valid);
  assertEqual(ops[3]->getName().c_str(),"send()");
  assertTrue(ops[3]->valid);
  delete scrp;
  
  scrp= new script("average-temperature","temperature().slidingWindow(+, 0, 6).map(a/6).send()",
                            opts.thing, opts.device, "url", token,  "\"temperature\", \"motion\"",2,5,8);
  assertTrue(scrp->valid);
  assertEqual(scrp->feature.c_str(),"temperature");
  assertEqual(scrp->scriptId.c_str(),"average-temperature");
  assertEqual(scrp->scriptStr.c_str(),"temperature().slidingWindow(+, 0, 6).map(a/6).send()");
  assertEqual(scrp->feature.c_str(),"temperature");

  ops=scrp->operations;
  assertEqual(ops.size(),(size_t) 4); 
  assertEqual(ops[0]->getName().c_str(),"accept()");
  assertTrue(ops[0]->valid);
  assertEqual(ops[1]->getName().c_str(),"slidingWindow(+,0,6)");//spaces are deleted by the parser
  assertTrue(ops[1]->valid);
  assertEqual(ops[2]->getName().c_str(),"map(a/6)");
  assertTrue(ops[2]->valid);
  assertEqual(ops[3]->getName().c_str(),"send()");
  assertTrue(ops[3]->valid);

  createSample(sam);
  assertFalse(scrp->execute(sam));
  assertFalse(scrp->execute(sam));
  assertFalse(scrp->execute(sam));
  assertFalse(scrp->execute(sam));
  assertFalse(scrp->execute(sam));
  assertTrue(scrp->execute(sam));// only after 6 input the value is sent to the api, this line delete sam
  createSample(sam);
  assertTrue(scrp->execute(sam));
  delete scrp;
  
  scrp= new script("average-temperature","temperature().window(+, 0, 4).map(a/4).send()",
                            opts.thing, opts.device, "url", token,  "\"temperature\", \"motion\"",1,3,4);
  assertTrue(scrp->valid);
  assertEqual(scrp->feature.c_str(),"temperature");
  assertEqual(scrp->scriptId.c_str(),"average-temperature");
  assertEqual(scrp->scriptStr.c_str(),"temperature().window(+, 0, 4).map(a/4).send()");
  assertEqual(scrp->feature.c_str(),"temperature");

  assertFalse(scrp->execute(sam));// this line delete sam
  createSample(sam);
  assertFalse(scrp->execute(sam));// this line delete sam
  createSample(sam);
  assertFalse(scrp->execute(sam));// this line delete sam
  createSample(sam);
  assertTrue(scrp->execute(sam));// only after 4 input the value is sent to the api (this line delete sam)
  createSample(sam);
  assertFalse(scrp->execute(sam));// this line delete sam
  createSample(sam);
  assertFalse(scrp->execute(sam));// this line delete sam
  createSample(sam);
  assertFalse(scrp->execute(sam));// this line delete sam
  createSample(sam);
  assertTrue(scrp->execute(sam));// only after 4 input the value is sent to the api (this line delete sam)
  delete scrp;

  scrp= new script("average-temperature","temperatur(10).slidingWindow(+, 0, 6).map(a/6).send()",
                            opts.thing, opts.device, "url", token, "temperature",1,3,6);
  assertFalse(scrp->valid); // typo in "temperatur" -> script not valid
  delete scrp;
  scrp= new script("average-temperature","temperatures(10).slidingWindow(+, 0, 6).map(a/6).send()",
                            opts.thing, opts.device, "url", token, "temperature",1,3,6);
  assertFalse(scrp->valid); // typo in "temperatures" -> script not valid
  delete scrp;
  scrp= new script("average-temperature","temperature(10).sliding(+, 0, 6).map(a/6).send()",
                            opts.thing, opts.device, "url", token, "temperature",1,3,6);
  assertFalse(scrp->valid); // "sliding" is not an operations -> script not valid
  delete scrp;
  scrp= new script("average-temperature","temperature(10a).sliding(+, 0, 6).map(a/6).send()",
                            opts.thing, opts.device, "url", token, "temperature",1,3,6);
  assertFalse(scrp->valid); // typo in "temperature(10a)" -> script not valid
  delete scrp;
  scrp= new script("average-temperature","temperature(10a).sliding(+, 0, 6).map(a/6).send",
                            opts.thing, opts.device, "url", token, "temperature",1,3,6);
  assertFalse(scrp->valid); // typo in "send" -> script not valid
  delete scrp;
  scrp= new script("average-temperature","temperature.sliding(+, 0, 6).map(a/6).send()",
                            opts.thing, opts.device, "url", token, "temperature",1,3,6);
  assertFalse(scrp->valid); // typo in "temperature" -> script not valid
  delete scrp;
  scrp= new script("average-temperature","temperature(10).sliding(+, 0, 6).map(a/6).send()",
                            opts.thing, opts.device, "url", token, "temperature",1,2,6);
  assertFalse(scrp->valid); // typo in "temperature(10a)" -> script not valid
  delete scrp;
  scrp= new script("average-temperature","temperature(10).sliding(+, 0, 6).map(a/6).send",
                            opts.thing, opts.device, "url", token, "temperature",1,3,5);
  assertFalse(scrp->valid); // typo in "send" -> script not valid
  delete scrp;
  scrp= new script("average-temperature","temperature.sliding(+, 0, 6).map(a/6).send(10)",
                            opts.thing, opts.device, "url", token, "temperature",1,3,6);
  assertFalse(scrp->valid); // typo in "temperature" -> script not valid
  delete scrp;
}

testF(Settings,postVal){
  Serial.println("POSTVAL");
  token="JWT token";
  postVal* postV = new postVal("send()", opts.thing, opts.device, opts.url,token,
                             "temperature", "average-temperature",1 );
  assertEqual(postV->getName().c_str(),"send()");
  assertTrue(postV->valid);
  createSample(sam,5);
  postV->setInput(sam);
  assertEqual((postV->execute())->value,(float)5);// this function delete sam
  createSample(sam);
  assertEqual(postV->batch.size(),(size_t)(0));
  delete postV;

  postV = new postVal("send(5)", opts.thing, opts.device, opts.url,token,
                             "temperature", "average-temperature",5 );
  assertEqual(postV->getName().c_str(),"send(5)");
  assertTrue(postV->valid);
  postV->setInput(sam);
  assertEqual(postV->execute(),NULL);//#1
  assertEqual(postV->batch.size(),(size_t)(1));
  createSample(sam);//different insances of sample are needed
  postV->setInput(sam);
  assertEqual(postV->execute(),NULL);//#2
  assertEqual(postV->batch.size(),(size_t)(2));
  createSample(sam);//different insances of sample are needed
  postV->setInput(sam);
  assertEqual(postV->execute(),NULL);//#3
  assertEqual(postV->batch.size(),(size_t)(3));
  createSample(sam);//different insances of sample are needed
  postV->setInput(sam);
  assertEqual(postV->execute(),NULL);//#4
  assertEqual(postV->batch.size(),(size_t)(4));
  createSample(sam);//different insances of sample are needed
  postV->setInput(sam);
  assertEqual((postV->execute())->feature.c_str(),"");// this function delete sam
  createSample(sam);
  assertEqual(postV->batch.size(),(size_t)(0));  
  delete postV;

  postV =new postVal("send(aa)", opts.thing, opts.device, opts.url,token,
                             "temperature", "average-temperature",10 );
  assertFalse(postV->valid); 
  delete postV;
  postV =new postVal("send(1aa)", opts.thing, opts.device, opts.url,token,
                             "temperature", "average-temperature",10 );
  assertFalse(postV->valid); 
  delete postV; 
  postV =new postVal("send(1a1)", opts.thing, opts.device, opts.url,token,
                             "temperature", "average-temperature",10 );
  assertFalse(postV->valid);  
  delete postV;    
  postV =new postVal("send(11)", opts.thing, opts.device, opts.url,token,
                             "temperature", "average-temperature",10 );
  assertFalse(postV->valid);  
  delete postV; 
  delete sam;     
}

testF(Settings,sample){
  Serial.println("SAMPLE");

  createSample(sam,19);
  assertEqual(sam->value,(float)19);
  assertEqual(sam->feature.c_str(),"temperature");
  delete sam;
}

testF(Settings,edgine){
  
  
  Serial.println("EDGINE");
  edgine* Edge=edgine::getInstance();
  opts.username="username1";//mocked username
  opts.password="password1";//mocked password
  Edge->init(opts);// authentication falis
  assertEqual(Edge->Api->getIssueBufferSize(),4);//4 Issue: token_expiration_time,period,cycle,retryTime

  //update with the correct login for other test
  opts.username="username";//correct username
  opts.password="password";//correct password

  Edge->init(opts);// in the mocked init we retrieve 2 scripts: "temperature().send(5)"" and 
                      // "temperature(10).window(+, 0, 60).map(a/60).send()"                    
  vector<sample*> samples;
  createSample(sam);
  samples.push_back(sam);//use this sample all the times
  for (int i = 0; i < 4; i++)
  { // for 4 times no data are sent to the API
    assertEqual(Edge->evaluate(samples), 0);//this function makes a copy of each sample so sam is never be deleted
  }
  assertEqual(Edge->Api->getIssueBufferSize(),4);
  assertEqual(Edge->evaluate(samples), 1); // 5th time the first script POST the datum
  assertEqual(Edge->Api->getIssueBufferSize(),0); //After POSTing a value the issue are POSTed too
  assertEqual(Edge->evaluate(samples), 0);  

  for(int i=0;i<samples.size();i++){
    delete samples[i];
  }
  samples.clear();
  delete Edge->Api;
  delete Edge;
  
  
}

testF(Settings,APIRest){
  
  
  Serial.println("APIREST");
  string response=Api->POSTLogin(opts.url+"/"+opts.ver+"/"+opts.login, opts.username, opts.password);
  assertEqual(response.c_str(), "200{\"token\": \"JWT token\"}");
  response=Api->POSTLogin(opts.url+"/"+opts.ver+"/"+opts.login, "aa","bb");
  assertEqual(response.c_str(), "none");

  response= Api->GETInfoUpdateDate(opts.url,token);
  assertEqual(response.c_str(),"200{"
                          "\"version\": \"0.2.001\","
                          "\"environment\": \"production\","
                          "\"token_expiration_time\": \"30m\","
                          "\"database\": \"mongodb://localhost:27017/atmosphere-prod\","
                          "\"timestamp\": \"1581425017114\""
                        "}");
  response= Api->GETInfoUpdateDate(opts.url,"token");
  assertEqual(response.c_str(),"none");

  response=Api->GETDescr(opts.url,token);
  assertEqual(response.c_str(),"200{"
        "\"features\": ["
          " \"temperature\""
        "],"
        "\"tags\": [],"
        "\"scripts\": ["
          "\"average-hourly-temperature\","
            "\"group-temperature\""
        "],"
        "\"visibility\": \"private\","
        "\"period\": \"5s\","
        "\"cycle\": \"10m\","
        "\"retryTime\": \"10s\","
        "\"scriptListMaxSize\": 5,"
        "\"measurementBufferSize\": 20,"
        "\"issueBufferSize\": 20,"
        "\"sendBufferSize\": 20,"
        "\"scriptStatementMaxSize\": 5,"
        "\"statementBufferSize\": 10,"
        "\"measurementBufferPolicy\": \"decimation\","
        "\"_id\": \"temperature-sensor-riccardo-office\","
        "\"owner\": {"
            "\"_id\": \"5dcec66bc67ed54963bc865c\","
            "\"username\": \"riccardo-office-temperature-sensor-username\","
            "\"type\": \"provider\""
        "}"
      "}");
  response=Api->GETDescr(opts.url,"JWT token1");
  assertEqual(response.c_str(),"200{"
        "\"features\": ["
          " \"temperature\""
        "],"
        "\"tags\": [],"
        "\"scripts\": ["
          "\"average-hourly-temperature\","
            "\"group-temperature\""
        "],"
        "\"visibility\": \"private\","
        "\"period\": \"5a\","
        "\"cycl\": \"10m\","
        "\"retryTime\": \"1d0s\","
        "\"scriptListMaxSize\": 5,"
        "\"measurementBufferSize\": 20,"
        "\"issueBufferSize\": 20,"
        "\"sendBufferSize\": 20,"
        "\"scriptStatementMaxSize\": 5,"
        "\"statementBufferSize\": 10,"
        "\"measurementBufferPolicy\": \"decimation\","
        "\"_id\": \"temperature-sensor-riccardo-office\","
        "\"owner\": {"
            "\"_id\": \"5dcec66bc67ed54963bc865c\","
            "\"username\": \"riccardo-office-temperature-sensor-username\","
            "\"type\": \"provider\""
        "}"
      "}");
  response=Api->GETDescr(opts.url,"token");
  assertEqual(response.c_str(),"none");

  response=Api->GETScript(opts.url+"/v1/scripts?filter={\"_id\":\"group-temperature\"}",token);
  assertEqual(response.c_str(),"200{\"docs\": ["
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
  response=Api->GETScript(opts.url+"/v1/scripts?filter={\"_id\":\"average-hourly-temperature\"}",token);
  assertEqual(response.c_str(),"200{\"docs\": ["
					  "{"
						  "\"visibility\": \"private\","
						  "\"tags\": [],"
						  "\"_id\": \"average-hourly-temperature\","
						  "\"code\": \"temperature(60).window(+, 0, 10).map(a/100).send()\","
						  "\"owner\": {"
							  "\"_id\": \"5dcec66bc67ed54963bc865c\","
							  "\"username\": \"riccardo-office-temperature-sensor-username\","
							  "\"type\": \"provider\"}"
						  "}"
					  "],"
					"\"totalDocs\": 1, \"limit\": 10, \"hasPrevPage\": false, \"hasNextPage\": false, \"page\": 1, \"totalPages\": 1, \"pagingCounter\": 1, \"prevPage\": null, \"nextPage\": null}");
    
  response=Api->GETScript(opts.url,"token");
  assertEqual(response.c_str(),"none");
  createSample(sam);
  boolean success= Api->POSTMeasurement(*sam,token);
  assertTrue(success);
  assertEqual(Api->getSampleBufferSize(),0);
  success= Api->POSTMeasurement(*sam,"token");
  assertFalse(success);
  assertEqual(Api->getSampleBufferSize(),1);
  for (int i = 1; i < 20; i++)
  {
    success= Api->POSTMeasurement(*sam,"token");
    assertFalse(success);
    assertEqual(Api->getSampleBufferSize(),1+i);
  }
  success= Api->POSTMeasurement(*sam,"token");// sample #21 has to be added to Buffer 
  assertFalse(success);
  assertEqual(Api->getSampleBufferSize(),11);//halve samples when overflow the BufferSize (20) and only the add the new sample
  
  success= Api->POSTMeasurement(*sam,token);//now the POST works again
  assertTrue(success);
  assertEqual(Api->getSampleBufferSize(),0);// empty the buffer when POST a new sample

  for (int i = 1; i <= 20; i++)
  {
    success= Api->POSTMeasurement(*sam,"token");
    assertFalse(success);
    assertEqual(Api->getSampleBufferSize(),i);
  }
  Api->setSampleBufferSize(30);
  assertEqual(Api->getSampleBufferSize(),20);
  Api->setSampleBufferSize(15);
  assertEqual(Api->getSampleBufferSize(),15);
  success= Api->POSTMeasurement(*sam,"token");
  assertFalse(success);
  assertEqual(Api->getSampleBufferSize(),9);
  success= Api->POSTMeasurement(*sam,token);//now the POST works again
  assertTrue(success);
  assertEqual(Api->getSampleBufferSize(),0);// empty the buffer when POST a new sample
  Api->setSampleBufferSize(20);
  assertEqual(Api->getSampleBufferSize(),0);

  success=Api->POSTIssue(opts.url,token,opts.thing,
                          "temperature",opts.device,"group-temperature");
  assertTrue(success);
  assertEqual(Api->getIssueBufferSize(),0);
  success=Api->POSTIssue(opts.url,"token",opts.thing,
                          "temperature",opts.device,"group-temperature");
  assertFalse(success);
  assertEqual(Api->getIssueBufferSize(),1);
  
  for (int i = 1; i < 20; i++)
  {
    success= Api->POSTIssue(opts.url,"token",opts.thing,
                          "temperature",opts.device,"group-temperature");
    assertFalse(success);
    assertEqual(Api->getIssueBufferSize(),1+i);
  }
  success= Api->POSTIssue(opts.url,"token",opts.thing,
                          "temperature",opts.device,"group-temperature");
  assertFalse(success);
  assertEqual(Api->getIssueBufferSize(),11);
  
  success= Api->POSTMeasurement(*sam,token);
  assertTrue(success);
  assertEqual(Api->getIssueBufferSize(),0);
  for (int i = 1; i <= 20; i++)
  {
    success= Api->POSTIssue(opts.url,"token",opts.thing,
                          "temperature",opts.device,"group-temperature");
    assertFalse(success);
    assertEqual(Api->getIssueBufferSize(),i);
  }
  success= Api->POSTMeasurement(*sam,"token");
  assertFalse(success);
  assertEqual(Api->getIssueBufferSize(),20);
  success= Api->POSTMeasurement(*sam,token);
  assertTrue(success);
  assertEqual(Api->getIssueBufferSize(),0);
  
  for (int i = 1; i <= 20; i++)
  {
    success= Api->POSTIssue(opts.url,"token",opts.thing,
                          "temperature",opts.device,"group-temperature");
    assertFalse(success);
    assertEqual(Api->getIssueBufferSize(),i);
  }
  Api->setIssueBufferSize(30);
  assertEqual(Api->getIssueBufferSize(),20);
  Api->setIssueBufferSize(15);
  assertEqual(Api->getIssueBufferSize(),15);
  success= Api->POSTMeasurement(*sam,"token");
  assertFalse(success);
  assertEqual(Api->getSampleBufferSize(),1);
  success= Api->POSTIssue(opts.url,"token",opts.thing,
                          "temperature",opts.device,"group-temperature");
  assertFalse(success);
  assertEqual(Api->getIssueBufferSize(),9);
  success= Api->POSTMeasurement(*sam,token);//now the POST works again
  assertTrue(success);
  assertEqual(Api->getSampleBufferSize(),0);// empty the buffer when POST a new sample
  Api->setIssueBufferSize(20);
  assertEqual(Api->getIssueBufferSize(),0);
  delete sam;
  
  
}

testF(Settings, connection){
  
  
  Serial.println("CONNECTION");
  assertFalse(conn->isConnected());
  conn->setupConnection(ssidWIFI,passWIFI);
  assertTrue(conn->isConnected());
  delete conn;
  
  
}

void setup() {
  delay(1000); // wait for stability on some boards to prevent garbage Serial
  Serial.begin(115200);
  while (!Serial); 
}

void loop() {
  TestRunner::run();
}
