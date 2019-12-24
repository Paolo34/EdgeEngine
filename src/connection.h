//connection.h

#ifndef connection_h
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
  boolean TESTING;
  boolean connected;

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
  TESTING=false;
  connected=false;
}

void connection::setupConnection(const char* ssidWifi,const char* passWifi){
  if(!TESTING){
    delay(1000);
    WiFi.begin(ssidWifi, passWifi); 
    while (WiFi.status() != WL_CONNECTED) {
      delay(2000);
      Serial.println("Connecting to WiFi..");
    }
    Serial.println("Connected to the WiFi network");
  }
  else{
	  connected=true;
  }
  
}

boolean connection::isConnected(){
	if(!TESTING){
		return (WiFi.status() == WL_CONNECTED);
	}
	else{
		return connected;
	}
}

#endif
