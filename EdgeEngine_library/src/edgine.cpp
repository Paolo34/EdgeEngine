/*
  edgine.cpp - Class for create an instance of Edge Engine.
  Created by Francesco Tornatore, February 24, 2020.
  Released into the public domain.
*/
#include "edgine.h"

#include <stdlib.h>

edgine* edgine::instance=NULL;

edgine* edgine::getInstance(){
  if(instance==NULL){
    instance= new edgine();
  }
  return instance;
}

edgine::edgine(){
  Api=APIRest::getInstance();  
  conn=connection::getInstance();
}

void edgine::init( options opts){
  initialization=true;
  WiFiconnected=true;

  this->opts=opts; 
  authenticate();
  do{//GET INFO
    if( ( ((double)clock() / CLOCKS_PER_SEC) - startLogCount ) >= token_expiration_time ){//verify token validity
      authenticate();
    }
    startGetCount = (double)clock() / CLOCKS_PER_SEC;
    Serial.println(token.c_str());
    response = Api->GETInfoUpdateDate(opts.url+"/"+opts.ver+"/"+opts.info,token); // Get the infos
    if(isOKresponse(response)){
      temp = stringToSec( parseResponse(response,"token_expiration_time"));
      if(temp==-1){
         Api->POSTIssue(opts.url+"/"+opts.ver+"/"+opts.issues,token,opts.device,"Invalid token_expiration_time interval ","field");
      }else
      {
        token_expiration_time=temp;
      }
    }
    else{
      Api->POSTIssue(opts.url+"/"+opts.ver+"/"+opts.issues,token,opts.device,response+" retrieving info","initialization");
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
      checkFields();
      scriptsId = parseResponse(response,"scripts");
      features= parseResponse(response,"features");
    }
    else{
      Api->POSTIssue(opts.url+"/"+opts.ver+"/"+opts.issues,token,opts.device,response+" retrieving description","initialization");
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

  setToken(token); //useful for testing only
  initialization=false;
}


int edgine::evaluate(vector<sample*> samples){

  //Check if we have to request a new TOKEN and info & date
  logCount = ((double)clock() / CLOCKS_PER_SEC)-startLogCount ;      
  if( logCount >= token_expiration_time-period  && conn->isConnected()){ //every "token_expiration_time" authenticate (the engine could go to sleep while has to update the token so: token_expiration_time-period)
    authenticate();
    setToken(token); // Update the token in each script

    response = Api->GETInfoUpdateDate(opts.url+"/"+opts.ver+"/"+opts.info,token); // To update tokentoken_expiration_time and datetime
    if(isOKresponse(response)){
      temp = stringToSec( parseResponse(response,"token_expiration_time"));
      if(temp==-1){
         Api->POSTIssue(opts.url+"/"+opts.ver+"/"+opts.issues,token,opts.device,"Invalid token_expiration_time interval ","field");
      }else
      {
        token_expiration_time=temp;
      }
    }
    else{
      Api->POSTIssue(opts.url+"/"+opts.ver+"/"+opts.issues,token,opts.device,response+" retrieving info"); // generic
    }
  }

  //Check if we have to update scripts and description
  getDescrCount = (double)clock() / CLOCKS_PER_SEC-startGetDescrCount;

  if( getDescrCount >= cycle  && conn->isConnected()){
    startGetDescrCount=(double)clock() / CLOCKS_PER_SEC;
    response = Api->GETDescr(opts.url+"/"+opts.ver+"/"+opts.devs+"/"+opts.id, token); // Get the description
    //if GET fails does not matter, the engine use the previous description 
    if(isOKresponse(response)){

      checkFields();
      scriptsId = parseResponse(response,"scripts");
      features= parseResponse(response,"features");
      retrieveScriptsCode(token, scriptsId);
      //if retrieve fails does not matter, the engine use the previous scripts
    }
    else{
      Api->POSTIssue(opts.url+"/"+opts.ver+"/"+opts.issues,token,opts.device,response+" retrieving virtual description");// generic
    }
    
  }
  if(!conn->isConnected()){
    WiFiconnected=false;
    conn->reconnect();
  }
  if(conn->isConnected() && WiFiconnected==false){
    WiFiconnected=true;
    Api->POSTIssue(opts.url+"/"+opts.ver+"/"+opts.issues,token,opts.device,"Connection had been lost! Now the engine is reconnected","connection");
  }

  return executeScripts(samples);
}

void edgine::authenticate(){
  do{
    startLogCount= (double)clock() / CLOCKS_PER_SEC; 
    response=Api->POSTLogin(opts.url+"/"+opts.ver+"/"+opts.login, opts.username, opts.password); // Authentication

    if(isOKresponse(response)){
      token = parseToken(response);
    }else{
      if(initialization)
        Api->POSTIssue(opts.url+"/"+opts.ver+"/"+opts.issues,token,opts.device,response+" LOGIN FAILED","initialization");
      else
        Api->POSTIssue(opts.url+"/"+opts.ver+"/"+opts.issues,token,opts.device,response+" LOGIN FAILED","token");

    }
    if(!conn->isConnected() && initialization){// if it looses connection during initialization, reconnect (when init is completed the connection should be checked by main program)
      WiFiconnected=false;
      conn->reconnect();
      if(conn->isConnected())
        WiFiconnected=true;
    }

    while(!isOKresponse(response) && ( ((double)clock() / CLOCKS_PER_SEC)-startLogCount ) < retryTime);//wait "retryTime" if login failed, then retry login
  }while(!isOKresponse(response));
}

void edgine::retrieveScriptsCode(string token, string scriptsId){
  firstGetScriptsResponse="ok";
  int startIndex=1;
  int endIndex=1;
  string tempCode;
  deleteSpaces(scriptsId);


  while( startIndex < scriptsId.length() ){
    endIndex=scriptsId.find("\"",startIndex+1); // start the search from the next charater    
    scriptId= scriptsId.substr(startIndex,endIndex-startIndex);

    codeResponse = Api->GETScript(opts.url + "/" + opts.ver + "/" + opts.scps + "?filter={\"_id\":\"" + scriptId + "\"}", token);   
    
    Serial.println(scriptId.c_str());
    
    if (!isOKresponse(codeResponse)){//if any of the requests fails
      firstGetScriptsResponse = "none";// THIS VARIABLE MATTERS ONLY THE FIRST TIME WE GET SCRIPTS
      tempCode="none";// do not block all scripts retrieve because only one fails, try others
      if(initialization){
        Api->POSTIssue(opts.url+"/"+opts.ver+"/"+opts.issues,token,opts.device,codeResponse+" retrieving "+scriptId+" script","initialization");
      }
      else{
        Api->POSTIssue(opts.url+"/"+opts.ver+"/"+opts.issues,token,opts.device,codeResponse+" retrieving "+scriptId+" script");//generic issue

        for(int i=0;i<scripts.size();i++){
         if(scripts[i]->scriptId==scriptId){
            scripts[i]->valid=true; // if fails to retieve the code use the previous already present
            break; //exit the for loop because the scriptId is univocal
          }
        }

      }
    }
    else{
      tempCode=parseResponse( codeResponse,"code");
    }
    
    if(tempCode!="none"){
      //verify if it is a new script
      for(int i=0;i<scripts.size();i++){
        if(scripts[i]->scriptId==scriptId && scripts[i]->scriptStr==tempCode ){ //if there is already this script
          Serial.print("Script Unchanged: ");
          Serial.println(scriptId.c_str());
          scripts[i]->valid=true; // set it to valid because it is already in the API
          goto cnt; //is already present so do nothing and go to retrieve next script
        }
        else if(scripts[i]->scriptId==scriptId){ //if there is already this script but the code has changed
          Serial.print("Script changed: ");
          Serial.println(scriptId.c_str());
          scripts[i]->valid=false;// invalidate the old version of the script and then create the new version of it   
        }
      }    
      //create the script
      scripts.push_back(new script(scriptId, tempCode, opts.thing, opts.device, opts.url+"/"+opts.ver+"/"+opts.measurements, token, features,sendBufferSize,scriptStatementMaxSize,statementBufferSize) );
      if(scripts.back()->valid==false){//if the creation of the script has failed 
        
        if(scripts.back()->operations.size()<=1){ // if the script has only one operation the error is in the feature
          string feature = scripts.back()->feature;// the feature is the one with the error
          Api->POSTIssue(opts.url+"/"+opts.ver+"/"+opts.issues,token,opts.device,feature+" has an error","script"); 
        }
        else{
          if(scripts.back()->operations.size()>scriptStatementMaxSize){
            string id=scripts.back()->scriptId;
            Api->POSTIssue(opts.url+"/"+opts.ver+"/"+opts.issues,token,opts.device,id+" has too many statements!","script"); 
          }
          else{
            string invalidOperation = scripts.back()->operations.back()->getName();// the last operation created is the one with the error
            Api->POSTIssue(opts.url+"/"+opts.ver+"/"+opts.issues,token,opts.device,invalidOperation+" has an error in "+scripts.back()->scriptId,"script"); 
          }
          
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
      Serial.print("Script deleted: ");
      Serial.println(scripts[i]->scriptId.c_str());
      delete scripts[i];
      scripts.erase(scripts.begin()+i);
      i--; //since we deleted a script, even if the scripts size stays unchanged every position is shifted by one
    }
    else{
      Serial.print("Script valid: ");
      Serial.println(scripts[i]->scriptId.c_str());

      scripts[i]->valid=false;// preset valid to false, to be reconfirmed on the next check in the API
    }
  }
  while (scripts.size()>scriptListMaxSize){
    Api->POSTIssue(opts.url+"/"+opts.ver+"/"+opts.issues,token,opts.device,"Script list full!, "+scripts.back()->scriptId+" not added ","script"); 
    Serial.print(scripts.back()->scriptId.c_str());
    Serial.println(" removed because list is full");
    delete scripts.back();
    scripts.pop_back();
  }

  Serial.print("There are: ");
  Serial.print((int)scripts.size());
  Serial.println(" scripts");
}


int edgine::executeScripts(vector<sample*> samples){
	int samplesSent=0;
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

void edgine::checkFields(){
  temp = stringToSec(parseResponse(response,"period"));
  if(temp==-1){
      Api->POSTIssue(opts.url+"/"+opts.ver+"/"+opts.issues,token,opts.device,"Invalid period interval ","field");
  }else{
    period=temp;
  }
  temp = stringToSec(parseResponse(response,"cycle"));
  if(temp==-1){
      Api->POSTIssue(opts.url+"/"+opts.ver+"/"+opts.issues,token,opts.device,"Invalid cycle interval ","field");
  }else{
    cycle=temp;
  }
  temp = stringToSec(parseResponse(response,"retryTime"));
  if(temp==-1){
      Api->POSTIssue(opts.url+"/"+opts.ver+"/"+opts.issues,token,opts.device,"Invalid retryTime interval ","field");
  }else{
    retryTime=temp;
  }
  temp = stringToSec(parseResponse(response,"scriptListMaxSize",false));  
  if(temp==-1){
      Api->POSTIssue(opts.url+"/"+opts.ver+"/"+opts.issues,token,opts.device,"Invalid scriptListMaxSize size ","field");
  }else{
    scriptListMaxSize=temp;
    scripts.reserve(scriptListMaxSize);
  }
  temp = stringToSec(parseResponse(response,"measurementBufferSize",false));
  if(temp==-1){
      Api->POSTIssue(opts.url+"/"+opts.ver+"/"+opts.issues,token,opts.device,"Invalid measurementBufferSize size ","field");
  }else{
    measurementBufferSize=temp;
    Api->setSampleBufferSize(measurementBufferSize);
  }
  temp = stringToSec(parseResponse(response,"issueBufferSize",false));
  if(temp==-1){
      Api->POSTIssue(opts.url+"/"+opts.ver+"/"+opts.issues,token,opts.device,"Invalid issueBufferSize size ","field");
  }else{
    issueBufferSize=temp;
    Api->setIssueBufferSize(issueBufferSize);
  }
  temp = stringToSec(parseResponse(response,"sendBufferSize",false));
  if(temp==-1){
      Api->POSTIssue(opts.url+"/"+opts.ver+"/"+opts.issues,token,opts.device,"Invalid sendBufferSize size ","field");
  }else{
    sendBufferSize=temp;
  }
  temp = stringToSec(parseResponse(response,"scriptStatementMaxSize",false));
  if(temp==-1){
      Api->POSTIssue(opts.url+"/"+opts.ver+"/"+opts.issues,token,opts.device,"Invalid scriptStatementMaxSize size ","field");
  }else{
    scriptStatementMaxSize=temp;
  }
  temp = stringToSec(parseResponse(response,"statementBufferSize",false));
  if(temp==-1){
      Api->POSTIssue(opts.url+"/"+opts.ver+"/"+opts.issues,token,opts.device,"Invalid statementBufferSize size ","field");
  }else{
    statementBufferSize=temp;
  }
  tempPol = parseResponse(response,"measurementBufferPolicy");
  if(!isAllowed(tempPol,policies)){
    Api->POSTIssue(opts.url+"/"+opts.ver+"/"+opts.issues,token,opts.device,"Invalid measurementBufferPolicy policy ","field");
  }else{
    measurementBufferPolicy=temp;
  }
  
}

void edgine::setToken(string token){
  for(int i=0;i<scripts.size();i++){
    scripts[i]->setToken(token);
  }
}

string edgine::parseToken(string response){
  if(response.find("J")!=-1){
    return response.substr( response.find("J"), response.rfind("\"")-response.find("J") );
  }
  return "none";
}

//NOT WORKS WITH CUSTOM OPERATIONS 
string edgine::parseResponse( string response, string fieldName, bool delimitedField){
  deleteSpaces(response);
  if( response.find(fieldName) ==-1){
    Api->POSTIssue(opts.url+"/"+opts.ver+"/"+opts.issues,token,opts.device,fieldName+" field is not present!","field");
    Serial.print(fieldName.c_str());
    Serial.println(" field is not present!");
    return "none";
  }

  int beginOfValue = response.find( ":", response.find(fieldName) )+1;//find starting index of field value
  int endOfValue;
  string fieldValue;

  if(delimitedField){ // examples: "fieldName": "fieldValue", "fieldName": ["fieldValue"], "fieldName": {"fieldValue"}
    switch ( response.at( beginOfValue ) ){
    case '\"':
      endOfValue = response.find('\"',beginOfValue+1);// start looking for the last delimiter from the next value
      break;
    case '(':
      endOfValue = findEndIndex('(',')', beginOfValue+1,response);
      break;
    case '[':
      endOfValue = findEndIndex('[',']', beginOfValue+1,response);
      break;
    case '{':
      endOfValue = findEndIndex('{','}', beginOfValue+1,response);
      break;
    }
      
    fieldValue= response.substr( beginOfValue+1, endOfValue-(beginOfValue+1));
  }
  else{ // example "fieldName": fieldValue
    endOfValue = response.find(',',beginOfValue);// start looking for the last delimiter from the next value
    if(response.at(endOfValue-1) =='}' || response.at(endOfValue-1) ==']'){// if the field is the last of the a JSON objects( {...} ) or JSON array( [...] )
      endOfValue-=1;
    }
    else if(endOfValue==-1){ //if the object is the last of response
      endOfValue = response.find('}',beginOfValue);

      if(endOfValue==-1){//if the array is the last of response
        endOfValue = response.find(']',beginOfValue);

      }
    }
    fieldValue=response.substr( beginOfValue, endOfValue-beginOfValue);
  }
  return fieldValue;
}


int edgine::findEndIndex (char first,char last, int start, string response){
  int nextClose;
  while(true){
    string delimiters= "";
    int nextOpen = response.find( first, start );
    nextClose = response.find( last, start );
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
       delimiters.erase(delimiters.length()-1);
       start=nextClose+1;
    }
    //if the next one is a open bracket
    else{
      delimiters.push_back(first);
      start=nextOpen+1;
    }
  }
  return nextClose;
}

int edgine::stringToSec( string numString){
  int numberValue=-1 ;

  if(!numString.empty()){ // if there is no time interval we assign 0 
    char lastChar = numString.at(numString.length()-1);
    if (lastChar>'9' || lastChar<'0'){ // if the last char is not a number we try to interpret it as a measure unit
      //check the validity of the interval, there must be only numbers (last may be the unit)
      if(isaDigit(numString.substr(0,numString.length()-1)))
      {
        numberValue = atoi( numString.substr(0,numString.length()-1).c_str() ); // if conversion fails a 0 is returned
        
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
    }
    else if(isaDigit(numString)){
      numberValue = atoi( numString.c_str() );
    }
  }
  
  return numberValue;
}



bool edgine::isOKresponse(string response){
  int code=atoi( response.substr(0,3).c_str() );// if toInt() fails the code would be 0
  return code>=200 && code<=308;
}

bool edgine::isaDigit(string numberStr){
  for (int i = 0; i < numberStr.length(); i++)
  {
    char c=numberStr.at(i);
    if(c>'9' || c<'0')
    {
      return false;
    }
  }
  return true;
}
bool edgine::isAllowed(string policy,string policiesAllowed){
  int startIndex=policiesAllowed.find(policy);
  if(startIndex!=-1){
	  int endIndex=policiesAllowed.find(",",startIndex);

	  if( policiesAllowed.substr( startIndex, (endIndex!=-1? endIndex-startIndex : policiesAllowed.length()-startIndex) )== policy )
		  return true;
  }
  return false;
}

void edgine::deleteSpaces(string& str){
  int pos=0;
  while ( ( pos=str.find(" ") ) !=-1){
    str.erase(pos,1);//delete whitespace
  }
}

int edgine::getPeriod(){
  return period;
}