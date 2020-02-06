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
  String info;
  String alerts;
  //descriptors
  String thing;
  String device;
  String id;
}options;

#include "APIRest.h" //API wrapper
#include "script.h"
#include "operation.h"
#include "sample.h"
#include <time.h>
#define MILLIS_PER_SEC 1000


//SINGLETON
class edgine{
  private:
  //constructor
  edgine();
  
  //variables
  static edgine* instance;
  options opts;
  boolean initialization;
  
  String token="none";
  String response;
  vector<script*> scripts;
  String scriptsId; // id of the scripts
  String features;
  double startLogCount=0;// starting instant of counter of Login
  double startGetDescrCount=0;// starting instant of counter of Get description
  double startGetCount=0;// starting instant of counter of Get scripts/date
  
  int token_expiration_time=1800;//30 minutes= 1800 seconds; interval between two login
  int cycle=10;//10 minutes; interval between two GET script
  int period= 2;//5 seconds; interval between two loop execution: max speed of loop
  int retryTime=10; //retry every 10 seconds if the login or the first GETDescr/GETScript/GETDate fails

  double logCount=(double)token_expiration_time;
  double getDescrCount=(double)cycle;
  
  int temp;
  String tempCode;
  String codeResponse;
  String scriptId;
  String firstGetScriptsResponse;
  int samplesSent=0;
  
  //methods
  void authenticate();
  void retrieveScriptsCode(String, String);
  int executeScripts(vector<sample*>);
  String ParseResponse( String, String);
  int FindEndIndex (char, char, int, String);
  int stringToSec(String);
  boolean isaDigit(String);
  boolean isOKresponse(String);
  String ParseToken(String);
  void setToken(String);
  
  public:
  //variables
  APIRest* Api; //Wrapper for the Rest API
  
  //methods
  static edgine* getInstance();
  void init(options);
  int evaluate(vector<sample*>);

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
  initialization=true;
  this->opts=opts; 
  authenticate();

  do{//GET INFO
    if( ( ((double)clock() / CLOCKS_PER_SEC) - startLogCount ) >= token_expiration_time ){//verify token validity
      authenticate();
    }
    startGetCount = (double)clock() / CLOCKS_PER_SEC;
    response = Api->GETInfoUpdateDate(opts.url+"/"+opts.ver+"/"+opts.info,token); // Get the infos
    if(isOKresponse(response)){
      temp = stringToSec( ParseResponse(response,"token_expiration_time"));
      if(temp==-1){
         Api->POSTAlert(opts.url+"/"+opts.ver+"/"+opts.alerts,token,opts.device,"Invalid token_expiration_time interval ","field_issue");
      }else
      {
        token_expiration_time=temp;
      }
    }
    else{
      Api->POSTAlert(opts.url+"/"+opts.ver+"/"+opts.alerts,token,opts.device,response,"initialization_issue");
    }
    while(!isOKresponse(response) && ( ((double)clock() / CLOCKS_PER_SEC)-startGetCount ) < retryTime);//wait here "retryTime" if GETDescr failed, then retry GETDescr

  }while(!isOKresponse(response));


  do{// GET DESCRIPTION
    if( ( ((double)clock() / CLOCKS_PER_SEC) - startLogCount ) >= token_expiration_time ){//verify token validity
      authenticate();
    }
    startGetDescrCount=(double)clock() / CLOCKS_PER_SEC;
    response = Api->GETDescr(opts.url+"/"+opts.ver+"/"+opts.devs+"/"+opts.id, token); // Get the description
    if(isOKresponse(response)){
      // temp = stringToSec(ParseResponse(response,"period"));
      // if(temp==-1){
      //    Api->POSTAlert(opts.url+"/"+opts.ver+"/"+opts.alerts,token,opts.device,"Invalid period interval ","field_issue");

      // }else
      // {
      //   period=temp;
      // }
    
      // temp = stringToSec(ParseResponse(response,"cycle"));
      // if(temp==-1){
      //    Api->POSTAlert(opts.url+"/"+opts.ver+"/"+opts.alerts,token,opts.device,"Invalid cycle interval ","field_issue");

      // }else
      // {
      //   cycle=temp;
      // }
      temp = stringToSec(ParseResponse(response,"retryTime"));
      if(temp==-1){
         Api->POSTAlert(opts.url+"/"+opts.ver+"/"+opts.alerts,token,opts.device,"Invalid retryTime interval ","field_issue");

      }else
      {
        retryTime=temp;
      }
      scriptsId = ParseResponse(response,"scripts");
      features= ParseResponse(response,"features");
    }
    else{
      Api->POSTAlert(opts.url+"/"+opts.ver+"/"+opts.alerts,token,opts.device,response,"initialization_issue");
    }
    while(!isOKresponse(response) && ( ((double)clock() / CLOCKS_PER_SEC)-startGetDescrCount ) < retryTime);//wait here "retryTime" if GETDescr failed, then retry GETDescr

  }while(!isOKresponse(response));
  

  do{// GET SCRIPTS
    if( ( ((double)clock() / CLOCKS_PER_SEC) - startLogCount ) >= token_expiration_time ){//verify token validity
      authenticate();
    }
    startGetCount = (double)clock() / CLOCKS_PER_SEC;  
    retrieveScriptsCode(token, scriptsId); 
    
    while(firstGetScriptsResponse=="none" && ( ((double)clock() / CLOCKS_PER_SEC)-startGetCount ) < retryTime);//wait here "retryTime" if login failed, then retry login
  }while(firstGetScriptsResponse=="none" );
 
  initialization=false;
}


int edgine::evaluate(vector<sample*> samples){

  //Check if we have to request a new TOKEN and info & date
  logCount = ((double)clock() / CLOCKS_PER_SEC)-startLogCount ;      
  if( logCount >= token_expiration_time -period){ //every "token_expiration_time" authenticate (the engine could go to sleep while has to update the token so: token_expiration_time-period)
    authenticate();
    setToken(token); // Update the token in each script

    response = Api->GETInfoUpdateDate(opts.url+"/"+opts.ver+"/"+opts.info,token); // To update tokentoken_expiration_time and datetime
    if(isOKresponse(response)){
      temp = stringToSec( ParseResponse(response,"token_expiration_time"));
      if(temp==-1){
         Api->POSTAlert(opts.url+"/"+opts.ver+"/"+opts.alerts,token,opts.device,"Invalid token_expiration_time interval ","field_issue");
      }else
      {
        token_expiration_time=temp;
      }
    }
    else{
      Api->POSTAlert(opts.url+"/"+opts.ver+"/"+opts.alerts,token,opts.device,response); // generic
    }
  }

  //Check if we have to update scripts and description
  getDescrCount = (double)clock() / CLOCKS_PER_SEC-startGetDescrCount;

  if( getDescrCount >= cycle ){
    startGetDescrCount=(double)clock() / CLOCKS_PER_SEC;
    response = Api->GETDescr(opts.url+"/"+opts.ver+"/"+opts.devs+"/"+opts.id, token); // Get the description
    //if GET fails does not matter, the engine use the previous description 
    if(isOKresponse(response)){
      // temp = stringToSec(ParseResponse(response,"period"));
      // if(temp==-1){
      //    Api->POSTAlert(opts.url+"/"+opts.ver+"/"+opts.alerts,token,opts.device,"Invalid period interval ","field_issue");
      // }else
      // {
      //   period=temp;
      // }    
      // temp = stringToSec(ParseResponse(response,"cycle"));
      // if(temp==-1){
      //    Api->POSTAlert(opts.url+"/"+opts.ver+"/"+opts.alerts,token,opts.device,"Invalid cycle interval ","field_issue");
      // }else
      // {
      //   cycle=temp;
      // }
      temp = stringToSec(ParseResponse(response,"retryTime"));
      if(temp==-1){
         Api->POSTAlert(opts.url+"/"+opts.ver+"/"+opts.alerts,token,opts.device,"Invalid retryTime interval ","field_issue");
      }else
      {
        retryTime=temp;
      }
      scriptsId = ParseResponse(response,"scripts");
      features= ParseResponse(response,"features");
      retrieveScriptsCode(token, scriptsId);
      //if retrieve fails does not matter, the engine use the previous scripts
    }
    else{
      Api->POSTAlert(opts.url+"/"+opts.ver+"/"+opts.alerts,token,opts.device,response);// generic
    }
    /*
    if (isOKresponse(response)){
      cycle = ParseResponse(response,"cycle")!="" ? ParseResponse(response,"cycle").toInt():600; //default 10 minutes
      scriptsId = ParseResponse(response,"scripts");
      features= ParseResponse(response,"features");
      retrieveScriptsCode(token, scriptsId);
      //if retrieve fails does not matter, the engine use the previous scripts
    }*/
    
  }
 
  return executeScripts(samples);
}

void edgine::authenticate(){
  do{
    startLogCount= (double)clock() / CLOCKS_PER_SEC;
    //startLogCount = millis();      
    response=Api->POSTLogin(opts.url+"/"+opts.ver+"/"+opts.login, opts.username, opts.password); // Authentication
    if(isOKresponse(response)){
      token = ParseToken(response);
    }else{
      Api->POSTAlert(opts.url+"/"+opts.ver+"/"+opts.alerts,token,opts.device,response+" LOGIN FAILED");// generic
    }

    while(!isOKresponse(response) && ( ((double)clock() / CLOCKS_PER_SEC)-startLogCount ) < retryTime);//wait "retryTime" if login failed, then retry login
  }while(!isOKresponse(response));
}

void edgine::retrieveScriptsCode(String token, String scriptsId){
  firstGetScriptsResponse="ok";
  int startIndex=1;
  int endIndex=1;

  //Serial.println("retrieveScriptsCode :"+scriptsId);
  scriptsId.replace(" ","");//delete whitespace
  
  while( startIndex < scriptsId.length() ){
    
    endIndex=scriptsId.indexOf("\"",startIndex+1); // start the search from the next charater    
    scriptId= scriptsId.substring(startIndex,endIndex);

    codeResponse = Api->GETScript(opts.url + "/" + opts.ver + "/" + opts.scps + "?filter={\"_id\":\"" + scriptId + "\"}", token);   

    if (!isOKresponse(codeResponse)){//if any of the requests fails
      firstGetScriptsResponse = "none";// THIS VARIABLE MATTERS ONLY THE FIRST TIME WE GET SCRIPTS
      tempCode="none";// do not block all scripts retrieve because only one fails, try others
      if(initialization){
        Api->POSTAlert(opts.url+"/"+opts.ver+"/"+opts.alerts,token,opts.device,codeResponse,"initialization_issue");
      }
      else{
        Api->POSTAlert(opts.url+"/"+opts.ver+"/"+opts.alerts,token,opts.device,codeResponse);//generic issue
      }
    }
    else{
      tempCode=ParseResponse( codeResponse,"code");
    }
    
    if(tempCode!="none"){
      //verify if it is a new script
      for(int i=0;i<scripts.size();i++){
        if(scripts[i]->scriptId==scriptId && scripts[i]->scriptStr==tempCode ){ //if there is already this script
          Serial.println("Script Unchanged: "+scriptId);
          scripts[i]->valid=true; // set it to valid because it is already in the API
          goto cnt; //is already present so do nothing and go to retrieve next script
        }
        else if(scripts[i]->scriptId==scriptId){ //if there is already this script but the code has changed
          Serial.println("Script changed: "+scriptId);
          scripts[i]->valid=false;// invalidate the old version of the script and then create the new version of it   
        }
      }    
      
      //create the script
      scripts.push_back(new script(scriptId, tempCode, opts.thing, opts.device, opts.url+"/"+opts.ver+"/"+opts.measurements, token, features) );
      if(scripts.back()->valid==false){//if the creation of the script has failed 

        
        if(scripts.back()->operations.size()<=1){ // if the script has only one operation the error is in the feature
          String feature = scripts.back()->feature;// the feature is the one with the error
          Api->POSTAlert(opts.url+"/"+opts.ver+"/"+opts.alerts,token,opts.device,feature+" has an error","script_issue"); 
        }else{
          String invalidOperation = scripts.back()->operations.back()->getName();// the last operation created is the one with the error
          Api->POSTAlert(opts.url+"/"+opts.ver+"/"+opts.alerts,token,opts.device,invalidOperation+" has an error","script_issue"); 
        }
        delete scripts.back(); //delete the script to free memory
        scripts.pop_back();//remove last script
      }
      
      cnt:; //go directly here if a script already exists and it is not changed
    
    }

    startIndex=endIndex+3;//+3 because we want to avoid: "," characters between two scripts id

    
  }
  
  //  delete scripts that are not valid anymore (deleted in the API)
  for(int i=0;i<scripts.size();i++){
    if(scripts[i]->valid==false){
      Serial.println("Script deleted: "+scripts[i]->scriptId);
      delete scripts[i];
      scripts.erase(scripts.begin()+i);
      i--; //since we deleted a script, even if the scripts size stays unchanged every position is shifted by one
    }
    else{
      Serial.println("Script valid: "+scripts[i]->scriptId);
      scripts[i]->valid=false;// preset valid to false, to be reconfirmed on the next check in the API
    }
  }
  Serial.println("There are: " + String( scripts.size() )+" scripts");

}


int edgine::executeScripts(vector<sample*> samples){
	samplesSent=0;
	for(int j=0;j<samples.size();j++){
		for(int i=0;i<scripts.size();i++){
			if(scripts[i]->feature==samples[j]->feature){
				if( scripts[i]->execute( samples[j] ) )
					samplesSent++;//useful only for testing
			}
		}
	}
  return samplesSent;
}

void edgine::setToken(String token){
  for(int i=0;i<scripts.size();i++){
    scripts[i]->setToken(token);
  }
}


String edgine::ParseToken(String response){
  if(response.indexOf("J")!=-1){
    return response.substring( response.indexOf("J"), response.lastIndexOf("\"") );
  }
  return "none";
}

//NOT WORKS WITH CUSTOM OPERATIONS 
String edgine::ParseResponse( String response, String fieldName ){
  
  if( response.indexOf(fieldName) ==-1){
    Api->POSTAlert(opts.url+"/"+opts.ver+"/"+opts.alerts,token,opts.device,fieldName+" field is not present!","field_issue");
    Serial.println(fieldName+" field is not present!");
    return "none";
  }
  response.replace(" ","");//delete whitespace
  int beginOfValue = response.indexOf( ":", response.indexOf(fieldName) )+1;//find starting index of field value
  int endOfValue;
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
  int nextClose;
  while(true){
    String delimiters= "";
    int nextOpen = response.indexOf( first, start );
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

int edgine::stringToSec( String numString){
  int numberValue ;
  if(numString!=""){ // if there is no time interval we assign 0 
    char lastChar = numString.charAt(numString.length()-1);
    if (lastChar>'9' || lastChar<'0'){ // if the last char is not a number we try to interpret it as a measure unit
      //check the validity of the interval, there must be only numbers 
      if(!isaDigit(numString.substring(0,numString.length()-1)))
      {
        return -1;// if there is an error in the numString
      }
      numberValue = ( numString.substring(0,numString.length()-1) ).toInt(); // if conversion fails a 0 is returned
      
      switch( lastChar ){
      case 's'://do nothing is already in Second
        break;
      case 'm':// minutes
        numberValue*=60;
        break;
      case 'h':// hours
        numberValue*=60*60;
        break;
      case 'd':// days
        numberValue*=60*60*24;
        break;
      default:// if  char non contemplated
        return -1;// if there is an error in the numString
        break;
      }
    }
    else{
      numberValue = ( numString.substring(0,numString.length()) ).toInt();
    }
  }
  
  return numberValue;
}



boolean edgine::isOKresponse(String response){
  int code=response.substring(0,3).toInt();// if toInt() fails the code would be 0
  return code>=200 && code<210;
}

boolean edgine::isaDigit(String numberStr){
  for (int i = 0; i < numberStr.length(); i++)
  {
    char c=numberStr.charAt(i);
    if(c>'9' || c<'0')
    {
      return false;
    }
  }
  return true;
}
double edgine::getPeriod(){
  return period;
}
#endif 
