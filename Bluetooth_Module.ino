#include <LiquidCrystal.h>

#define FORWARD 'F'
#define BACKWARD 'B'
#define LEFT 'L'
#define RIGHT 'R'
#define CIRCLE 'C'
#define CROSS 'X'
#define TRIANGLE 'T'
#define SQUARE 'S'
#define START 'A'
#define PAUSE 'P'

LiquidCrystal LCD(8, 9, 4, 5, 6, 7);

int ENA = 3; 
int ENB = 11;
int IN1 = A5;
int IN2 = A1;
int IN3 = 12;
int IN4 = 13;

String statusText = "Stopped";

void setup() {
  LCD.begin(16, 2);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  Serial.begin(9600);
}


void loop() {

  if (Serial.available()) {
    char command = Serial.read();

    // Only accept valid commands
    if (command=='F' || command=='B' || command=='L' || command=='R' ||
        command=='C' || command=='S' || command=='X' || command=='T' ||
        command=='A' || command=='P') 
    {
      BL(command);              // Run movement
      updateLCD();              // Update LCD safely
    }
    // Ignore all other characters (fixes your problem)
  }

  // Time update
  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate >= 500) {
    LCD.setCursor(0, 0);
    LCD.print("Time: ");
    LCD.print(millis() / 1000);
    LCD.print(" s   ");
    lastUpdate = millis();
  }
}

void updateLCD() {
  LCD.setCursor(0, 1);
  LCD.print("Move: ");
  LCD.print(statusText);
}

void BL(char command) {
  switch (command) {
    case FORWARD:
      statusText = "Forward   ";
      Forward(200);
      break;
    case TRIANGLE:
      statusText = "Forward   ";
      Forward(200);
      break;
    case CROSS:
      statusText = "Backward  ";
      Back(200);
      break;
    case BACKWARD:
      statusText = "Backward  ";
      Back(200);
      break;
    case RIGHT:
      statusText = "Right     ";
      Right(200);
      break;
    case CIRCLE:
      statusText = "Right     ";
      Right(200);
      break;
    case LEFT:
      statusText = "Left      ";
      Left(200);
      break;
    case SQUARE:
      statusText = "Left      ";
      Left(200);
      break;
    default:
      statusText = "Stopped   ";
      STOP();
      break;
  }
}

void Forward(int spd){
  digitalWrite(IN1, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN4, HIGH);
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

void Back(int spd){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, spd);
  analogWrite(ENB, spd);
}

void Left(int spd){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, spd);
  analogWrite(ENB, spd);
}

void Right(int spd){
  digitalWrite(IN1, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, spd);
  analogWrite(ENB, spd);
}
