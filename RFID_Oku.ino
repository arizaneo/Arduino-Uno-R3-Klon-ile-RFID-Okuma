#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9
#define SS_PIN 10
#define LED_PIN 3

MFRC522 mfrc522(SS_PIN, RST_PIN);

// String yetkiliKart = "04 A2 5B 1F 3C 72 80"; // kendi kart UID'inle değiştir
String yetkiliKart = "A7 A0 AB 02"; // kendi kart UID'inle değiştir

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  pinMode(LED_PIN, OUTPUT);
  Serial.println("Kartınızı okutun...");
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent()) return;
  if (!mfrc522.PICC_ReadCardSerial()) return;

  String uid = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    uid += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
    uid += String(mfrc522.uid.uidByte[i], HEX);
    if (i != mfrc522.uid.size - 1) uid += " ";
  }
  uid.toUpperCase();
  Serial.println("Kart UID: " + uid);

  if (uid == yetkiliKart) {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("Yetkili kart, giriş izni verildi!");
    delay(1000);
    digitalWrite(LED_PIN, LOW);
  } else {
    Serial.println("Yetkisiz kart!");
    for (int i = 0; i < 3; i++) {
      digitalWrite(LED_PIN, HIGH);
      delay(200);
      digitalWrite(LED_PIN, LOW);
      delay(200);
    }
  }
}
