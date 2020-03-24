/*
  edgine.h - Class for create an instance of Edge Engine.
  Created by Francesco Tornatore, February 24, 2020.
  Released into the public domain.
*/
#ifndef edgine_h
#define edgine_h


#include <vector>
using std::vector;
#include <string>
using std::string;
#include "APIRest.h" // API wrapper
#include "connection.h" // connection wrapper
#include "script.h"
#include "operation.h"
#include "sample.h"
#include <time.h>
#define MILLIS_PER_SEC 1000

typedef struct{
  string username;
  string password;
  //route
  string url;
  string ver;
  string login;
  string devs;
  string scps;
  string measurements;
  string info;
  string issues;
  //descriptors
  string thing;
  string device;
  string id;
}options;

//SINGLETON
class edgine{
  private:
  //constructor
  edgine();
  
  //variables
  static edgine* instance;
  options opts;
  bool initialization;
  bool WiFiconnected;
  
  string token="none";
  string response;
  vector<script*> scripts;
  string scriptsId; // id of the scripts
  string features;
  float startLogCount=0;// starting instant of counter of Login
  float startGetDescrCount=0;// starting instant of counter of Get description
  float startGetCount=0;// starting instant of counter of Get scripts/date
  
  int token_expiration_time=1800;//30 minutes= 1800 seconds; interval between two login
  int cycle=10;//10 minutes; interval between two GET script
  int period= 2;//5 seconds; interval between two loop execution: max speed of loop
  int retryTime=10; //retry every 10 seconds if the login or the first GETDescr/GETScript/GETDate fails

  int scriptListMaxSize= 5; // max num of scripts in the engine 
  int measurementBufferSize= 20; // max num of measurements saved if their POST falis
  int issueBufferSize= 20; // max num of issues saved if their POST falis
  int sendBufferSize= 20; // max num of measurements in a batch 
  int scriptStatementMaxSize= 5; // max num of operations per script
  int statementBufferSize= 10; // max num of sample involved in one operation
  string measurementBufferPolicy= "newest"; //policy of deletion of measurements whose POST failed 
  string policies="newest,decimation,average";

  float logCount=(float)token_expiration_time;
  float getDescrCount=(float)cycle;
  
  int temp;
  string tempPol;
  //string tempCode;
  string codeResponse;
  string scriptId;
  string firstGetScriptsResponse;
  
  //methods
  void authenticate();
  void retrieveScriptsCode(string, string);
  int executeScripts(vector<sample*>);
  string parseResponse( string, string,bool= true);
  void checkFields();
  int findEndIndex (char, char, int, string);
  int stringToSec(string);
  bool isaDigit(string);
  bool isOKresponse(string);
  string parseToken(string);
  void setToken(string);
  bool isAllowed(string,string);
  void deleteSpaces(string&);
  
  public:
  //variables
  APIRest* Api; //Wrapper for the Rest API
  connection* conn;
  
  //methods
  static edgine* getInstance();
  void init(options);
  int evaluate(vector<sample*>);

  //getters
  int getPeriod();
};

#endif 
