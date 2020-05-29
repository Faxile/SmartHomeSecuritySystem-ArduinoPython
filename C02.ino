
#include <WiFi.h>
#include <IOXhop_FirebaseESP32.h>
#include "Adafruit_CCS811.h"

#define WIFI_SSID "EVO-Charji-BA71"
#define WIFI_PASSWORD "farhan7410"
#define FIREBASE_HOST "smarthomefyp-f9244.firebaseio.com"
#define FIREBASE_AUTH "tnKgoJBwESx2pP4WMqJL0S35TknWS4cGbydAESbk"



Adafruit_CCS811 ccs;
char light;
char fan;
char lock;
char hom;
char cot;
void setup() {
  Serial.begin(9600);
  

  Serial.println("CCS811 test");

  if(!ccs.begin()){
    Serial.println("Failed to start sensor! Please check your wiring.");
    
  }
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
  String stat = Firebase.getString("/sensorstatus/command");
  String command2 = Firebase.getString("/co2/command");
  light = stat.charAt(0);
  fan = stat.charAt(1);
  lock = stat.charAt(2);
  cot = command2.charAt(0);
  hom = command2.charAt(1);

  if(cot == '1')
  {
  if(ccs.available()){
    if(!ccs.readData()){
     uint16_t val = ccs.geteCO2();
      Serial.print("CO2:");
      Serial.println(val);
      Serial.println(command2);
      Serial.print("OI");
      if(val <600 && hom == '0' && lock == '1' && light =='1' && fan =='1' )
      {
        light = '0';
        fan = '0';
        Serial.println("6555");
        command2 = light;
        command2+=fan;
        command2+=lock;
        Serial.println(command2);
        Firebase.setString("/sensorstatus/command",command2);
      }
      if(val > 600 && hom == '1' && (light == '0' || fan == '0'))
      {
        light = '1';
        fan = '1';
        Serial.println("6111");
        command2 = light;
        command2+=fan;
        command2+=lock;
        Firebase.setString("/sensorstatus/command",command2);
      }
    }
    else{
      Serial.println("ERROR!");
      
    }
    }
  }
  delay(1000);
}
