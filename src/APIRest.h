//APIRest.h

#ifndef APIRest_h
#define APIRest_h

#include <HTTPClient.h>
#include <time.h>
#include "sample.h"
/*
#define DAY  86400000 // 86400000 milliseconds in a day
#define HOUR  3600000 // 3600000 milliseconds in an hour
#define MINUTE  60000 // 60000 milliseconds in a minute
*/
#define SECOND  1000 // 1000 milliseconds in a second


typedef struct{
  String device;
  String date;
  String message;
  String type;
  String url;
} alert;

//SINGLETON wrapper
class APIRest{

  private:

  //variables
  int httpsCode;
  String response;
  boolean success;
  unsigned long startingTime;
  unsigned long timeElapsed;
  String timestamp;
  String actualDate;
  vector<alert> alertDB;
  vector<sample> database;
  boolean reposting;
  
  //constructor
  APIRest();
  
  //variables
  static APIRest* instance;
  
  //methods
  boolean isHTTPCodeOk(int);
  boolean needToBeRePOST(String);
  String ParseResponse(String,String,boolean);
  void rePOSTMeasurement(String);
  void rePOSTAlert(String);

  public:
  //variables
  
  //methods
  static APIRest* getInstance();
  String POSTLogin(String,String,String);
  String GETInfoUpdateDate(String,String);
  String GETDescr(String,String);
  String GETScript(String,String);
  boolean POSTMeasurement(sample,String);
  boolean POSTAlert(String,String,String,String,String,String);
  String getActualDate();
  boolean TESTING;
  int getSampleDBsize();
  int getAlertDBsize();
  
};

APIRest* APIRest::instance=NULL;

APIRest* APIRest::getInstance(){
  if(instance==NULL){
    instance= new APIRest();
  }
  return instance;
}
//constructor
APIRest::APIRest(){
  reposting=false;
  TESTING=false;
}

String APIRest::POSTLogin (String url, String username, String password){
  if(!TESTING){
    HTTPClient https;
    https.begin(url); //Specify the URL and certificate
    
    https.addHeader("Content-Type","application/json");
    httpsCode = https.POST("{\"username\": \"" + username + "\",\"password\": \"" + password + "\"}");//this is the body
    response=httpsCode+https.getString();

    if (!isHTTPCodeOk(httpsCode)) { //Check for the returning code
      Serial.print(F("[HTTPS] POST Login... failed," ));
      if(httpsCode<0)
        response+=" error: "+https.errorToString(httpsCode);      
    }
    Serial.println(response);
    https.end(); //Free the resources
    
    return response;
  }
  else  // Mocking for the Unit Test
  { //example of token response
    if(username=="username" && password =="password")
      return "200{\"token\": \"JWT token\"}";
      else
      {
        return "none";
      }
      
  }
  
}
String APIRest::GETInfoUpdateDate(String url, String token){
  if(!TESTING)
  {
    HTTPClient https;
    https.begin(url); //Specify the URL and certificate
    https.addHeader("Authorization",token);
    httpsCode = https.GET();//the body is empty
    
    response=httpsCode+https.getString();
    if (isHTTPCodeOk(httpsCode)) { //Check for the returning code
          
          startingTime = ((double)clock() / CLOCKS_PER_SEC)*SECOND; //milliseconds
          timestamp = ParseResponse(response,"timestamp",true);
          //example of timestamp in milliseconds: "1580394697254" 
    }
    else {
      if(httpsCode<0)
        response+=" error: "+https.errorToString(httpsCode);
      Serial.print(F("[HTTPS] GET Date... failed, "));

    }
    Serial.println(response);
    https.end(); //Free the resources

    
    return response;
  }  
  else  // Mocking for the Unit Test
  {
    if(token=="JWT token"){
      return "200{"
                "\"version\": \"0.2.001\","
                "\"environment\": \"production\","
                "\"token_expiration_time\": \"30m\","
                "\"database\": \"mongodb://localhost:27017/atmosphere-prod\","
                "\"timestamp\": \"1581425017114\""
              "}";
    }
    else{
      return "none";
    }
  }
   
}

String APIRest::GETDescr(String url, String token){
  if(!TESTING){
    HTTPClient https;
    https.begin(url); //Specify the URL and certificate

    https.addHeader("Authorization",token);
    httpsCode = https.GET();//the body is empty
    response=httpsCode+https.getString();
    if (!isHTTPCodeOk(httpsCode)) { //Check for the returning code
      if(httpsCode<0)
        response+=" error: "+https.errorToString(httpsCode);
      Serial.print(F("[HTTPS] GET Description... failed, "));      
    }
    Serial.println(response);
    https.end(); //Free the resources

    return response;
  }
  else{// Mocking for the Unit Test
    if(token=="JWT token"){
      return "200{"
        "\"features\": ["
          " \"temperature\""
        "],"
        "\"tags\": [],"
        "\"scripts\": ["
          "\"average-hourly-temperature\","
            "\"group-temperature\""
        "],"
        "\"visibility\": \"private\","
        "\"period\": \"5s\","
        "\"cycle\": \"10m\","
        "\"retryTime\": \"10s\","
        "\"_id\": \"temperature-sensor-riccardo-office\","
        "\"owner\": {"
            "\"_id\": \"5dcec66bc67ed54963bc865c\","
            "\"username\": \"riccardo-office-temperature-sensor-username\","
            "\"type\": \"provider\""
        "}"
      "}";
    }
    else{
      return "none";
    }
  }
  
}

String APIRest::GETScript(String url, String token){
  if(!TESTING){
    HTTPClient https;
    https.begin(url); //Specify the URL and certificate

    https.addHeader("Authorization",token);
    httpsCode = https.GET();//the body is empty
    response=httpsCode+https.getString();

    if (!isHTTPCodeOk(httpsCode)) { //Check for the returning code
      if(httpsCode<0)
        response+=" error: "+https.errorToString(httpsCode);
      Serial.print(F("[HTTPS] GET Script... failed, "));
    }
    Serial.println(response);
    https.end(); //Free the resources

    return response;
  }
  else{
    if(token=="JWT token"){
		if( ParseResponse(url,"_id",true) == "group-temperature"){
			return "200{\"docs\": ["
					  "{"
						  "\"visibility\": \"private\","
						  "\"tags\": [],"
						  "\"_id\": \"group-temperature\","
						  "\"code\": \"temperature().send(5)\","
						  "\"owner\": {"
							  "\"_id\": \"5dcec66bc67ed54963bc865c\","
							  "\"username\": \"riccardo-office-temperature-sensor-username\","
							  "\"type\": \"provider\"}"
					  "}"
					"],"
					"\"totalDocs\": 1, \"limit\": 10, \"hasPrevPage\": false, \"hasNextPage\": false, \"page\": 1, \"totalPages\": 1, \"pagingCounter\": 1, \"prevPage\": null, \"nextPage\": null}";
		}
		else if(ParseResponse(url,"_id",true)=="average-hourly-temperature"){
			return "200{\"docs\": ["
					  "{"
						  "\"visibility\": \"private\","
						  "\"tags\": [],"
						  "\"_id\": \"average-hourly-temperature\","
						  "\"code\": \"temperature(10).window(+, 0, 60).map(a/60).send()\","
						  "\"owner\": {"
							  "\"_id\": \"5dcec66bc67ed54963bc865c\","
							  "\"username\": \"riccardo-office-temperature-sensor-username\","
							  "\"type\": \"provider\"}"
						  "}"
					  "],"
					"\"totalDocs\": 1, \"limit\": 10, \"hasPrevPage\": false, \"hasNextPage\": false, \"page\": 1, \"totalPages\": 1, \"pagingCounter\": 1, \"prevPage\": null, \"nextPage\": null}";
    
		}
	}
    else{
      return "none";
    }
  }
   
}

boolean APIRest::POSTMeasurement(sample sam,String token){
  if(!TESTING){
    HTTPClient https;
    https.begin(sam.url); //Specify the URL and certificate 
    https.addHeader("Content-Type","application/json");
    https.addHeader("Authorization",token);
    httpsCode = https.POST("{\"thing\": \""+sam.thing+"\", \"feature\": \""+sam.feature+"\", \"device\": \""+sam.device+"\", \"script\": \""+sam.scriptId+"\", \"samples\": {\"values\":"+sam.value+"}, \"startDate\": \""+sam.startDate+"\", \"endDate\": \""+sam.endDate+"\"}" );//this is the body
    response=httpsCode+https.getString();

    if (isHTTPCodeOk(httpsCode)) { //Check for the returning code
      success=true;
    }
    else {// something has gone wrong in the POST
      // if the post has encoutered an error, we want to save datum that will be resent as soon as possible
      if(httpsCode<0)
        response+=" error: "+https.errorToString(httpsCode);
      if( needToBeRePOST(response)){
        database.push_back(sam);// save the datum in a local database
        Serial.print(F("[HTTPS] POST NewMeas... failed"));
        Serial.print(", value: "+String(sam.value));
        Serial.println(", script: "+sam.scriptId);
        success=false;
      }
      else{
        success=true;// if don't need to be resent
        Serial.println(F("Measurement aleady POSTed"));
      }
      
    }
    Serial.println(response);
    https.end(); //Free the resources

    if(!reposting){
      reposting=true;
      rePOSTMeasurement(token); // every time we post a new measurement retry to post all the failed ones
    }
    if(!reposting){
      reposting=true;
      rePOSTAlert(token); // every time we post a new measurement retry to post all the failed alerts
    }
    
    return success;
  }
  else{ //////////////////// TESTING ////////////////////
    if(token=="JWT token"){
      success = true;
    }
    else{
      // if the post has encoutered an error, we want to save datum that will be resent as soon as possible
      database.push_back(sam);// save the datum in a local database
      success = false;
    }
    if(!reposting){
      reposting=true;
      rePOSTMeasurement(token); // every time we post a new measurement retry to post all the failed ones
    }
    if(!reposting){
      reposting=true;
      rePOSTAlert(token); // every time we post a new measurement retry to post all the failed alerts
    }
    
    return success;    
  }
}

void APIRest::rePOSTMeasurement(String token){
  // j is useful to count the number of iteration equal to database size; 
  // since after repost the first element we erase it, the next one shift to the first position so access database[0] till end
  int size=database.size();
  for(int j=0; j<size; j++){
    APIRest::POSTMeasurement(database[j], token);
    //APIRest::POSTError(database[0].url, token, database[0].thing, database[0].feature, database[0].device, database[0].scriptId, database[0].value, database[0].date);
  }
  
  database.erase( database.begin(), database.begin()+size);//remove all samples rePOSTed
  vector<sample>(database).swap(database);// this create a new database with capacity equal to the size
  
  reposting=false;
} 

boolean APIRest::POSTAlert(String url,String token,String device,String message,String type="generic",String date=APIRest::getInstance()->getActualDate()){
  if(!TESTING){
    HTTPClient https;
    https.begin(url); //Specify the URL and certificate 
    https.addHeader("Content-Type","application/json");
    https.addHeader("Authorization",token);
    httpsCode = https.POST("{\"device\": \""+device+"\",  \"date\": \""+date+"\", \"message\": \""+message+"\",\"type\": \""+type+"\"}" );//this is the body

    response=httpsCode+https.getString();

    if (isHTTPCodeOk(httpsCode)) { //Check for the returning code
      success=true;
    }
    else {// something has gone wrong in the POST
      if(httpsCode<0)
        response+=" error "+https.errorToString(httpsCode);
      if( needToBeRePOST(response)){
        alert al;
        al.date=date;
        al.device=device;
        al.message=message;
        al.type=type;
        al.url=url;
        // if the post has encoutered an error, we want to save datum that will be resent as soon as possible
        alertDB.push_back(al);// save the datum in a local database
        Serial.println(F("[HTTPS] POST Alert... failed"));
        
        success=false;
      }else{
        Serial.println(F("Alert aleady POSTed"));
        success=true;// if don't need to be resent
      }
    }
    Serial.println(response);
    https.end(); //Free the resources

    return success;
  }

  else{
    if(token=="JWT token"){
      success= true; 
    }
    else{
      alert al;
      al.date=date;
      al.device=device;
      al.message=message;
      al.type=type;
      al.url=url;
      // if the post has encoutered an error, we want to save datum that will be resent as soon as possible
      alertDB.push_back(al);// save the datum in a local database
      success= false;
    }

    return success;
  }
}
void APIRest::rePOSTAlert(String token){
  
  // j is useful to count the number of iteration equal to database size; 
  // since after repost the first element we erase it, the next one shift to the first position so access database[0] till end
  for(int j=0; j<alertDB.size(); j++){
    APIRest::POSTAlert(alertDB[0].url,token,alertDB[0].device,alertDB[0].message,alertDB[0].type,alertDB[0].date);
    alertDB.erase( alertDB.begin() );// don't need to delete every alert individually because we passed the struct and not the pointer
  }
  vector<alert>(alertDB).swap(alertDB);// this create a new database with capacity equal to the size
  reposting=false;
} 


boolean APIRest::isHTTPCodeOk(int code){
  return  code>=200 && code<210;
}


boolean APIRest::needToBeRePOST(String response){
    if( ParseResponse(response,"value",false)=="6"){// "value"= 6 means that the resource was not created for some problem(usually because it already exists), so do not try create it again
      return false;
    }
    return true;
}
 

String APIRest::getActualDate(){

  timeElapsed = ((double)clock() / CLOCKS_PER_SEC)*SECOND - startingTime ; //in milliseconds
  actualDate = String(timestamp.toDouble() + timeElapsed);
  return actualDate;
}


String APIRest::ParseResponse( String response, String fieldName, boolean quotedField = true ){
  
  if( response.indexOf(fieldName) ==-1){
    return "";
  }
  
  response.replace(" ","");//delete whitespace
  int beginOfValue = response.indexOf( ":", response.indexOf(fieldName) )+1;//find starting index of field value
  int endOfValue;
  String fieldValue;

  if(quotedField){ // example "fieldName": "fieldValue"
    endOfValue = response.indexOf('\"',beginOfValue+1);// start looking for the last delimiter from the next value
    fieldValue=response.substring( beginOfValue+1, endOfValue);
  }
  else{ // example "fieldName": fieldValue
    endOfValue = response.indexOf(',',beginOfValue);// start looking for the last delimiter from the next value
    if(response.charAt(endOfValue-1) =='}' || response.charAt(endOfValue-1) ==']'){// if the field is the last of the a JSON objects({...}) or JSON array([...])
      endOfValue-=1;
    }
    else if(endOfValue==-1){ //if the object is the last of response
      endOfValue = response.indexOf('}',beginOfValue);

      if(endOfValue==-1){//if the array is the last of response
        endOfValue = response.indexOf(']',beginOfValue);

      }
    }
    fieldValue=response.substring( beginOfValue, endOfValue);
  }
  
  return fieldValue;
}
int APIRest:: getSampleDBsize(){
 return database.size();
}
int APIRest::getAlertDBsize(){
  return alertDB.size();
}


#endif
