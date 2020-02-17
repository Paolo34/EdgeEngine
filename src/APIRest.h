//APIRest.h

#ifndef APIRest_h
#define APIRest_h
#include <string>
using std::string;

#include <HTTPClient.h>
#include <time.h>
#include <pthread.h>
#include "sample.h"
/*
#define DAY  86400000 // 86400000 milliseconds in a day
#define HOUR  3600000 // 3600000 milliseconds in an hour
#define MINUTE  60000 // 60000 milliseconds in a minute
*/
#define SECOND  1000 // 1000 milliseconds in a second

typedef struct{
  string device;
  string date;
  string message;
  string type;
  string url;
} issue;

typedef struct{
  //input
  sample* sam;
  string token;
} taskParameter;

//SINGLETON wrapper
class APIRest{

  private:

  //variables
  int httpCode;
  char httpCodeTmp [4];
  string response;
  boolean success;
  unsigned long startingTime;
  unsigned long timeElapsed;
  string timestamp;
  char actualDate [15];
  vector<issue> issueDB;
  vector<sample> database;
  boolean reposting;

  
  taskParameter parameters;
  
  //constructor
  APIRest();
  
  //variables
  static APIRest* instance;
  
  //methods
  boolean isHTTPCodeOk(int);
  boolean needToBeRePOST(string);
  string ParseResponse(string,string,boolean);
  void deleteSpaces(string);
  void rePOSTMeasurement(string);
  void rePOSTIssue(string);

  static void POSTMeasurement_task( void *  );

  public:
  //variables
  
  //methods
  static APIRest* getInstance();
  string POSTLogin(string,string,string);
  string GETInfoUpdateDate(string,string);
  string GETDescr(string,string);
  string GETScript(string,string);
  boolean POSTMeasurement(sample,string);
  boolean POSTIssue(string,string,string,string,string,string);
  string getActualDate();
  boolean TESTING;
  int getSampleDBsize();
  int getIssueDBsize();
  
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

string APIRest::POSTLogin (string url, string username, string password){
  if(!TESTING){
    HTTPClient http;
    http.begin(url.c_str()); //Specify the URL and certificate
    http.addHeader("Content-Type","application/json");
    httpCode = http.POST( ("{\"username\": \"" + username + "\",\"password\": \"" + password + "\"}").c_str() );//this is the body
    
    itoa(httpCode,httpCodeTmp,10);
    
    response=string(httpCodeTmp)+http.getString().c_str();
    
    if (!isHTTPCodeOk(httpCode)) { //Check for the returning code
      Serial.print(F("[HTTPS] POST Login... failed," ));
      if(httpCode<0)
        response=response+" error: "+http.errorToString(httpCode).c_str();      
    }
    Serial.println(response.c_str());
    http.end(); //Free the resources
    
    return response;
  }
  else  // Mocking for the Unit Test
  { //example of token response
    if(username=="username" && password =="password"){
      return "200{\"token\": \"JWT token\"}";

    }
    else if(username=="username1" && password =="password1"){
      return "200{\"token\": \"JWT token1\"}";
    }
    else
    {
      return "none";
    }
      
  }
  
}
string APIRest::GETInfoUpdateDate(string url, string token){
  if(!TESTING)
  {
    HTTPClient http;
    http.begin(url.c_str()); //Specify the URL and certificate
    http.addHeader("Authorization",token.c_str());
    httpCode = http.GET();//the body is empty
    itoa(httpCode,httpCodeTmp,10);
    response=string(httpCodeTmp)+http.getString().c_str();
    if (isHTTPCodeOk(httpCode)) { //Check for the returning code
          
          startingTime = ((double)clock() / CLOCKS_PER_SEC)*SECOND; //milliseconds
          timestamp = ParseResponse(response,"timestamp",true);
          //example of timestamp in milliseconds: "1580394697254" 
    }
    else {
      if(httpCode<0)
        response=response+" error: "+http.errorToString(httpCode).c_str();
      Serial.print(F("[HTTPS] GET Date... failed, "));

    }
    Serial.println(response.c_str());
    http.end(); //Free the resources

    
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
    else if(token=="JWT token1"){
      return "200{"
                "\"version\": \"0.2.001\","
                "\"environment\": \"production\","
                "\"token_expiration_time\": \"30h\","
                "\"database\": \"mongodb://localhost:27017/atmosphere-prod\","
                "\"timestamp\": \"1581425017114\""
              "}";
    }
    else{
      return "none";
    }
  }
   
}

string APIRest::GETDescr(string url, string token){
  if(!TESTING){
    HTTPClient http;
    http.begin(url.c_str()); //Specify the URL and certificate

    http.addHeader("Authorization",token.c_str());
    httpCode = http.GET();//the body is empty
    itoa(httpCode,httpCodeTmp,10);
    response=string(httpCodeTmp)+http.getString().c_str();
    if (!isHTTPCodeOk(httpCode)) { //Check for the returning code
      if(httpCode<0)
        response=response+" error: "+http.errorToString(httpCode).c_str();
      Serial.print(F("[HTTPS] GET Description... failed, "));      
    }
    Serial.println(response.c_str());
    http.end(); //Free the resources

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
    }else if(token=="JWT token1"){
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
        "\"period\": \"5a\","
        "\"cycl\": \"10m\","
        "\"retryTime\": \"1d0s\","
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

string APIRest::GETScript(string url, string token){
  if(!TESTING){
    HTTPClient http;
    http.begin(url.c_str()); //Specify the URL and certificate

    http.addHeader("Authorization",token.c_str());
    httpCode = http.GET();//the body is empty
    itoa(httpCode,httpCodeTmp,10);
    response=string(httpCodeTmp)+http.getString().c_str();

    if (!isHTTPCodeOk(httpCode)) { //Check for the returning code
      if(httpCode<0)
        response=response+" error: "+http.errorToString(httpCode).c_str();
      Serial.print(F("[HTTPS] GET Script... failed, "));
    }
    Serial.println(response.c_str());
    http.end(); //Free the resources

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
    if(token=="JWT token1"){

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
      else{
        return "none";
      }
    }
    else{
      return "none";
    }
  }
   
}
/*
pthread_t thread;
int returnValue;
void *printThreadId(void *threadid) {
   Serial.println((int)threadid);
}*/

// void APIRest::POSTMeasurement_task( void * pvParameters ){
//     taskParameter param = *(taskParameter *) pvParameters;

//     HTTPClient http;
//     http.begin(param.sam->url); //Specify the URL and certificate 
//     http.addHeader("Content-Type","application/json");
//     http.addHeader("Authorization",param.token);
//     int httpCode = http.POST("{\"thing\": \""+param.sam->thing+"\", \"feature\": \""+param.sam->feature+"\", \"device\": \""+param.sam->device+"\", \"script\": \""+param.sam->scriptId+"\", \"samples\": {\"values\":"+param.sam->value+"}, \"startDate\": \""+param.sam->startDate+"\", \"endDate\": \""+param.sam->endDate+"\"}" );//this is the body
//     String response=httpCode+http.getString();
    
//     if (!isHTTPCodeOk(httpCode)) {// something has gone wrong in the POST
//       // if the post has encoutered an error, we want to save datum that will be resent as soon as possible
//       if(httpCode<0)
//         response+=" error: "+HTTPClient::errorToString(httpCode);
//       if( needToBeRePOST(response)){
//         database.push_back(param.sam);// save the datum in a local database
//         Serial.print(F("[HTTPS] POST NewMeas... failed"));
//         Serial.print(", value: "+String(param.sam->value));
//         Serial.println(", script: "+param.sam->scriptId);
//       }
//       else{
//         Serial.println(F("Measurement aleady POSTed"));
//       }
      
//     }
//     Serial.println(response);
//     http.end(); //Free the resources

//     if(!reposting){
//       reposting=true;
//       rePOSTMeasurement(param.token); // every time we post a new measurement retry to post all the failed ones
//     }
//     if(!reposting){
//       reposting=true;
//       rePOSTIssue(param.token); // every time we post a new measurement retry to post all the failed alerts
//     }

//     vTaskDelete(NULL);
// }

boolean APIRest::POSTMeasurement(sample sam,string token){
  if(!TESTING){
    
    // parameters.sam=&sam;
    // parameters.token=token;
    // xTaskCreatePinnedToCore( POSTMeasurement_task,"POSTMeas TASK",1000,&parameters,2,NULL,1);
    HTTPClient http;
    http.begin(sam.url.c_str()); //Specify the URL and certificate 
    http.addHeader("Content-Type","application/json");
    http.addHeader("Authorization",token.c_str());
    //Arduino does not support std::to_string(double) so I used here String(double).c_str()
    httpCode = http.POST(("{\"thing\": \""+sam.thing+"\", \"feature\": \""+sam.feature+"\", \"device\": \""+sam.device+"\", \"script\": \""+sam.scriptId+"\", \"samples\": {\"values\":"+String(sam.value).c_str()+"}, \"startDate\": \""+sam.startDate+"\", \"endDate\": \""+sam.endDate+"\"}").c_str() );//this is the body

    itoa(httpCode,httpCodeTmp,10);
    response=string(httpCodeTmp)+http.getString().c_str();

    if (isHTTPCodeOk(httpCode)) { //Check for the returning code
      success=true;
    }
    else {// something has gone wrong in the POST
      // if the post has encoutered an error, we want to save datum that will be resent as soon as possible
      if(httpCode<0)
        response=response+" error: "+http.errorToString(httpCode).c_str();
      if( needToBeRePOST(response)){
        database.push_back(sam);// save the datum in a local database
        Serial.print(F("[HTTPS] POST NewMeas... failed"));
        Serial.print(", value: ");
        Serial.print(sam.value);
        Serial.print(", script: ");
        Serial.println(sam.scriptId.c_str());
        success=false;
      }
      else{
        success=true;// if don't need to be resent
        Serial.println(F("Measurement aleady POSTed"));
      }
      
    }
    Serial.println(response.c_str());
    http.end(); //Free the resources

    if(!reposting){
      reposting=true;
      rePOSTMeasurement(token); // every time we post a new measurement retry to post all the failed ones
    }
    if(!reposting){
      reposting=true;
      rePOSTIssue(token); // every time we post a new measurement retry to post all the failed alerts
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
      Serial.println(token.c_str());
      Serial.println("resPOSTMeasurement");
      rePOSTMeasurement(token); // every time we post a new measurement retry to post all the failed ones
    }
    if(!reposting){
      reposting=true;
      Serial.println(token.c_str());
      Serial.println("resPOSTIssue");
      rePOSTIssue(token); // every time we post a new measurement retry to post all the failed alerts
    }
    
    return success;    
  }
}

void APIRest::rePOSTMeasurement(string token){
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

boolean APIRest::POSTIssue(string url,string token,string device,string message,string type="generic",string date=APIRest::getInstance()->getActualDate()){
  if(!TESTING){

    HTTPClient http;
    http.begin(url.c_str()); //Specify the URL and certificate 
    http.addHeader("Content-Type","application/json");
    http.addHeader("Authorization",token.c_str());
    httpCode = http.POST(("{\"device\": \""+device+"\",  \"date\": \""+date+"\", \"message\": \""+message+"\",\"type\": \""+type+"\"}").c_str() );//this is the body
    itoa(httpCode,httpCodeTmp,10);
    response=string(httpCodeTmp)+http.getString().c_str();

    if (isHTTPCodeOk(httpCode)) { //Check for the returning code
      success=true;
    }
    else {// something has gone wrong in the POST
      if(httpCode<0)
        response=response+" error: "+http.errorToString(httpCode).c_str();
      if( needToBeRePOST(response)){
        issue al;
        al.date=date;
        al.device=device;
        al.message=message;
        al.type=type;
        al.url=url;
        // if the post has encoutered an error, we want to save datum that will be resent as soon as possible
        issueDB.push_back(al);// save the datum in a local database
        Serial.println(F("[HTTPS] POST Issue... failed"));
        
        success=false;
      }else{
        Serial.println(F("Issue aleady POSTed"));
        success=true;// if don't need to be resent
      }
    }
    Serial.println(response.c_str());
    http.end(); //Free the resources

    return success;
  }

  else{
    if(token=="JWT token"){
      success= true; 
    }
    else{
      issue al;
      al.date=date;
      al.device=device;
      al.message=message;
      al.type=type;
      al.url=url;
      // if the post has encoutered an error, we want to save datum that will be resent as soon as possible
      issueDB.push_back(al);// save the datum in a local database
      success= false;
    }

    return success;
  }
}
void APIRest::rePOSTIssue(string token){
  
  // j is useful to count the number of iteration equal to database size; 
  // since after repost the first element we erase it, the next one shift to the first position so access database[0] till end
  for(int j=0; j<issueDB.size(); j++){
    APIRest::POSTIssue(issueDB[0].url,token,issueDB[0].device,issueDB[0].message,issueDB[0].type,issueDB[0].date);
    issueDB.erase( issueDB.begin() );// don't need to delete every issue individually because we passed the struct and not the pointer
  }
  vector<issue>(issueDB).swap(issueDB);// this create a new database with capacity equal to the size
  reposting=false;
} 


boolean APIRest::isHTTPCodeOk(int code){
  return  code>=200 && code<=308;
}


boolean APIRest::needToBeRePOST(string response){
    if( ParseResponse(response,"value",false)=="6"){// "value"= 6 means that the resource was not created for some problem(usually because it already exists), so do not try create it again
      return false;
    }
    return true;
}
 

string APIRest::getActualDate(){

  timeElapsed = ((unsigned long)clock() / CLOCKS_PER_SEC)*SECOND - startingTime ; //in milliseconds
  ultoa(atol(timestamp.c_str()) + timeElapsed, actualDate, 10);
  return string(actualDate);
}


string APIRest::ParseResponse( string response, string fieldName, boolean quotedField = true ){
  
  if( response.find(fieldName) ==-1){
    Serial.println("NONE FIELD");
    return "";
  }
  deleteSpaces(response);
  
  int beginOfValue = response.find( ":", response.find(fieldName) )+1;//find starting index of field value
  int endOfValue;
  string fieldValue;

  if(quotedField){ // example "fieldName": "fieldValue"
    endOfValue = response.find('\"',beginOfValue+1);// start looking for the last delimiter from the next value
    fieldValue=response.substr( beginOfValue+1, endOfValue - (beginOfValue+1));
  }
  else{ // example "fieldName": fieldValue
    endOfValue = response.find(',',beginOfValue);// start looking for the last delimiter from the next value
    if(response.at(endOfValue-1) =='}' || response.at(endOfValue-1) ==']'){// if the field is the last of the a JSON objects({...}) or JSON array([...])
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
void APIRest::deleteSpaces(string str){
  int pos=0;
  while ( ( pos=str.find(" ") ) !=-1){
    str.erase(pos,1);//delete whitespace
  }
}
int APIRest:: getSampleDBsize(){
 return database.size();
}
int APIRest::getIssueDBsize(){
  return issueDB.size();
}


#endif
