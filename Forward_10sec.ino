#include <LiquidCrystal.h>

// Motor Pins
int ENB = 11; 
int ENA = 3;
int IN4 = 13;
int IN3 = 12;
int IN2 = 2;
int IN1 = 1;

int echo = A1;
int trig = A2;

// LCD
LiquidCrystal lcd(8,9,4,5,6,7); 
// RS, E, D4, D5, D6, D7 (no need declaration for pins in LCD, due to shield)

void setup(){

  pinMode(ENA,OUTPUT);
  pinMode(ENB,OUTPUT);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);

  lcd.begin(16, 2);
  lcd.clear();
}

void Forward(int spd){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, spd);
  analogWrite(ENB, spd);
}

void STOP(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void loop(){
  lcd.clear();
  float timer=millis() / 1000.0;
  lcd.print("NOT THE");
  lcd.setCursor(12,0);
  lcd.print(timer);
  lcd.setCursor(0,1);
  lcd.print("PROGRESS TEST!!!");
  if (timer <= 10){
    Forward(200);
  }
  delay(10);
  if (timer > 10){
    STOP();
  }
}
