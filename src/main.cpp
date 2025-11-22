#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>

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
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Create LCD instance (address 0x27, 16 cols, 2 rows)

void setup() {
  Serial.begin(9600);  // Initialize serial communications
  while (!Serial);     // Wait for serial port to connect (for native USB boards)
  
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("RFID Ready");
  lcd.setCursor(0, 1);
  lcd.print("Scan card...");
  
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
  
  // Show card type
  Serial.print("Card Type: ");
  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  Serial.println(mfrc522.PICC_GetTypeName(piccType));
  
  Serial.println("=================================");
  Serial.println();
  
  // Check if this is Evan's card (UID: 59 D4 11 9E)
  if (mfrc522.uid.size == 4 &&
      mfrc522.uid.uidByte[0] == 0x59 &&
      mfrc522.uid.uidByte[1] == 0xD4 &&
      mfrc522.uid.uidByte[2] == 0x11 &&
      mfrc522.uid.uidByte[3] == 0x9E) {
    // Evan's card detected!
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Hello Evan");
    Serial.println("Welcome, Evan!");
  } else {
    // Different card detected
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Card Detected");
    lcd.setCursor(0, 1);
    // Display UID on LCD
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      if (mfrc522.uid.uidByte[i] < 0x10) lcd.print("0");
      lcd.print(mfrc522.uid.uidByte[i], HEX);
      if (i < mfrc522.uid.size - 1) lcd.print(" ");
    }
  }
  
  delay(2000);  // Display message for 2 seconds
  
  // Return to ready state
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("RFID Ready");
  lcd.setCursor(0, 1);
  lcd.print("Scan card...");
  
  // Halt PICC
  mfrc522.PICC_HaltA();
  
  // Stop encryption on PCD
  mfrc522.PCD_StopCrypto1();
}

