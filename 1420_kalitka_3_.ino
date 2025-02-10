#include <LiquidCrystal.h>
#include <Wire.h>
#include <Keypad.h>
#include <SPI.h>
#include <Servo.h>
#include <AmperkaKB.h>
// Задаём имя пинов дисплея
constexpr uint8_t PIN_RS = 8;
constexpr uint8_t PIN_EN = 9;
constexpr uint8_t PIN_DB4 = 10;
constexpr uint8_t PIN_DB5 = 11;
constexpr uint8_t PIN_DB6 = 12;
constexpr uint8_t PIN_DB7 = 13;
int piezoPin = A1;
Servo servo;
LiquidCrystal lcd(PIN_RS, PIN_EN, PIN_DB4, PIN_DB5, PIN_DB6, PIN_DB7);
AmperkaKB KB(7,3,4,5,6);
char pass[3] = {'4', '2', '3'}; // верный пароль
char buttons[4]; // массив нажатых кнопок
int k = 0; // счетчик нажатий
void setup() {
  Serial.begin(9600);
  // Устанавливаем размер экрана
  // Количество столбцов и строк
  lcd.begin(16, 2);
  KB.begin(KB1x4);
  servo.attach(2);
  servo.write(90);  // устанавливаем серву в закрытое сосотояние
  pinMode(1, OUTPUT);
  digitalWrite(1, HIGH); // датчик движения
  digitalWrite(0, HIGH); // замок калитки
} 
void loop() {
      lcd.clear();
      lcd.setCursor(0, 0); 
      lcd.print("Enter password!");
      lcd.setCursor(0, 1); 
      KB.read();
      if (KB.justPressed())  {
            buttons[k] = KB.getChar; // сохраняем значение кнопки в массиве
            lcd.setCursor(k, 1);
            lcd.print(buttons['*']); // выводим на экран символ '*' вместо значения кнопки
            k = k + 1; 
            if (k == 3) { 
              Serial.println( buttons);
              if (buttons[0] == pass[0] && buttons[1] == pass[1] && buttons[2] == pass[2])
              { 
                lcd.clear();
                lcd.setCursor(0, 1);
                lcd.print("Access granted!"); // если пароль совпал
                tone(A1, 1000, 300); // Делаем звуковой сигнал, Открытие
                digitalWrite(1, LOW);      
                digitalWrite(0, LOW);      
                servo.write(-90); // Поворачиваем серву 
                delay(6000); // 
                if (Serial.println(digitalRead(1))){
                  tone(5, 500, 500);
                  //delay(3000);  // Делаем звуковой сигнал,  Закрытие
                  servo.write(90);  // устанавливаем серву в закрытое сосотояние
                  digitalWrite(1, HIGH);
                  digitalWrite(0, HIGH);
                }
              }
              else {
                lcd.clear();
                lcd.setCursor(0, 1);
                lcd.print("Access denied!"); // если пароль не верен
                tone(A1, 1000, 1000); // Делаем звуковой сигнал
                 delay(3000);
                 lcd.clear();
                lcd.setCursor(0, 0); 
                lcd.print("Enter password!");
                lcd.setCursor(0, 1); 
                KB.read();
                
              }
            }
      }
}
