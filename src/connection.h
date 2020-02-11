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
  void disconnect();
  void reconnect();
  int status();
  boolean isConnected();
  boolean isConnectionLost();

    
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
      Serial.println(F("Connecting to WiFi.."));
    }
    Serial.println(F("Connected to the WiFi network"));
  }
  else{
	  connected=true;
  }
}

void connection::disconnect(){
  if(!TESTING){
    Serial.println(F("Disconnecting.."));
    WiFi.disconnect(); 
    
    if(WiFi.status() == WL_DISCONNECTED)
      Serial.println(F("Disconnected"));
  }
  else{
	  connected=false;
  }
}

void connection::reconnect(){
  if(!TESTING){
    Serial.println(F("Reconnecting.."));
    WiFi.reconnect();

    if(WiFi.status() == WL_CONNECTED)
      Serial.println(F("Reconnected"));
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
boolean connection::isConnectionLost(){
  if(!TESTING){
    return (WiFi.status() == WL_CONNECTION_LOST);
  }
  else{
	  return false;
  }
}
int connection::status(){
	if(!TESTING){
		return WiFi.status();
	}
	else{
		return 3;//connected
	}
}
#endif
