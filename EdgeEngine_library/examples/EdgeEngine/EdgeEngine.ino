using std::vector;
using std::string;
#include <time.h>
#include <EdgeEngine_library.h>

#include <SparkFunTSL2561.h>
#include <Wire.h>
//I2C wires
#define I2C_SDA 33
#define I2C_SCL 32

uint8_t pirPin = 35;

int tmpSensorAddress = 0x48;

// Create an SFE_TSL2561 object
SFE_TSL2561 lightSensor;
bool lightSensorGain;
unsigned int lightSensorms;

clock_t pirCounter;
clock_t cycleCounter; // count execution cycle time
clock_t sleepTime;

sample* motion=NULL;
sample* temperature=NULL;
sample* light=NULL;

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
  attachInterrupt(digitalPinToInterrupt(pirPin), detectedMotion, FALLING);
  
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
  temperature->value=getTemperature();
  samples.push_back(temperature);

  Edge->evaluate(samples);

  samples.clear(); // after evaluated all samples delete them

  delete temperature;
  temperature=NULL;
  delete light;
  light =NULL;  
  
  if( ((float)clock()-pirCounter)>=2000){// pir sensor needs 2 seconds to be ready to give another measurement
    if(!motion){
      delete motion;
      motion=NULL;
    }
    attachInterrupt(digitalPinToInterrupt(pirPin), detectedMotion, FALLING);
  }
 

  cycleCounter=clock()-cycleCounter;// duration of the exexution of th cycle
  
  // subtract te execution time to the Sleep period if result is not negative
  ((float)cycleCounter/CLOCKS_PER_SEC) < Edge->getPeriod() ? sleepTime=(Edge->getPeriod()-(float)cycleCounter/CLOCKS_PER_SEC)*1000 : sleepTime=0;//delay in milliseconds
  

  delay(sleepTime);
}

void detectedMotion(){
  detachInterrupt(digitalPinToInterrupt(pirPin)); //PIR sensor needs 2 seconds to take an image to compare to
  pirCounter=clock();
  motion = new sample("motion");
  //motion->startDate=Edge->Api->getActualDate();
  //motion->endDate=motion->startDate;
  motion->value=1;
  samples.push_back(motion);
}

float getLux(){
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

float getTemperature(){
  Wire.requestFrom(tmpSensorAddress,2); //size: 2 bytes 
  byte MSB = Wire.read();
  byte LSB = Wire.read();
  int TemperatureSum = ((MSB << 8) | LSB) >> 4; //12 bit int, two's complement for negative
  return TemperatureSum*0.0625;//quantization level is 0.0625 celsius
}


