#include <WiFi.h>
#include <IOXhop_FirebaseESP32.h>
#include <HTTPClient.h>

#define WIFI_SSID "EVO-Charji-BA71"
#define WIFI_PASSWORD "farhan7410"
#define FIREBASE_HOST "smarthomefyp-f9244.firebaseio.com"
#define FIREBASE_AUTH "tnKgoJBwESx2pP4WMqJL0S35TknWS4cGbydAESbk"

unsigned long start_time;
String current;
String prediction;
char light;
char fan;
char lock;
int lght = 27;
int fn = 26;
int ulck = 33;
int lck = 32;
void setup() {
  pinMode(lght, OUTPUT);
  pinMode(fn, OUTPUT);
  pinMode(lck, OUTPUT);
  pinMode(ulck,OUTPUT);
  Serial.begin(115200);
  delay(1000);        
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                  
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);

}
  Serial.println("Connected!");
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Serial.println(" Firebase Connected!");
  prediction = Firebase.getString("prediction");

  start_time = millis();
  
}
void loop() {
  // put your main code here, to run repeatedly:
  String command = Firebase.getString("/sensorstatus/command");
  light = command.charAt(0);
  fan = command.charAt(1);
  lock = command.charAt(2);
  if(millis() - start_time > 30000)
  {
    
    HTTPClient http;
    Serial.println("We're Here");
    http.begin("http://192.168.1.189:5000/time"); //Specify the URL
    int httpCode = http.GET();                                        //Make the request
 
    if (httpCode > 0) { //Check for the returning code
 
        current = http.getString();
        Serial.println(httpCode);
        Serial.println(current);
        if(prediction == current)
        {
          
          command = '1';
          command+='1';
          command+=lock;
          Firebase.setString("/sensorstatus/command",command);
          http.begin("http://192.168.1.189:5000/notify");
          int code2 = http.GET();
        }
      }
 
    else {
      Serial.println("Error on HTTP request");
    }
 
    http.end(); //Free the resources
    start_time = millis();
  }
  if(light == '1')
  {
    digitalWrite(lght, HIGH);
    Serial.println("Light On");
  }
  else{
    digitalWrite(lght, LOW);
    Serial.println("Light Offf");
  }
  if(fan == '1')
  {
    digitalWrite(fn, HIGH);
    Serial.println("fan On");
  }
  else{
        digitalWrite(fn, LOW);
      Serial.println("Fan off");        
  }
  if(lock == '0')
  {
    digitalWrite(ulck, HIGH);
    digitalWrite(lck, LOW);
    Serial.println("Lock On");
  }
  else{
        digitalWrite(lck, HIGH);
        digitalWrite(ulck,LOW);
        
        Serial.println("Lock off");
  }
  if(current == "02:00")
  {
      prediction = Firebase.getString("prediction");
  }
 
  delay(1000);
}
