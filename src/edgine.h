//edgine.h

#ifndef edgine_h
#define edgine_h

typedef struct{
  String username;
  String password;
  //route
  String url;
  String ver;
  String login;
  String devs;
  String scps;
  String measurements;
  String dateUrl;
  
  String thing;
  String device;
  String id;
}options;

typedef struct {
  double value;
  String date;
  String url;
  String thing;
  String feature;
  String device;
  String scriptId;
  // other info about the measurement
}measureData;

#include "APIRest.h" //API wrapper
#include "script.h"
#include "operation.h"
#include "sample.h"
#define MILLIS_PER_SEC 1000


//SINGLETON
class edgine{
  private:
  //constructor
  edgine();
  
  //variables
  static edgine* instance;
  //typedef struct options opts;
  options opts;
  
  String token;
  String response;
  vector<script> scripts;
  String scriptsId; // id of the scripts
  String features;
  double startLogCount=0;// starting instant of counter of Login
  double startGetCount=0;// starting instant of counter of Get scripts
  
  int tokenDuration=1800;//30 minutes= 1800 seconds; interval between two login
  int cycle=60;//1 minutes; interval between two GET script
  double period= 5;//interval between two loop execution: max speed of loop
  
  double logCount=(double)tokenDuration;
  double getCount=(double)cycle;

  int i,j;
  int startIndex;
  int endIndex;
  String tempCode;
  String scriptId;
  int beginOfValue;
  int endOfValue;
  int nextClose;
  int nextOpen;
  String delimiters="";
  int samplesSent=0;
  
  //methods
  void retrieveScriptsCode(String, String);
  int executeScripts(vector<sample>);
  String ParseResponse( String, String);
  int FindEndIndex (char, char, int, String);
  String ParseToken(String);
  void setToken(String);
  
  public:
  //variables
  APIRest* Api; //Wrapper for the Rest API
  
  //methods
  static edgine* getInstance();
  void init(options);
  int evaluate(vector<sample>);

  //getters
  double getPeriod();
};

edgine* edgine::instance=NULL;

edgine* edgine::getInstance(){
  if(instance==NULL){
    instance= new edgine();
  }
  return instance;
}

edgine::edgine(){
  Api=APIRest::getInstance();  
}

void edgine::init( options opts){

  this->opts=opts; 
  startLogCount = millis();      
  response=Api->POSTLogin(opts.url+"/"+opts.ver+"/"+opts.login, opts.username, opts.password); // Authentication
  token = ParseToken(response);
  ///////////////  WHAT IF THE LOGIN FAILS????????  ////////////////////////////

  ///////////////  WHAT IF THE GETDATE FAILS????????  ////////////////////////////
  //Api->GETDate(opts.dateUrl,token);
  
  startGetCount = millis();
  response = Api->GETDescr(opts.url+"/"+opts.ver+"/"+opts.devs+"/"+opts.id, token); // Get the scripts
  //response = GETDescr(token); // Get the virtual description
  //cycle = ParseResponse(response,"cycle")!="" ? ParseResponse(response,"cycle").toInt():600; // default 10 minutes
  scriptsId = ParseResponse(response,"scripts");
  features= ParseResponse(response,"features");
  retrieveScriptsCode(token, scriptsId);
}

int edgine::evaluate(vector<sample> samples){
  
  //Check if we have to update scripts
  getCount = (double)( millis()-startGetCount )/MILLIS_PER_SEC;
  if( getCount >= cycle ){
    startGetCount = millis(); 
    response = Api->GETDescr(opts.url+"/"+opts.ver+"/"+opts.devs+"/"+opts.id, token); // Get the scripts
    
    //if(response!="none") 
    /////////////////////  COSA FARE SE LA RICHIESTA DELLO DESCRIZIONE FALLISCE  /////////////////////////////////////////////////////////////////////////////////////////////////
    
    //cycle = ParseResponse(response,"cycle")!="" ? ParseResponse(response,"cycle").toInt():600; //default 10 minutes
    scriptsId = ParseResponse(response,"scripts");
    features= ParseResponse(response,"features");
    retrieveScriptsCode(token, scriptsId);
  }
  //Check if we have to request a new TOKEN
  logCount = (double)( millis()-startLogCount )/MILLIS_PER_SEC;      
  if( logCount >= tokenDuration ){ //every "tokenDuration" authenticate
    startLogCount = millis();
    response=Api->POSTLogin(opts.url+"/"+opts.ver+"/"+opts.login, opts.username, opts.password); // Authentication
    token= ParseToken(response);
    //token = POSTLogin(opts.username, opts.password); // Authentication    
    
    //if(token!="none") 
    /////////////////////  COSA FARE SE LA RICHIESTA DELLO DESCRIZIONE FALLISCE  /////////////////////////////////////////////////////////////////////////////////////////////////
    
    setToken(token); // Update the token in each script
  }  
 
  return executeScripts(samples);
}

void edgine::retrieveScriptsCode(String token, String scriptsId){
  startIndex=1;
  endIndex=1;

  //Serial.println("retrieveScriptsCode :"+scriptsId);
  scriptsId.replace(" ","");//delete whitespace
  
  while( startIndex < scriptsId.length() ){
    
    endIndex=scriptsId.indexOf("\"",startIndex+1); // start the search from the next charater
    
    scriptId= scriptsId.substring(startIndex,endIndex);

    String code = Api->GETScript(opts.url + "/" + opts.ver + "/" + opts.scps + "?filter={\"_id\":\"" + scriptId + "\"}", token);
    //if (code!="none")
    //////////////////////////// COSA FARE SE FALLISCE LA RICHIESTA DEL CODICE DI UNO SCRIPT ///////////////////////////////////////////////////////////////////////////////////
    
    tempCode=ParseResponse( code,"code");
    
    //verify if it is a new script
    for(i=0;i<scripts.size();i++){
      if(scripts[i].scriptId==scriptId && scripts[i].scriptStr==tempCode ){ //if there is already this script
        Serial.println("Script Unchanged: "+scriptId);
        scripts[i].valid=true; // set it to valid because it is already in the API
        goto cnt; //is already present so do nothing an pass to retrieve next script
      }
      else if(scripts[i].scriptId==scriptId){ //if there is already this script but the code has changed
        Serial.println("Script changed: "+scriptId);
        scripts[i].valid=false;// delete the old version of the script and then create the new version of it  
        //scripts.erase(scripts.begin()+i); // delete the old version of the script and then create the new version of it  
      }
    }    
    
    //create the script
    scripts.push_back( script(scriptId, tempCode, opts.thing, opts.device, opts.url+"/"+opts.ver+"/"+opts.measurements, token, features) );
    if(scripts.back().valid==false){//if the creation of the script has failed      
      scripts.pop_back();//remove last script
    }
    
    cnt:; //go directly there if a script already exists and it is not changed
    
    startIndex=endIndex+3;//+3 because we want to avoid: "," characters between two scripts id
  }
  
  //  delete scripts that are not valid anymore (deleted)
  for(i=0;i<scripts.size();i++){
    if(scripts[i].valid==false){
      Serial.println("Script deleted: "+scripts[i].scriptId);
      scripts.erase(scripts.begin()+i);
      i--; //since we deleted a script even if the scripts size stays unchanged every position is shifted by one
    }
    else{
      Serial.println("Script valid: "+scripts[i].scriptId);
      scripts[i].valid=false;// preset valid to false, to be reconfirmed on the next check in the API
    }
  }
  Serial.println("There are: "+String(scripts.size())+" scripts");
}


int edgine::executeScripts(vector<sample> samples){
	samplesSent=0;
	for(j=0;j<samples.size();j++){
		for(i=0;i<scripts.size();i++){
			if(scripts[i].feature==samples[j].feature){
				if( scripts[i].execute( samples[j].getValue() ) )
					samplesSent++;
			}
		}
	}
    return samplesSent;
}

void edgine::setToken(String token){
  for(i=0;i<scripts.size();i++){
    scripts[i].setToken(token);
  }
}


String edgine::ParseToken(String token){
  return token.substring( token.indexOf("J"), token.lastIndexOf("\"") );
}

//NOT WORKS WITH CUSTOM OPERATIONS BECAUSE IT DELETES ANY WHITESPACES
String edgine::ParseResponse( String response, String fieldName ){
  
  if( response.indexOf(fieldName) ==-1){
    Serial.println(fieldName+" field is not present!");
    return "";
  }
  response.replace(" ","");//delete whitespace
  beginOfValue = response.indexOf( ":", response.indexOf(fieldName) )+1;//find starting index of field value
  
  switch ( response.charAt( beginOfValue ) ){
    case '\"':
      endOfValue = response.indexOf('\"',beginOfValue+1);// start looking for the last delimiter from the next value
      break;
    case '(':
      endOfValue = FindEndIndex('(',')', beginOfValue+1,response);
      break;
    case '[':
      endOfValue = FindEndIndex('[',']', beginOfValue+1,response);
      break;
    case '{':
      endOfValue = FindEndIndex('{','}', beginOfValue+1,response);
      break;
  }
    
  return response.substring( beginOfValue+1, endOfValue);
}


int edgine::FindEndIndex (char first,char last, int start, String response){
  while(true){
    nextOpen = response.indexOf( first, start );
    nextClose = response.indexOf( last, start );
    //if no more open/close bracket is found it returns -1 
    if(nextOpen==-1)
      nextOpen=nextClose+1;
    if(nextClose==-1)
      nextClose=nextOpen+1;
      
    //if the next one is a close bracket and no bracket in the stack
    if ( nextClose<nextOpen && delimiters.length()==0 ){
       break;
    }
      //if the next one is a close bracket and an open bracket is at the end of the stack  delimiters.charAt(delimiters.length()-1)==first){
    else if( nextClose<nextOpen && delimiters.length()!=0){
       delimiters.remove(delimiters.length()-1);
       start=nextClose+1;
    }
    //if the next one is a open bracket
    else{
      delimiters.concat(first);
      start=nextOpen+1;
    }
  }
  return nextClose;
}

double edgine::getPeriod(){
  return period;
}
#endif 
