#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>

// If using the breakout with SPI, define the pins for SPI communication.
#define PN532_SCK  (2)
#define PN532_MOSI (3)
#define PN532_SS   (4)
#define PN532_MISO (5)

#define PN532_IRQ   (2)
#define PN532_RESET (3)  // Not connected by default on the NFC Shield


Adafruit_PN532 nfc(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS);


uint8_t compare[] = {0x0E, 0x8C, 0x5F, 0x23};
  uint8_t uid[] = { 0, 0, 0, 0};  
    uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)


void setup(void) {
  Serial.begin(115200);
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(13,OUTPUT);

  
  while (!Serial) delay(10); 
  
  Serial.println("Hello!");

  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }
  // Got ok data, print it out!
  Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
  
  nfc.SAMConfig();
  
  Serial.println("Waiting for an ISO14443A Card ...");
}

void PrintHex8(uint8_t *data, uint8_t length) // prints 8-bit data in hex with leading zeroes
{
     char tmp[16];
       for (int i=0; i<length; i++) {
         sprintf(tmp, "0x%.2X",data[i]);
         Serial.print(tmp); Serial.print(" ");
       }
}

bool hexCompare(){
  for(int i=0; i<uidLength; i++){
      if(uid[i] != compare [i])
      {
        return false; 
         
      }
    }
    return true;
  
  }

void loop(void) {
 
 uint8_t success;
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

  
  if (success) {
    // Display some basic information about the card
    Serial.println("Found an ISO14443A card");
    Serial.print("  UID Length: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
    Serial.print("  UID Value: ");
    nfc.PrintHex(uid, uidLength);
   
    Serial.println("");

      Serial.print("With uint8_t array:  "); PrintHex8(uid,uidLength); Serial.print("\n");

    
      
      if (hexCompare())
      { 
       
      Serial.println("IDs matched unlocking door");
      uint8_t data[32];
      Serial.println("Starting doorlock");
      
      

        digitalWrite(12,HIGH);
        delay(2000);
        digitalWrite(12,LOW);

        
        
        
      }
      else
      {
       Serial.println("Wrong ID locking the door");
      
        
        digitalWrite(13,HIGH);
        delay(2000);
        digitalWrite(13,LOW);
      }      
      
    }
 
    else
    {
      Serial.println("This doesn't seem to be an NTAG203 tag (UUID length != 7 bytes)!");
    }
    
    // Wait a bit before trying again
    Serial.println("\n\nEnter another tag");
   // Serial.flush();
   // while (!Serial.available());
   // while (Serial.available()) {
   // Serial.read();
   // }
    //Serial.flush();    
  }
  
