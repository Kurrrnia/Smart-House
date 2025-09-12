#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define SS_PIN 10
#define RST_PIN 9
#define LED 17

// Deklarasi pin untuk sensor air dan servo
const int sensorAirPin = A0;  // Sensor air terhubung ke pin analog A0
const int ldrPin = A1;  // Pin LDR terhubung ke A1

MFRC522 mfrc522(SS_PIN, RST_PIN);
Servo myServo; 
Servo clothesServo;
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Alamat I2C mungkin berbeda (0x27 atau 0x3F)

String allowedUID = "34 58 EF 51";  // Ganti dengan UID dari kartu/kunci RFID Anda yang diizinkan

void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  SPI.begin();
  mfrc522.PCD_Init();
  myServo.attach(3);
  clothesServo.attach(6);  // Servo untuk jemuran otomatis di pin 6
  myServo.write(90);  // Servo pintu pada posisi tertutup
  clothesServo.write(90);  // Jemuran pada posisi keluar (awal)

  // Inisialisasi LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("HANDS ON ARDUINO");
  lcd.setCursor(0, 1);
  lcd.print("SMART HOUSE");

  Serial.println("Tempatkan kartu RFID di dekat reader...");
}

void loop() {
  // Bagian RFID untuk membuka pintu
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    String content = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
      content.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    content.toUpperCase();

    Serial.println("UID: " + content);
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("UID: ");
    lcd.setCursor(0, 1);
    lcd.print(content);

    if (content.substring(1) == allowedUID) {
      Serial.println("Akses diizinkan");
      Serial.println("Silakan Masuk");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Akses diizinkan");
      lcd.setCursor(0, 1);
      lcd.print("Silakan Masuk");

      myServo.write(0);  // Buka pintu
      digitalWrite(LED, HIGH);
     // lcd.clear();
      //lcd.setCursor(0, 0);
     // lcd.print("LAMPU MENYALA");
      delay(5000);  // Tunggu 5 detik
      myServo.write(90);  // Tutup pintu
      digitalWrite(LED, LOW);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("HANDS ON ARDUINO");
    lcd.setCursor(0, 1);
    lcd.print("SMART HOUSE");
     // digitalWrite(LED, LOW);
    } else {
      Serial.println("Akses ditolak");
      Serial.println("Hubungi Pemilik Rumah");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Akses Ditolak");
      lcd.setCursor(0, 1);
      lcd.print("Hubungi Pemilik");
      delay(2000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("HANDS ON ARDUINO");
    lcd.setCursor(0, 1);
    lcd.print("SMART HOUSE");
    }

    mfrc522.PICC_HaltA();  // Halt komunikasi RFID
    delay(1000);
  }

  // Bagian Jemuran Otomatis menggunakan Sensor Air
 
  // Membaca nilai dari sensor air
  int sensorValue = analogRead(sensorAirPin);
   Serial.println(sensorValue);
  // Menentukan posisi servo berdasarkan nilai sensor
  if (sensorValue > 200) {  // Ubah angka 500 sesuai dengan nilai ambang batas sensor air Anda
     Serial.println("Air terdeteksi, jemuran masuk");
     lcd.clear();
    lcd.setCursor(0, 0);
     lcd.print("Air terdeteksi");
    lcd.setCursor(0, 1);
     lcd.print("Jemuran masuk");
     delay(2000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("HANDS ON ARDUINO");
    lcd.setCursor(0, 1);
    lcd.print("SMART HOUSE");
    for (int x  =  90; x>0;x--){
      clothesServo.write(x);
      Serial.println(x);
      delay(10);
    }
    //;  // Posisi servo untuk menarik jemuran masuk
    ////delay(100);
  } else {
    // Tidak ada air, jemuran keluar
    Serial.println("Tidak ada air, jemuran keluar");


    clothesServo.write(90);  // Posisi servo untuk mengeluarkan jemuran

  delay(100);}
  }


  

