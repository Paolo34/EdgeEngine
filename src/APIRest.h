//APIRest.h

#ifndef APIRest_h
#define APIRest_h

#include <HTTPClient.h>

#define DAY  86400000 // 86400000 milliseconds in a day
#define HOUR  3600000 // 3600000 milliseconds in an hour
#define MINUTE  60000 // 60000 milliseconds in a minute
#define SECOND  1000 // 1000 milliseconds in a second

typedef enum {
    FAILED = 400,
    BOH = 700
} error_code;



//SINGLETON wrapper
class APIRest{

  private:

  //variables
  int httpsCode;
  String response;
  boolean success;
  unsigned long startingTime;
  unsigned long timeElapsed;
  String zone;
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
  int milliSecElapsed;
  String startingDate;
  String actualDate;
  int j;
  boolean reposting;
  int beginOfValue;
  int endOfValue;
  
  //constructor
  APIRest();
  
  //variables
  static APIRest* instance;
  
  //methods
  boolean isHTTPCodeOk(int);
  String errorToString(int);
  String ParseResponse(String,String);
  void rePOSTMeasurement(String);
  
  public:
  //variables
  vector<measureData> database;
  //methods
  static APIRest* getInstance();
  String POSTLogin(String,String,String);
  String GETDate(String,String);
  String GETDescr(String,String);
  String GETScript(String,String);
  boolean POSTMeasurement(String,String,String,String,String,String,double,String);
  boolean POSTError(String,String,String,String,String,String,String);
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

  startingDate="2019-12-14T12:25:06.324Z"; //DELETE THIS WHEN THE DATE ROUTE IS WORKING
  startingTime = millis();//DELETE THIS WHEN THE DATE ROUTE IS WORKING
  zone =String("Z");//DELETE THIS WHEN THE DATE ROUTE IS WORKING

  TESTING=false;
}

String APIRest::POSTLogin (String url, String username, String password){
  if(!TESTING){
    HTTPClient https;
    https.begin(url); //Specify the URL and certificate
    
    https.addHeader("Content-Type","application/json");
    httpsCode = https.POST("{\"username\": \"" + username + "\",\"password\": \"" + password + "\"}");//this is the body

    if (isHTTPCodeOk(httpsCode)) { //Check for the returning code
        response = https.getString();
        Serial.println(httpsCode);
        Serial.println(response);
    }
    else {
      response="none";
      Serial.printf("[HTTPS] POST Login... failed, error: %s\n", https.errorToString(httpsCode).c_str());
    }
    
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
String APIRest::GETDate(String url, String token){
  if(!TESTING)
  {
    HTTPClient https;
    https.begin(url); //Specify the URL and certificate
    https.addHeader("Authorization",token);
    httpsCode = https.GET();//the body is empty
    
    if (isHTTPCodeOk(httpsCode)) { //Check for the returning code
          response = https.getString();
          Serial.println(httpsCode);
          Serial.println(response);
          
          ////////////// WE HAVE TO PARSE THE GETDATE RESPONSE /////////////////
          //startingDate = GETDate("url",token);
          //startingTime = millis();
          //zone =startingDate.substring(23,startingDate.length());
    }
    else {
      ////////////// WHAT IF THE GET DATE FAILS?????? /////////////////
      response="none";
      Serial.printf("[HTTPS] GET Date... failed, error: %s\n", https.errorToString(httpsCode).c_str());
    }
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
    if (isHTTPCodeOk(httpsCode)) { //Check for the returning code
        response = https.getString();
        Serial.println(httpsCode);
        Serial.println(response);
    }
    else {
      response="none";
      Serial.printf("[HTTPS] GET Description... failed, error: %s\n", https.errorToString(httpsCode).c_str());
    }
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
    if (isHTTPCodeOk(httpsCode)) { //Check for the returning code
          response = https.getString();
          Serial.println(httpsCode);
          Serial.println(response);
    }
    else {
      response="none";
      Serial.printf("[HTTPS] GET Script... failed, error: %s\n", https.errorToString(httpsCode).c_str());
    }
    https.end(); //Free the resources

    return response;
  }
  else{
    if(token=="JWT token"){
		if( ParseResponse(url,"_id") == "group-temperature"){
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
		else if(ParseResponse(url,"_id")=="average-hourly-temperature"){
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

boolean APIRest::POSTMeasurement(String url,String token,String thing,String feature,String device,String scriptId, double input,String date=APIRest::getInstance()->getActualDate()){
  if(!TESTING){
    HTTPClient https;
    https.begin(url); //Specify the URL and certificate 
    https.addHeader("Content-Type","application/json");
    https.addHeader("Authorization",token);
    httpsCode = https.POST("{\"thing\": \""+thing+"\", \"feature\": \""+feature+"\", \"device\": \""+device+"\", \"script\": \""+scriptId+"\", \"samples\": {\"values\":"+input+"}, \"startDate\": \""+date+"\", \"endDate\": \""+date+"\"}" );//this is the body
    if (isHTTPCodeOk(httpsCode)) { //Check for the returning code
      Serial.println(httpsCode);
      Serial.println(https.getString());
      success=true;
    }
    else {// something has gone wrong in the POST
      measureData datum;// if the post has encoutered an error, we want to save datum that will be resent as soon as possible
      datum.value=input;
      datum.date=date;
      datum.url=url;
      datum.thing=thing;
      datum.feature=feature;
      datum.device=device;
      datum.scriptId=scriptId;
      database.push_back(datum);// save the datum in a local database
      Serial.printf("[HTTPS] POST NewMeas... failed, error: %s\n", https.errorToString(httpsCode).c_str());
      success=false;
    }
    https.end(); //Free the resources
    
    if(!reposting){
      reposting=true;
      rePOSTMeasurement(token); // every time we post a new measurement retry to post all the failed ones
    }
    
    return success;
  }
  else{
    if(token=="JWT token"){
      success = true;
    }
    else{
      measureData datum;// if the post has encoutered an error, we want to save datum that will be resent as soon as possible
      datum.value=input;
      datum.date=date;
      datum.url=url;
      datum.thing=thing;
      datum.feature=feature;
      datum.device=device;
      datum.scriptId=scriptId;
      database.push_back(datum);// save the datum in a local database
      success = false;
    }
    if(!reposting){
      reposting=true;
      rePOSTMeasurement(token); // every time we post a new measurement retry to post all the failed ones
    }
    
    return success;    
  }
  
}

void APIRest::rePOSTMeasurement(String token){
  
  // j is useful to count the number of iteration equal to database size; 
  // since after repost the first element we erase it, the next one shift to the first position so access database[0] till end
  for(j=0; j<database.size(); j++){
    APIRest::POSTMeasurement(database[0].url, token, database[0].thing, database[0].feature, database[0].device, database[0].scriptId, database[0].value, database[0].date);
    //APIRest::POSTError(database[0].url, token, database[0].thing, database[0].feature, database[0].device, database[0].scriptId, database[0].value, database[0].date);
    database.erase( database.begin() );
  }
  
  reposting=false;
  
} 

boolean APIRest::POSTError(String url,String token,String thing,String feature,String device,String scriptId,String date=APIRest::getInstance()->getActualDate()){
  if(!TESTING){
    HTTPClient https;
    https.begin(url); //Specify the URL and certificate 
    https.addHeader("Content-Type","application/json");
    https.addHeader("Authorization",token);
    httpsCode = https.POST("{\"thing\": \""+thing+"\", \"feature\": \""+feature+"\", \"device\": \""+device+"\", \"script\": \""+scriptId+"\", \"startDate\": \""+date+"\", \"endDate\": \""+date+"\"}" );//this is the body
    if (isHTTPCodeOk(httpsCode)) { //Check for the returning code
      Serial.println(httpsCode);
      Serial.println(https.getString());
      success=true;
    }
    else {// something has gone wrong in the POST
      
      Serial.printf("[HTTPS] POST Error... failed, error: %s\n", https.errorToString(httpsCode).c_str());
      success=false;
    }
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


boolean APIRest::isHTTPCodeOk(int code){
  return code<210;
}

String APIRest::errorToString(int error){
    switch(error) {
    case HTTPC_ERROR_CONNECTION_REFUSED:
        return F("connection refused");
    default:
        return String();
    }
}

String APIRest::getActualDate(){

  timeElapsed = millis()-startingTime ;
  
  dayElapsed = timeElapsed / DAY ;                                //number of days
  hourElapsed = (timeElapsed % DAY) / HOUR;                       //the remainder from days division (in milliseconds) divided by hours, this gives the full hours
  minuteElapsed = ((timeElapsed % DAY) % HOUR) / MINUTE ;         //and so on...
  secondElapsed = (((timeElapsed % DAY) % HOUR) % MINUTE) / SECOND;
  milliSecElapsed = (((timeElapsed % DAY) % HOUR) % MINUTE) % SECOND ;
  
  year = startingDate.substring(0,4).toInt();
  month = startingDate.substring(5,7).toInt();
  day = startingDate.substring(8,10).toInt();
  hour = startingDate.substring(11,13).toInt();
  minute = startingDate.substring(14,16).toInt();
  second = startingDate.substring(17,19).toInt();
  milliSec = startingDate.substring(20,23).toInt();
  
  milliSec+=milliSecElapsed;
  second+=(secondElapsed+milliSec/1000);
  milliSec%=1000;
  minute+=(minuteElapsed+second/60);
  second%=60;
  hour+=(hourElapsed+minute/60);
  minute%=60;
  day+=(dayElapsed+hour/24);
  hour%=24;
  
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
        month=10;
      }
      break;
    case 10:
      if(day>31){
        day%=31;
        month=11;
      }
      break;
    case 11:
      if(day>30){
        day%=30;
        month=6;
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
            
  
  if(dayElapsed>=40){//since millis() overflows after about 50 days 
    startingTime=millis();
    startingDate=actualDate;    
  }
  
  return actualDate;
}

String APIRest::ParseResponse( String response, String fieldName ){
  
  if( response.indexOf(fieldName) ==-1){
    return "";
  }
  response.replace(" ","");//delete whitespace
  beginOfValue = response.indexOf( ":", response.indexOf(fieldName) )+1;//find starting index of field value
  
  endOfValue = response.indexOf('\"',beginOfValue+1);// start looking for the last delimiter from the next value
  
  return response.substring( beginOfValue+1, endOfValue);
}

#endif
