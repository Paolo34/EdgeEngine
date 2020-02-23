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
  long  startingTime;
  double timeElapsed;
  string timestamp;
  string actualDate ;
  vector<issue> issueBuffer;
  vector<sample> sampleBuffer;
  int issueBufferSize=20;
  int sampleBufferSize=20;
  int decimationPolicyFactor = 2; // size/decimationPolicyFactor; that is half of data will be deleted
  boolean reposting;
  
  taskParameter parameters;
  
  //constructor
  APIRest();
  
  //variables
  static APIRest* instance;
  
  //methods
  boolean isHTTPCodeOk(int);
  boolean needToBeRePOST(string);
  string parseResponse(string,string,boolean);
  void deleteSpaces(string);
  void rePOSTMeasurement(string);
  void rePOSTIssue(string);
  void checkIssueBufferSize();
  void checkSampleBufferSize();

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
  int getSampleBufferSize();
  int getIssueBufferSize();
  void setSampleBufferSize(int);
  void setIssueBufferSize(int);
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
          timestamp = parseResponse(response,"timestamp",true);
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
        "\"scriptListMaxSize\": 5,"
        "\"measurementBufferSize\": 20,"
        "\"issueBufferSize\": 20,"
        "\"sendBufferSize\": 20,"
        "\"scriptStatementMaxSize\": 5,"
        "\"statementBufferSize\": 10,"
        "\"measurementBufferPolicy\": \"decimation\","
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
        "\"scriptListMaxSize\": 5,"
        "\"measurementBufferSize\": 20,"
        "\"issueBufferSize\": 20,"
        "\"sendBufferSize\": 20,"
        "\"scriptStatementMaxSize\": 5,"
        "\"statementBufferSize\": 10,"
        "\"measurementBufferPolicy\": \"decimation\","
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
      if( parseResponse(url,"_id",true) == "group-temperature"){
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
      else if(parseResponse(url,"_id",true)=="average-hourly-temperature"){
        return "200{\"docs\": ["
              "{"
                "\"visibility\": \"private\","
                "\"tags\": [],"
                "\"_id\": \"average-hourly-temperature\","
                "\"code\": \"temperature(60).window(+, 0, 10).map(a/100).send()\","
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
      if( parseResponse(url,"_id",true) == "group-temperature"){
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
      else if(parseResponse(url,"_id",true)=="average-hourly-temperature"){

        return "200{\"docs\": ["
              "{"
                "\"visibility\": \"private\","
                "\"tags\": [],"
                "\"_id\": \"average-hourly-temperature\","
                "\"code\": \"temperature(60).window(+, 0, 10).map(a/100).send()\","
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
//         sampleBuffer.push_back(param.sam);// save the datum in a local sampleBuffer
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
    //[TBD] Arduino does not support std::to_string(double) so I used here String(double).c_str()
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
        checkSampleBufferSize();//if there is not space, make it
        sampleBuffer.push_back(sam);// save the datum in a local sampleBuffer
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
      checkSampleBufferSize();//if there is not space, make it
      sampleBuffer.push_back(sam);// save the datum in a local sampleBuffer
      success = false;
    }
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
}

void APIRest::rePOSTMeasurement(string token){
  // j is useful to count the number of iteration equal to sampleBuffer size; 
  // since after repost the first element we erase it, the next one shift to the first position so access sampleBuffer[0] till end
  int size=sampleBuffer.size();
  for(int j=0; j<size; j++){
    APIRest::POSTMeasurement(sampleBuffer[0], token);
    sampleBuffer.erase( sampleBuffer.begin());
  }
  vector<sample>(sampleBuffer).swap(sampleBuffer);// this create a new sampleBuffer with capacity equal to the size
  
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

      if( needToBeRePOST(response) ){
        issue al;
        al.date=date;
        al.device=device;
        al.message=message;
        al.type=type;
        al.url=url;

        checkIssueBufferSize();//if there is not space, make it
        // if the post has encoutered an error, we want to save datum that will be resent as soon as possible
        issueBuffer.push_back(al);// save the datum in a local sampleBuffer
        
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

      checkIssueBufferSize();//if there is not space, make it
      // if the post has encoutered an error, we want to save datum that will be resent as soon as possible
      issueBuffer.push_back(al);// save the datum in a local Buffer
      success= false;
    }

    return success;
  }
}
void APIRest::rePOSTIssue(string token){
  // j is useful to count the number of iteration equal to Buffer size; 
  // since after repost the first element we erase it, the next one shift to the first position so access issueBuffer[0] till end
  int size=issueBuffer.size();
  for(int j=0; j<size; j++){
    APIRest::POSTIssue(issueBuffer[0].url,token,issueBuffer[0].device,issueBuffer[0].message,issueBuffer[0].type,issueBuffer[0].date);
    issueBuffer.erase( issueBuffer.begin() );// don't need to deallocate every issue individually because we passed the struct and not the pointer
  }
  vector<issue>(issueBuffer).swap(issueBuffer);// this create a new Buffer with capacity equal to the size, that frees memory
  reposting=false;
} 


boolean APIRest::isHTTPCodeOk(int code){
  return  code>=200 && code<=308;
}


boolean APIRest::needToBeRePOST(string response){
    if( parseResponse(response,"value",false)=="6"){// "value"= 6 means that the resource was not created for some problem(usually because it already exists), so do not try create it again
      return false;
    }
    return true;
}

string APIRest::getActualDate(){

  timeElapsed = ((double)clock() / CLOCKS_PER_SEC)*SECOND - startingTime ; //in milliseconds
  //[TBD] Arduino does not support std::to_string(double) so I used here String(double).c_str()
  //ftoa(atof(timestamp.c_str()) + timeElapsed, actualDate, 10);
  return string( String( atof(timestamp.c_str()) + timeElapsed ).c_str() );
}


string APIRest::parseResponse( string response, string fieldName, boolean quotedField = true ){
  deleteSpaces(response);
  if( response.find(fieldName) ==-1){
    Serial.print(fieldName.c_str());
    Serial.println(" field is not present!");
    return "";
  }
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

int APIRest:: getSampleBufferSize(){
 return sampleBuffer.size();
}
int APIRest::getIssueBufferSize(){
  return issueBuffer.size();
}

void APIRest:: setSampleBufferSize(int size){
  // if(size<sampleBuffer.size()){ [TBD]
  //   //Call the correct policy
  // }
  if(size<sampleBuffer.size()){
    sampleBuffer.erase(sampleBuffer.begin(), sampleBuffer.begin()+(sampleBuffer.size()-size));
    vector<sample>(sampleBuffer).swap(sampleBuffer);// this create a new Buffer with capacity equal to the size, that frees memory allocated with the erased issue
  }
  sampleBufferSize=size;
  sampleBuffer.reserve(size);//useful if (size>sampleBuffer.size()) in other cases it does nothing, no need of "if clause"

}

void APIRest::setIssueBufferSize(int size){
  if(size<issueBuffer.size()){
    issueBuffer.erase( issueBuffer.begin(), issueBuffer.begin()+(issueBuffer.size()-size) );// don't need to delete every issue individually because we passed the struct and not the pointer
    vector<issue>(issueBuffer).swap(issueBuffer);// this create a new Buffer with capacity equal to the size, that frees memory allocated with the erased issue
  }
  issueBufferSize=size;
  issueBuffer.reserve(size);//useful if (size>issueBuffer.size()) in other cases it does nothing, no need of "if clause"
}

void APIRest::checkIssueBufferSize(){
  if(issueBufferSize<=issueBuffer.size()-(reposting? 1:0) ){ //if the rePOSTing of an issue fails, when this check is done the issue is already at the begin of issueBuffer,
    // so do not take into account its presence (so issueBuffer.size()-1), beacuse the issue will be deleted from the begin of the queue and added back to the end.
    // don't need to deallocate every issue individually because we passed the struct and not the pointer
    issueBuffer.erase( issueBuffer.begin(), issueBuffer.begin()+ issueBufferSize/decimationPolicyFactor); //delete issueBufferSize/decimationPolicyFactor issue 
    vector<issue>(issueBuffer).swap(issueBuffer);// this create a new Buffer with capacity equal to the size, that frees memory allocated with the erased issues
  }
}
void APIRest::checkSampleBufferSize(){
  if(sampleBufferSize<=sampleBuffer.size()-(reposting? 1:0) ){ //if the rePOSTing of a sample fails, when this check is done the sample is already at the begin of sampleBuffer,
    // so do not take into account its presence (so sampleBuffer.size()-1), beacuse the sample will be deleted from the begin of the queue and added back to the end.
    //[TBD]
    Serial.println("sample buffer FULL");
    // don't need to deallocate every sample individually because we passed the struct and not the pointer
    sampleBuffer.erase( sampleBuffer.begin(), sampleBuffer.begin()+ sampleBufferSize/decimationPolicyFactor); //delete sampleBufferSize/decimationPolicyFactor sample 
    vector<sample>(sampleBuffer).swap(sampleBuffer);// this create a new Buffer with capacity equal to the size, that frees memory allocated with the erased samples
  }
}

#endif
