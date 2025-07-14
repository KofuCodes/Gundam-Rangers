#include <MFRC522.h>
#include <SPI.h>
#include <Arduino.h>

#define SS_PIN   18  // RC522 SDA (SS)
#define RST_PIN  21  // RC522 Reset
#define SCK_PIN  23  // RC522 Clock
#define MOSI_PIN 22  // RC522 MOSI
#define MISO_PIN 19  // RC522 MISO

const int RED_PIN = 5;
const int GREEN_PIN = 4;
const int BLUE_PIN = 2;

MFRC522 mfrc522(SS_PIN, RST_PIN); 

void setup() {
  Serial.begin(9600);
  delay(1000);  // Stability delay

  pinMode(RED_PIN, OUTPUT);  // PWM Channel 0
  pinMode(GREEN_PIN, OUTPUT);  // PWM Channel 1
  pinMode(BLUE_PIN, OUTPUT);  // PWM Channel 2
  
  SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, SS_PIN);
  mfrc522.PCD_Init();
  Serial.println("Approximate your card to the reader...");
}

void loop() {
  byte version = mfrc522.PCD_ReadRegister(mfrc522.VersionReg);
  if (version == 0x00 || version == 0xFF) {
    Serial.println("RC522 not found.");
    delay(1000);
    return;
  }
  
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  //Show UID on serial monitor
  // Serial.print("UID tag :");
  String content= "";
  byte letter;

  for (byte i = 0; i < mfrc522.uid.size; i++){
    //  Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    //  Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }

  content.toUpperCase();

  String noSpaces = "";

  for (int i = 0; i < content.length(); i++) {
    if (content[i] != ' ') {
      noSpaces += content[i];
    }
  }

  if (noSpaces == "04A96B12C42A81") {
    Serial.println("Hello, Red Ranger!");
    setColor(255, 0, 0);
  } else if (noSpaces == "04AB7112C42A81") {
    Serial.println("Hello, Green Ranger!");
    setColor(0, 255, 0);
  } else if (noSpaces == "04EC7612C42A81") {
    Serial.println("Hello, Blue Ranger!");
    setColor(0, 0, 255);
  } else {
    Serial.println("Access denied.");
  }
}

void setColor(int redValue, int greenValue, int blueValue) {
  analogWrite(RED_PIN, redValue);
  analogWrite(GREEN_PIN, greenValue);
  analogWrite(BLUE_PIN, blueValue);
}
