using std::vector;
using std::string;
#include <time.h>
#include <EdgeEngine_library.h>

#include <SparkFunTSL2561.h>
#include <Wire.h>

#define I2C_SDA 33
#define I2C_SCL 32

int pirPin = 35;
int lightPin = 34;// pin 27 not working with this sketch
int potPin = 32;

// Create an SFE_TSL2561 object, here called "light":
SFE_TSL2561 lightSensor;
bool lightSensorGain;
unsigned int lightSensorms;

clock_t pirCounter;
clock_t cycleCounter; // count execution cycle time
clock_t sleepTime;

sample* motion=NULL;
sample* temperature=NULL;
sample* light=NULL;
/*
const char* ssidWifi = "TIM-91746045";
const char* passWifi = "1Oj3eyR5qHD3jAaT5Jfj1Ooh";
*/
const char* ssidWifi = "TORNATOREwifi";
const char* passWifi = "finalborgo";

edgine* Edge;
connection* Connection; //Wrapper for the wifi connection
vector<sample*> samples;


/**
 * setup
 */
void setup() {
  
  Serial.begin(115200);

  //setup I2C
  Wire.begin(I2C_SDA, I2C_SCL);

  //setup sensors
  lightSensor.begin();//Can pass nothing to light.begin(), default I2C address (0x39), or use one presets if you have changed the ADDR jumper on the board
  lightSensorGain=0;
  lightSensor.setTiming(lightSensorGain,2,lightSensorms);
  lightSensor.setPowerUp();

  //setup connection
  Connection = connection::getInstance();
  Connection->setupConnection(ssidWifi, passWifi);

  options opts;
  //login
  opts.username = "riccardo-office-temperature-sensor-username";
  opts.password =  "riccardo-office-temperature-sensor-password";
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
  opts.device = "environment-riccardo-office";
  opts.id = "environment-riccardo-office";
  
  //initialize Edge engine
  Edge=edgine::getInstance();
  Edge->init(opts);
  
  //Interrupt sensor setup
  pinMode(pirPin, INPUT);
  //attachInterrupt(digitalPinToInterrupt(pirPin), detectedMotion, FALLING);
  
}

void loop() {
  cycleCounter=clock();
  
  //create a light measurement sample
  light = new sample("light");
  light->startDate=Edge->Api->getActualDate();
  light->endDate=light->startDate;
  light->value=getLux();
  samples.push_back(light);
  
  //create a temperature measurement sample
  temperature = new sample("temperature");
  temperature->startDate=Edge->Api->getActualDate();
  temperature->endDate=temperature->startDate;
  temperature->value=0;
  samples.push_back(temperature);

  Edge->evaluate(samples);
  samples.clear(); // after evaluated all samples delete them

  delete temperature;
  temperature=NULL;
  delete light;
  light =NULL;
  //delete motion;

  // if( ((double)clock()-pirCounter)>=2000){// pir sensor needs 2 seconds to be ready to give another measurement
  //   //attachInterrupt(digitalPinToInterrupt(pirPin), detectedMotion, FALLING);
  //   //Serial.println("attachInterrupt");
  // }
  if (!Connection->isConnected()) {
    Serial.println("Device disconnected");
    Serial.print("WIFI STATUS: ");
    Serial.println(WiFi.status());
    Connection->reconnect();
  }

  cycleCounter=clock()-cycleCounter;// duration of the exexution of th cycle
  
  // subtract te execution time to the Sleep period if result is not negative
  ((double)cycleCounter/CLOCKS_PER_SEC) < Edge->getPeriod() ? sleepTime=(Edge->getPeriod()-(double)cycleCounter/CLOCKS_PER_SEC)*1000 : sleepTime=0;//delay in milliseconds
  

  delay(sleepTime);
}

// void detectedMotion(){
//   detachInterrupt(digitalPinToInterrupt(pirPin)); //PIR sensor needs 2 seconds to take an image to compare to
//   pirCounter=(double)clock();
//   Serial.println("Motion detected");
//   motion = new sample("motion");
//   motion->startDate=Edge->Api->getActualDate();
//   motion->endDate=motion->startDate;
//   motion->value=1;
//   samples.push_back(motion);
// }

double getLux(){
  unsigned int data0, data1;
  if (lightSensor.getData(data0,data1))
  {
    double lux;// Resulting lux value
    if (!lightSensor.getLux(lightSensorGain,lightSensorms,data0,data1,lux)) 
    { 
      //sensor saturated
    }
    return lux;
  }
  else{
     //I2C error
  }
  
}


