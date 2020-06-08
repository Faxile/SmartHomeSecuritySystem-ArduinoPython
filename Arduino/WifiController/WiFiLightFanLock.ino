#include <WiFi.h>
#include <IOXhop_FirebaseESP32.h>
#include <HTTPClient.h>

#define WIFI_SSID ""
#define WIFI_PASSWORD ""
#define FIREBASE_HOST "smarthomefyp-f9244.firebaseio.com"
#define FIREBASE_AUTH "tnKgoJBwESx2pP4WMqJL0S35TknWS4cGbydAESbk"

char light;
char fan;
char lock;
int lght = 27;
int fn = 26;
int ulck = 33;
int lck = 32;
void setup() {
  pinMode(lght, OUTPUT);
  digitalWrite(lght, HIGH);
  pinMode(fn, OUTPUT);
  digitalWrite(fn, HIGH);
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
  
}
void loop() {
  // put your main code here, to run repeatedly:
  String command = Firebase.getString("/sensorstatus/command");
  //String patt = Firebase.getString("/sensorstatus/pattern");
  light = command.charAt(0);
  fan = command.charAt(1);
  lock = command.charAt(2);
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
 
  delay(1000);
}