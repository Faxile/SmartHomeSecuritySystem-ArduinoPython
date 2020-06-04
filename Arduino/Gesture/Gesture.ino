#include <cp437font.h>
#include <LiquidCrystal_I2C.h>


#include <WiFi.h>
#include <IOXhop_FirebaseESP32.h>
#include "Adafruit_APDS9960.h"

#define WIFI_SSID ""
#define WIFI_PASSWORD ""
#define FIREBASE_HOST "smarthomefyp-f9244.firebaseio.com"
#define FIREBASE_AUTH "tnKgoJBwESx2pP4WMqJL0S35TknWS4cGbydAESbk"


// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;

// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows); 


int buzz = 4;
Adafruit_APDS9960 apds;
String set_gesture;
unsigned long start_time;
void setup() {
  Serial.begin(115200);
  delay(1000);
  if(!apds.begin()){
    Serial.println("failed to initialize Gesture! Please check your wiring.");
  }
  else Serial.println("Gesture initialized!");

  //gesture mode will be entered once proximity mode senses something close
  apds.enableProximity(true);
  apds.enableGesture(true);
  pinMode(buzz, OUTPUT);
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

  Serial.print(Firebase.getString("/gestures/command") + "\n");
  set_gesture = Firebase.getString("/gestures/command");

  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();

  lcd.setCursor(0, 0);
  // print message
  lcd.print("Last Gesture: ");
  
}
void loop() {
  digitalWrite(buzz, LOW);
  // put your main code here, to run repeatedly:
  start_time = millis();
  String read_ges = writeGesture();
  set_gesture = Firebase.getString("/gestures/command");
  
  if(read_ges == set_gesture )
  {
    Serial.println("Omg this actually works ffs!");
    String command = Firebase.getString("/sensorstatus/command");
    char lght = command.charAt(0);
    char fan = command.charAt(1);
    command = lght;
    command +=fan;
    command +='0';
    Firebase.setString("/sensorstatus/command",command);

    lcd.setCursor(0,1);
    lcd.print("Unlocked!");
    delay(1000);
    lcd.clear();
    lcd.setCursor(0, 0);
  // print message
  lcd.print("Last Gesture: ");

  
  }
  else if(read_ges != set_gesture && read_ges.length() == 4)
  {
    Serial.println("EEEEEEEEEEEEEEEEEEEEEEEE");
    digitalWrite(buzz, HIGH);
  }
  
  delay(500);
  
}

String writeGesture()
{
  String full = "";
 
  unsigned long end_time = millis();
  while(1){
   
  uint8_t gesture = apds.readGesture();

    if(gesture == APDS9960_UP)
    {
      full += "^";
      Serial.write("^");
      start_time = millis();

      lcd.setCursor(14,0);
      lcd.print("U");
    }
    
    else if(gesture == APDS9960_LEFT){
      full+= "<";
      Serial.write("<");
      start_time = millis();
      
      lcd.setCursor(14,0);
      lcd.print("L");
    }
    else if(gesture == APDS9960_RIGHT){
      full+=">";
      Serial.write(">");
      start_time = millis();

      lcd.setCursor(14,0);
      lcd.print("R");
    }
    else if(gesture == APDS9960_DOWN){
      full += "v";
      Serial.write("v");
      start_time = millis();

      lcd.setCursor(14,0);
      lcd.print("D");
    }
    else if(gesture == APDS9960_NONE)
    {
       Serial.println("NONE");
    }
    end_time = millis();
  if(end_time - start_time > 4000 || full.length()>=4){
  Serial.write("Done");
  Serial.println(full); 
  break;
  }
  }
  return full;

}
