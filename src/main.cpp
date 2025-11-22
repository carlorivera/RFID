#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>

// RC522 Module Pin Configuration for Arduino Mega 2560
// IMPORTANT: Arduino Mega hardware SPI pins are FIXED:
// For proper operation, you MUST connect:
// NSS/SDA -> Pin 10 (your current connection - OK)
// SCK     -> Pin 52 (you have it on pin 13 - NEEDS TO BE MOVED!)
// MOSI    -> Pin 51 (you have it on pin 11 - NEEDS TO BE MOVED!)
// MISO    -> Pin 50 (you have it on pin 12 - NEEDS TO BE MOVED!)
// RST     -> Pin 5 (your current connection - OK)
// GND     -> GND
// 3.3V    -> 3.3V (IMPORTANT: Do NOT use 5V!)

#define SS_PIN 10
#define RST_PIN 5


MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

#define buzzer 8

void setup() {
  pinMode(buzzer,OUTPUT);// set digital IO pin pattern, OUTPUT to be output 
  Serial.begin(9600);  // Initialize serial communications
  while (!Serial);     // Wait for serial port to connect (for native USB boards)
  
  SPI.begin();         // Initialize SPI bus
  mfrc522.PCD_Init();  // Initialize MFRC522
  
  delay(4);            // Optional delay for some boards
  
  Serial.println("=================================");
  Serial.println("RC522 RFID Module Test Program");
  Serial.println("=================================");
  Serial.println();
  
  // Show details of PCD - MFRC522 Card Reader
  mfrc522.PCD_DumpVersionToSerial();
  Serial.println();
  Serial.println("Scan a RFID card or tag...");
  Serial.println();
}

void loop() {
  // Reset the loop if no new card present on the sensor/reader
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Card detected!
  Serial.println("=================================");
  Serial.println("CARD DETECTED!");
  Serial.println("=================================");
  
  // Show card UID
  Serial.print("Card UID: ");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }


  Serial.println();
  

  // Play different tones based on card UID
  if (mfrc522.uid.uidByte[0] == 0x59 && mfrc522.uid.uidByte[1] == 0xD4 && mfrc522.uid.uidByte[2] == 0x11 && mfrc522.uid.uidByte[3] == 0x9E) {
    // Authorized card - high pitch, single beep
    Serial.println("Authorized card! Playing success tone");
    tone(buzzer, 500);  // 2000 Hz tone
    delay(200);
    noTone(buzzer);
  } else {
    // Unknown card - low pitch, double beep
    Serial.println("Unknown card! Playing error tone");
    tone(buzzer, 500);   // 500 Hz low tone
    delay(100);
    noTone(buzzer);
    delay(50);
    tone(buzzer, 500);
    delay(100);
    noTone(buzzer);
  }

  // Show card type
  Serial.print("Card Type: ");
  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  Serial.println(mfrc522.PICC_GetTypeName(piccType));
  
  Serial.println("=================================");
  Serial.println();
  
  // Halt PICC
  mfrc522.PICC_HaltA();
  
  // Stop encryption on PCD
  mfrc522.PCD_StopCrypto1();
}

