using std::vector;

#include "src/connection.h"
#include "src/edgine.h"
#include "src/sample.h"



int pirPin = 35;
int lightPin = 34;// pin 27 not working with this sketch
int potPin = 32;

double pirCounter;
sample motion = sample("temperature");

const char* ssidWifi = "TIM-91746045";
const char* passWifi = "1Oj3eyR5qHD3jAaT5Jfj1Ooh";
/*
const char* ssidWifi = "TORNATOREwifi";
const char* passWifi = "finalborgo";
*/
/*
const char* ssid = "S7Chicco";
const char* password = "LLLLLLLL";
*/
options opts;

edgine* Edge;
connection* Connection; //Wrapper for the wifi connection
vector<sample> samples;
/*
const char* root_ca= \
"-----BEGIN CERTIFICATE-----\n" \
"MIIDJTCCAg2gAwIBAgIJAL2SSUeiac7lMA0GCSqGSIb3DQEBCwUAMGAxLTArBgNV\n" \
"BAMMJEJpdGRlZmVuZGVyIFBlcnNvbmFsIENBLmF2ZnJlZTAwMDAwMDEMMAoGA1UE\n" \
"CwwDSURTMRQwEgYDVQQKDAtCaXRkZWZlbmRlcjELMAkGA1UEBhMCVVMwHhcNMTkw\n" \
"OTA0MTQzOTE4WhcNMTkxMjAzMTQzOTE4WjAfMR0wGwYDVQQDExRsM3AuYXRtb3Nw\n" \
"aGVyZS50b29sczCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAKIv/oZH\n" \
"GYvosa9Hpga7V9G065/+MzgKKhOqJXkEF1YTkyQ7l395RyHHm6qYxRTunh405/il\n" \
"KrtZIcX3wBfDYjPuvBk28KME17zlWfW1ZZfFoJjjrx1yeOD6lvDNNZgnt1GRZCRX\n" \
"JcrSQR67ihcvn6Qp3ItdxzItU5Xoet9+ARiyjcF1qOomBPsrm21zXeclbhGAFJqT\n" \
"CyDMWIahZByNN6deurTX2+CGGj0z89mujwfT0MtK8LQxaZ6FrmMXC6q+dcLIITuc\n" \
"jFPRqOqqIPK7p3nLn9T9U6/BKRmrN8tMDiYDj6ho6At5vAVpFJRjvrcrJWBCSF4o\n" \
"kGHcE6h/6nabUUsCAwEAAaMjMCEwHwYDVR0RBBgwFoIUbDNwLmF0bW9zcGhlcmUu\n" \
"dG9vbHMwDQYJKoZIhvcNAQELBQADggEBAAtQsteDHMM0aWgAj2KpjTL0h2Aq9DQw\n" \
"7RgFOOBscmDJBOJr8/Kg7SRnLuX0Xgngo9hLkWRDEm4Z8w+qtXWYBESgeKIGG8wM\n" \
"OE+uiZG2W6+sMijUVFq1ZO8ZEqlmt8aF/TF3syPNiZiJWzhqNnO+Ihm7a+xBGe2d\n" \
"Nd89cDp3n1fFG3Go0hB25CkObVMRwisDlZ/J9OEONoS+SVmzzOmiXD38ifphP1EZ\n" \
"QUdm499dycwPutvUXeO0aqfNE7ONA/GIeDy+XKeake6SlqsJWLko2oBSy8HDZu1j\n" \
"66BLarM52c/65MkI8e9XnjXpeSGQ4x6+4qKpapQyj3xOLKqVEYnEZbM=\n" \
"-----END CERTIFICATE-----\n"; //Atmosphere CA
*/

/**
 * setup
 */
void setup() {
  
  Serial.begin(115200);

  //setup connection
  Connection = connection::getInstance();
  Connection->setupConnection(ssidWifi, passWifi);
  
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
  opts.dateUrl= "URL OF THE DATE";
  //Edgine identifiers
  opts.thing = "riccardo-office";
  opts.device = "temperature-sensor-riccardo-office";
  opts.id = "temperature-sensor-riccardo-office";

  //initialize Edge engine
  Edge=edgine::getInstance();
  Edge->init(opts);
  
  //Interrupt sensor setup
  pinMode(pirPin, INPUT);
  //attachInterrupt(digitalPinToInterrupt(pirPin), detectedMotion, FALLING);
  
  
}



void loop() {
  
  if ( Connection->isConnected() ) //Check the current connection status
  { 
    /*
    //create a temperature measurement sample
    sample temperature = sample("temperature");
    float t = dht.readTemperature();
    Serial.println(t);
    temperature.setValue(t);
    samples.push_back(temperature);

    //create a humidity measurement sample
    sample humidity = sample("humidity");
    
    humidity.setValue(1);
    samples.push_back(humidity);
    */
   
    //create a light measurement sample
    sample light = sample("temperature");
    int lig=analogRead(lightPin);
    //Serial.println(lig);
    light.setValue( lig );
   // samples.push_back(light);
    
    //create a potentiometer measurement sample
    sample potentiometer = sample("temperature");
    int pot=analogRead(potPin);
    potentiometer.setValue( pot );
    samples.push_back(potentiometer);

    Edge->evaluate(samples);
    samples.clear(); // after evaluated all samples delete them
    
    if((millis()-pirCounter)>=2000){// pir sensor needs 2 seconds to be ready to give another measurement
      //attachInterrupt(digitalPinToInterrupt(pirPin), detectedMotion, FALLING);
      //Serial.println("attachInterrupt");
    }
      
    delay(Edge->getPeriod()*1000);//delay in milliseconds
    
  }
  else{
    Serial.println("Device disconnected");
  }
  
}

void detectedMotion(){
  detachInterrupt(digitalPinToInterrupt(pirPin)); //PIR sensor needs 2 seconds to take an image to compare to
  pirCounter=millis();
  Serial.println("Motion detected");
  motion.setValue(1);
  samples.push_back(motion);
}
