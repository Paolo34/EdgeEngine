/*
  connection.cpp - Class for managing WiFi connection.
  Created by Francesco Tornatore, February 24, 2020.
  Released into the public domain.
*/

#include "connection.h"

#include <Arduino.h>
#include <esp_wifi.h>
#include <WiFi.h>


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
  wifi_config_t current_conf;
  esp_wifi_get_config(WIFI_IF_STA, &current_conf);
  current_conf.sta.listen_interval = (uint16_t)65535;// max uint16
  esp_wifi_set_config(WIFI_IF_STA, &current_conf);

  esp_wifi_set_ps(WIFI_PS_MAX_MODEM);// set Power Saving Mode without shutdown WIFI
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


bool connection::isConnected(){
	if(!TESTING){
		return (WiFi.status() == WL_CONNECTED);
	}
	else{
		return connected;
	}
}
bool connection::isConnectionLost(){
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