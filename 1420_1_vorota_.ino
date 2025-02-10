#include "SPI.h"
#include "MFRC522.h"
#include "Time.h"
#include <Stepper_28BYJ.h>
#include <Wire.h> // библиотека для управления устройствами по I2C 
#include <LiquidCrystal_I2C.h> // подключаем библиотеку для QAPASS 1602
#define LED1_PIN 2
LiquidCrystal_I2C LCD(0x27,16,2); // присваиваем имя LCD для дисплея
#define STEPS 4078
Stepper_28BYJ stepper(STEPS, 4,5,6,7);
#define RST_PIN 9 // RES pin
#define SS_PIN  10 // SDA (SS) pin
const uint8_t valid_uid[] = { 0x4A, 0x51, 0x6F, 0x81 };// верная
const uint8_t valid_uidp[] = { 0x22, 0x73, 0x95, 0x53 };// просроченная
MFRC522 mfrc522(SS_PIN, RST_PIN); // создание объекта mfrc522
int row_excel = 0; // количество строк
int test = 423; // тестовая переменная, которая будет передана excel

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  stepper.setSpeed(30);
  LCD.init(); 
  LCD.backlight(); 
  LCD.setCursor(1, 0);    
  LCD.clear() ;
  LCD.print("You are welcome!"); 
  pinMode(LED1_PIN, OUTPUT);
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(1, HIGH);
}

void loop() {
  if(!mfrc522.PICC_IsNewCardPresent())
    return;
  if(!mfrc522.PICC_ReadCardSerial())
    return;
  row_excel++; 
  Serial.print("DATE,TIME,"); // запись в excel текущей даты и времени
  byte piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
  delay(1000);
  if((mfrc522.uid.size == sizeof(valid_uid)) &&
      (memcmp(&mfrc522.uid.uidByte, &valid_uid,
              sizeof(valid_uid)) == 0))
  {
    LCD.clear() ;  
    LCD.print("Open");   
    digitalWrite(LED1_PIN, HIGH);
    stepper.step(6000); 
    stepper.step(-3000);
    if (Serial.println(digitalRead(1))){
      delay(6000);
      stepper.step(3000);
      delay(6000);
      stepper.step(-3000);
      }
    else{
      stepper.step(-3000);
    }
    digitalWrite(LED1_PIN, LOW);
    }
    if((mfrc522.uid.size == sizeof(valid_uidp)) &&
      (memcmp(&mfrc522.uid.uidByte, &valid_uidp,
              sizeof(valid_uid)) == 0))
  {
      LCD.clear(); 
      LCD.print("Exspired");
      delay(1000);
    }
    else {
      tone(A1, 1000, 1000);
      LCD.clear();      
      LCD.print("Error");
      delay(1000);
    }
  }



