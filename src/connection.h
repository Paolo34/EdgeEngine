//connection.h

#ifndef conncetion_h
#define connection_h

#include <WiFi.h>

//SINGLETON wrapper
class connection{
  private:
  //constructor
  connection();
  
  //variables
  static connection* instance;
  
  //methods
  
  public:
  //variables
  
  //methods
  static connection* getInstance();
  void setupConnection(const char*,const char*);
  boolean isConnected();
    
};

connection* connection::instance=NULL;

connection* connection::getInstance(){
  if(instance==NULL){
    instance= new connection();
  }
  return instance;
}

connection::connection(){
}

void connection::setupConnection(const char* ssidWifi,const char* passWifi){
  delay(1000);
  WiFi.begin(ssidWifi, passWifi); 
  while (WiFi.status() != WL_CONNECTED) {
    delay(2000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
}

boolean connection::isConnected(){
  return (WiFi.status() == WL_CONNECTED);
}

#endif
