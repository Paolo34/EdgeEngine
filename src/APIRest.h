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
  /*String zone;
  int year;
  int month;
  int day;
  int hour;
  int minute;
  int second;
  int milliSec;
  int dayElapsed;                                
  int hourElapsed; 
  int minuteElapsed;
  int secondElapsed;
  int milliSecElapsed;*/
  //String startingDate;
  String actualDate;
  vector<alert> alertDB;
  boolean reposting;
  
  //constructor
  APIRest();
  
  //variables
  static APIRest* instance;
  
  //methods
  boolean isHTTPCodeOk(int);
  boolean needToBeRePOST(String);
  String errorToString(int);
  String ParseResponse(String,String,boolean);
  void rePOSTMeasurement(String);
  void rePOSTAlert(String);

  public:
  //variables
  vector<sample> database;
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
      return " {\"token\": \"JWT token\"}";
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
          /*time_t time=timestamp.substring(0,timestamp.length()-3).toDouble();// first to last 3 digit are seconds
          milliSec=timestamp.substring(timestamp.length()-3,timestamp.length()).toInt(); // last 3 digit are milliseconds
          tm *tm_local =localtime(&time);
          second=tm_local->tm_sec;
          minute=tm_local->tm_min;
          hour=tm_local->tm_hour;
          day=tm_local->tm_mday;
          month=tm_local->tm_mon+1; // months are counted from 0 to 11
          year=tm_local->tm_year+1900; // years are counted since 1900
          */
    }
    else {
      ////////////// WHAT IF THE GET DATE FAILS?????? /////////////////
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
      return "2019-12-14T12:25:06.324Z";
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
      return "{"
        "\"features\": ["
          " \"temperature\""
        "],"
        "\"tags\": [],"
        "\"scripts\": ["
          "\"average-hourly-temperature\","
            "\"group-temperature\""
        "],"
        "\"visibility\": \"private\","
        "\"period\": 10,"
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
			return "{\"docs\": ["
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
			return "{\"docs\": ["
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
      return true; 
    }
    else{
      return false;
    }
  }
}
void APIRest::rePOSTAlert(String token){
  
  // j is useful to count the number of iteration equal to database size; 
  // since after repost the first element we erase it, the next one shift to the first position so access database[0] till end
  for(int j=0; j<alertDB.size(); j++){
    APIRest::POSTAlert(alertDB[0].url,token,alertDB[0].device,alertDB[0].message,alertDB[0].type,alertDB[0].date);
    alertDB.erase( alertDB.begin() );// don't need to delete every alert individually because we passed the struct and not the pointer
  }
  reposting=false;
} 


boolean APIRest::isHTTPCodeOk(int code){
  return  code>=200 && code<210;
}


boolean APIRest::needToBeRePOST(String response){
    if( ParseResponse(response,"value",false)=="6"){// "value"= 6 means that the resource already exists, so do not try create it again
      return false;
    }
    return true;
}
/*
String APIRest::errorToString(int error){
    switch(error) {
    case HTTPC_ERROR_CONNECTION_REFUSED:
        return F("connection refused");
    default:
        return String();
    }
}*/

String APIRest::getActualDate(){

  timeElapsed = ((double)clock() / CLOCKS_PER_SEC)*SECOND - startingTime ; //in milliseconds

  /*
  startingTime+= timeElapsed; //update the starting time 
  
  //hypotesis: between 2 call of this function elapses less than 28 days
  dayElapsed = timeElapsed / DAY ;                                //number of days
  hourElapsed = (timeElapsed % DAY) / HOUR;                       //the remainder from days division (in milliseconds) divided by hours, this gives the full hours
  minuteElapsed = ((timeElapsed % DAY) % HOUR) / MINUTE ;         //and so on...
  secondElapsed = (((timeElapsed % DAY) % HOUR) % MINUTE) / SECOND;
  milliSecElapsed = (((timeElapsed % DAY) % HOUR) % MINUTE) % SECOND ;
  Serial.println("day: "+ String(dayElapsed)+", hour: "+ String(hourElapsed)+", min: "+ String(minuteElapsed)+", sec: "+ String(secondElapsed)+", millis: "+String(milliSecElapsed) );
  

  milliSec+=milliSecElapsed;
  second+=(secondElapsed+milliSec/1000);
  milliSec%=1000;
  minute+=(minuteElapsed+second/60);
  second%=60;
  hour+=(hourElapsed+minute/60);
  minute%=60;
  day+=(dayElapsed+hour/24);
  hour%=24;
  //Serial.println("day: "+ String(day)+", hour: "+ String(hour)+", min: "+ String(minute)+", sec: "+ String(second)+", millis: "+String(milliSec) );
  switch(month){
    case 1:
      if(day>31){
        day%=31;
        month=2;
      }
      break;
    case 2:
      if((year%100==0 && year%400==0) || (year%100!=0 && year%4==0)){ //bissextile years
        if(day>29){
          day%=29;
          month=3;
        }
      }
      else{//not bissextile years
        if(day>28){
          day%=28;
          month=3;
        }
      }
      break;
    case 3:    
    if(day>31){
        day%=31;
        month=4;
      }
      break;
    case 4:
      if(day>30){
        day%=30;
        month=5;
      }
      break;
    case 5:
      if(day>31){
        day%=31;
        month=6;
      }
      break;
    case 6:
      if(day>30){
        day%=30;
        month=7;
      }
      break;
    case 7:
      if(day>31){
          day%=31;
          month=8;
        }
      break;
    case 8:
      if(day>31){
        day%=31;
        month=9;
      }
      break;
    case 9:
      if(day>30){
        day%=30;
        month=9;
      }
      break;
    case 10:
      if(day>31){
        day%=31;
        month=10;
      }
      break;
    case 11:
      if(day>30){
        day%=30;
        month=11;
      }
      break;
    case 12:
      if(day>31){
          day%=31;
          month=1;
          year++;
        }
      break;
  }

  actualDate = String(year) + "-" + ( month<10 ? "0"+String(month):String(month) ) + "-" + ( day<10 ? "0"+String(day):String(day) ) + "T" +
            ( hour<10 ? "0"+String(hour):String(hour) )+":"+( ( minute<10 ? "0"+String(minute):String(minute) ) )+":"+(( second<10 ? "0"+String(second):String(second) ) )+ // ( second<10 ? "0"+String(second):String(second) )
            "." + ( milliSec<100 ? ( milliSec<10 ? "00"+String(milliSec):"0"+String(milliSec) ):String(milliSec) ) + String(zone); //( minute<10 ? "0"+String(minute):String(minute) )
  */
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


#endif
