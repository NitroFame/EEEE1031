#include <LiquidCrystal.h>

// Motor Pins
int ENA = 3; 
int ENB = 11;
int IN1 = A2;
int IN2 = 12;
int IN3 = 1;
int IN4 = 13;

// IR Sensors
int irsR = A1;
int irsL = A3;

// Encoder (must use pin 2 or 3 on UNO)
int EP1 = 2;

// Encoder variables
const int pulsePerRev = 20;
float wheelDia = 65; // mm
float wheelCircum = wheelDia * 3.14159;
volatile long pulses_1 = 0;

unsigned long prevInterrupt = 0;
unsigned long PrevTime = 0;
float distTravel_1 = 0;
float dist_check = 0;

// LCD
LiquidCrystal lcd(8,9,4,5,6,7);


// ---------- SETUP ----------
void setup() {

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(irsR, INPUT);
  pinMode(irsL, INPUT);

  pinMode(EP1, INPUT_PULLUP);

  lcd.begin(16, 2);
  lcd.clear();

  attachInterrupt(digitalPinToInterrupt(EP1), Counter, RISING);
}


// ---------- MAIN LOOP ----------
void loop() {

  // Distance update every 0.5 sec
  if (millis() - PrevTime >= 500) {
    noInterrupts();
    distTravel_1 = (pulses_1 * wheelCircum / pulsePerRev) / 10.0 / 3 * 1.1 ;  // in cm
    interrupts();
    PrevTime = millis();
  }

  // ---- LCD Print ----
  lcd.setCursor(0,0);
  lcd.print("Dist:");
  lcd.print(distTravel_1);
  lcd.print("cm     "); // clear old digits

  int L = digitalRead(irsL);
  int R = digitalRead(irsR);

  lcd.setCursor(0,1);
  lcd.print("Time : ");
  lcd.print(millis() / 1000.0);
  lcd.print("s       ");

  // ---- Decision ----
  int spd = 120;
  int t_spd = 255;

  if (distTravel_1 - dist_check > 65)
  {
    STOP();
    delay(2000);
    dist_check = distTravel_1;
  }



  if (L == 0 && R == 0) 
  {
    Forward(spd); // both see white 
  }
  else if (L == 1 && R == 1) 
  {
    Forward(spd);
    delayMicroseconds(3000);
    STOP(); // both see black
  }
  else if (L == 1 && R == 0)
  { // only the left one sees black
    while (true)
    {
      L=digitalRead(irsL);
      R=digitalRead(irsR);
      Left(t_spd);
      if (!(L==1 && R ==0)) 
      {
        break;
      }
    }
  }   
  else if (L == 0 && R == 1)
  { // only the right one sees black
  while (true)
    {
      L=digitalRead(irsL);
      R=digitalRead(irsR);
      Right(t_spd);
      if (!(L==0 && R ==1)) 
      {
        break;
      }
    }
  }
  else
  {
    STOP();
  } 
}



// ---------- FUNCTIONS ----------

void Counter() {
  unsigned long RIGHT_NOW = micros();
  if (RIGHT_NOW - prevInterrupt > 50){  
    pulses_1++;
    prevInterrupt = RIGHT_NOW;
  }
}


// Movement Functions
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

void Back(int spd){
  digitalWrite(IN1, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN4, HIGH);
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