#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>
 
#define SS_PIN 10
#define RST_PIN 4
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
 
Servo doorServo;
String command = "";
int redPin= 5;
int greenPin = 6;
int bluePin = 7;

int ledMode=0;

void setup() {
  Serial.begin(9600);

  //Door pins
  doorServo.attach(8);
  doorServo.write(90);

  //RGB pins
  pinMode(redPin,  OUTPUT);              
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522

}

void loop() {
  if (Serial.available()) {
    command = Serial.readStringUntil('\n');
    command.trim();
    if (command == "light on") {
      ledMode=1;
      setColor(255, 255, 255);
      Serial.println("DONE");
    }else if ((command == "purple") && (ledMode==1)) {
      setColor(170, 0, 255);
      Serial.println("DONE");
    }else if ((command=="green") && (ledMode==1)){
      setColor(0,  255, 0);
      Serial.println("DONE");
    } else if ((command=="red") && (ledMode==1)){
      setColor(255, 0, 0);
      Serial.println("DONE");
    } else if ((command=="blue") && (ledMode==1)){
      setColor(0, 0, 255);
      Serial.println("DONE");
    } else if ((command=="yellow") && (ledMode==1)){
      setColor(255, 255, 0);
      Serial.println("DONE");
    } else if ((command=="white") && (ledMode==1)){
      setColor(255, 255, 255);
      Serial.println("DONE");
    } else if ((command=="orange") && (ledMode==1)){
      setColor(255, 60, 0);
      Serial.println("DONE");
    } else if (command == "light off") {
      ledMode=0;
      setColor(0,  0, 0);
      Serial.println("DONE");
    } else if (command == "open door") {
      Serial.println("Waiting for card... (10 sec)");
      
      unsigned long startTime = millis();
      bool cardDetected = false;

      while (millis() - startTime < 10000) { // 10 sec scan
        if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
          cardDetected = true;
          break;
        }
      }
      if (!cardDetected) {
        Serial.println("No card detected");
        return;
      }
      // Show UID on serial monitor
      Serial.print("UID tag :");
      String content = "";
      for (byte i = 0; i < mfrc522.uid.size; i++) {
        Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
        Serial.print(mfrc522.uid.uidByte[i], HEX);
        content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
        content.concat(String(mfrc522.uid.uidByte[i], HEX));
      }
      Serial.println();
      content.toUpperCase();

      if (content.substring(1) == "E2 0E 66 EE") {
        Serial.println("Authorized access, opening door");
        Serial.println("DONE");  
        doorServo.write(0);
      } else {
        Serial.println("Access denied");
        Serial.println("DONE");  
      }

    } else if (command == "close door") {
      doorServo.write(90);
      Serial.println("DONE");
    }
  }
}

void setColor(int redValue, int greenValue,  int blueValue) {
  analogWrite(redPin, redValue);
  analogWrite(greenPin,  greenValue);
  analogWrite(bluePin, blueValue);
}





