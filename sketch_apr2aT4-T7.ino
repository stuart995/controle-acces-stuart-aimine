#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <MFRC522.h>
#include <SPI.h>

// Define the pins for the RC522 module
#define SS_PIN 10
#define RST_PIN 9

// Create instances of the MFRC522 and LiquidCrystal_I2C classes
MFRC522 rfid(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2);  // LCD 16x2

// Known RFID card UID (Change this to match your card's UID)
byte knownUIDs[][4] = {
{0x49, 0xED, 0xAA, 0x9C},
{0xB0, 0xA7, 0x75, 0x89},
{0x81, 0x03, 0x13, 0xAF},
{0xC1, 0x59, 0x73, 0x02},
{0x66, 0x08, 0x27, 0x9E}
};

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Access Control");
  lcd.setCursor(0, 1);
  lcd.print("System Ready");
  delay(2000);
  lcd.clear();
  lcd.print("Scan your RFID");
}

void loop() {
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {

    Serial.print("UID tag: ");
    for (byte i = 0; i < rfid.uid.size; i++) {
      Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
      Serial.print(rfid.uid.uidByte[i], HEX);
    }
    Serial.println();

    if (checkUID(rfid.uid.uidByte)) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Access Granted");
      lcd.setCursor(0, 1);
      lcd.print("Welcome!");
    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Access Denied");
      lcd.setCursor(0, 1);
      lcd.print("Invalid Card");
    }

    delay(3000);
    lcd.clear();
    lcd.print("Scan your RFID");

    rfid.PICC_HaltA();
  }
}
bool checkUID(byte* uid) {
  for (byte j = 0; j < sizeof(knownUIDs) / 4; j++) {
    bool match = true;
    for (byte i = 0; i < 4; i++) {
      if (uid[i] != knownUIDs[j][i]) {
        match = false;
        break;
      }
    }
    if (match) return true;
  }
  return false;
}


