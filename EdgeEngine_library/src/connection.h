/*
  connection.h - Class for managing WiFi connection.
  Created by Francesco Tornatore, February 24, 2020.
  Released into the public domain.
*/

#ifndef connection_h
#define connection_h
#include <string>
using std::string;

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
  bool TESTING;
  bool connected;

  //methods
  static connection* getInstance();
  void setupConnection(const char*,const char*);
  void disconnect();
  void reconnect();
  int status();
  bool isConnected();
  bool isConnectionLost();
    
};

#endif
