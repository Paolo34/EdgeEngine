//APIRest.h

#ifndef APIRest_h
#define APIRest_h

#include <HTTPClient.h>
#define DAY  86400000 // 86400000 milliseconds in a day
#define HOUR  3600000 // 3600000 milliseconds in an hour
#define MINUTE  60000 // 60000 milliseconds in a minute
#define SECOND  1000 // 1000 milliseconds in a second

//SINGLETON wrapper
class APIRest{

  private:

  //variables
  int httpsCode;
  String response;
  boolean success;
  unsigned long startingTime;
  String startingDate;
  String actualDate;
  
  //constructor
  APIRest();
  
  //variables
  static APIRest* instance;
  
  //methods
  String getDate();
  
  public:
  //variables
  vector<measureData> database;
  //methods
  static APIRest* getInstance();
  String POSTLogin(String,String,String);
  boolean POSTMeasurement(String,String,String,String,String,String,double,String);
  void rePOSTMeasurement(String);
  String GETDescr(String,String);
  String GETScript(String,String);
  
  
};

APIRest* APIRest::instance=NULL;

APIRest* APIRest::getInstance(){
  if(instance==NULL){
    instance= new APIRest();
  }
  return instance;
}

APIRest::APIRest(){
  //HERE GET DATETIME FROM THE ROUTE OF DATE
  startingDate="2019-11-25T23:25:06.324Z";
  startingTime = millis();
}

String APIRest::POSTLogin (String url, String username, String password){
  HTTPClient https;
  https.begin(url); //Specify the URL and certificate
  
  https.addHeader("Content-Type","application/json");
  httpsCode = https.POST("{\"username\": \"" + username + "\",\"password\": \"" + password + "\"}");//this is the body

  if (httpsCode > 0) { //Check for the returning code
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

String APIRest::GETDescr(String url, String token){
   HTTPClient https;
   https.begin(url); //Specify the URL and certificate

   https.addHeader("Authorization",token);
   
   httpsCode = https.GET();//the body is empty
   if (httpsCode > 0) { //Check for the returning code
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

String APIRest::GETScript(String url, String token){
   HTTPClient https;
   https.begin(url); //Specify the URL and certificate

   https.addHeader("Authorization",token);
   
   httpsCode = https.GET();//the body is empty
   if (httpsCode > 0) { //Check for the returning code
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

boolean APIRest::POSTMeasurement(String url,String token,String thing,String feature,String device,String scriptId,double input,String date=APIRest::getInstance()->getDate()){
  HTTPClient https;
  https.begin(url); //Specify the URL and certificate
  https.addHeader("Content-Type","application/json");
  https.addHeader("Authorization",token);
  int httpsCode = https.POST("{\"thing\": \""+thing+"\", \"feature\": \""+feature+"\", \"device\": \""+device+"\", \"script\": \""+scriptId+"\", \"samples\": {\"values\":"+input+"},\"timestamp\": \"" +date+"\"}" );//this is the body
  if (httpsCode < 210) { //Check for the returning code
    Serial.println(httpsCode);
    Serial.println(https.getString());
    success=true;
  }
  else {
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
  return success;
}

void APIRest::rePOSTMeasurement(String token){
  // j is useful to count the number of iteration equal to database size; 
  // since after repost the first element we erase it, the next one shift to the first position so access database[0] till end
  
  for(int j=0; j<database.size(); j++){
    APIRest::POSTMeasurement(database[0].url, token, database[0].thing, database[0].feature, database[0].device, database[0].scriptId, database[0].value, database[0].date);
    database.erase( database.begin() );
    Serial.println(database.size());
  }
  
}

String APIRest::getDate(){

  unsigned long timeElapsed = millis()-startingTime ;
  
  int dayElapsed = timeElapsed / DAY ;                                //number of days
  int hourElapsed = (timeElapsed % DAY) / HOUR;                       //the remainder from days division (in milliseconds) divided by hours, this gives the full hours
  int minuteElapsed = ((timeElapsed % DAY) % HOUR) / MINUTE ;         //and so on...
  int secondElapsed = (((timeElapsed % DAY) % HOUR) % MINUTE) / SECOND;
  int milliSecElapsed = (((timeElapsed % DAY) % HOUR) % MINUTE) % SECOND ;
  
  int year = startingDate.substring(0,4).toInt();
  int month = startingDate.substring(5,7).toInt();
  int day = startingDate.substring(8,10).toInt();
  int hour = startingDate.substring(11,13).toInt();
  int minute = startingDate.substring(14,16).toInt();
  int second = startingDate.substring(17,19).toInt();
  int milliSec = startingDate.substring(20,23).toInt();
  
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
   
  Serial.println("year: "+String(year));
  Serial.println("month: "+String(month));
  Serial.println("day: "+String(day));
  Serial.println("hour: "+String(hour));
  Serial.println("minute: "+String(minute));
  Serial.println("second: "+String(second));
  Serial.println("millisec: "+String(milliSec));

  actualDate = String(year) + "-" + ( month<10 ? "0"+String(month):String(month) ) + "-" + ( day<10 ? "0"+String(day):String(day) ) + "T" +
            ( hour<10 ? "0"+String(hour):String(hour) )+":"+( String(minute) )+":"+(String(second) )+ // ( second<10 ? "0"+String(second):String(second) )
            "." + ( milliSec<100 ? ( milliSec<10 ? "00"+String(milliSec):"0"+String(milliSec) ):String(milliSec) ) + "Z"; //( minute<10 ? "0"+String(minute):String(minute) )
            
  Serial.println("date: "+actualDate);
  
  if(dayElapsed>=40){//since millis() overflows after about 50 days 
    startingTime=millis();
    startingDate=actualDate;    
  }
  
  return actualDate;
}

#endif
