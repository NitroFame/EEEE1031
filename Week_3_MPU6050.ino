#include <LiquidCrystal.h>
#include <Wire.h>
#include <MPU6050.h>

// Motor Pins
int ENA = 3; 
int ENB = 11;
int IN1 = A2;
int IN2 = 12;
int IN3 = 1;
int IN4 = 13;

int irsR = A1;
int irsL = A3;


// LCD
LiquidCrystal lcd(8,9,4,5,6,7); // RS, E, D4, D5, D6, D7 (no need declaration for pins in LCD, due to shield)
bool onslope = false;

//MPU6050
MPU6050 mpu;

// Declaring the PINS
void setup(){

  pinMode(ENA,OUTPUT);
  pinMode(ENB,OUTPUT);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);

  lcd.begin(16, 2);
  lcd.clear();
  
  Serial.begin(9600);
  Wire.begin();
  mpu.initialize();

  if (!mpu.testConnection()) {
    lcd.print("MPU not connected");
    while (1);
  }

}



unsigned long prevTime = 0;
const int interval = 100;
float roll = 0;
float pitch = 0;
float slope = 0;

// State flags
bool onSlope = false;
bool slopeSaved = false;
bool reachedTop = false;

void loop() {
    unsigned long currentTime = millis();

    Forward(255);

    // Read MPU every 100 ms
    if (currentTime - prevTime >= interval) {
        prevTime = currentTime;

        int16_t ax, ay, az;
        mpu.getAcceleration(&ax, &ay, &az);

        float Ax = ax / 16384.0;
        float Az = az / 16384.0;

        roll = verifyroll(Ax, Az);
          

        if (onSlope == false && roll > 15 && roll < 50) {
            onSlope = true;
            slope = roll;
            slopeSaved = true;
        }

        if (onSlope == true && roll < 5) 
        {
          delay(500);
          STOP();
          delay(4000); 
          spin360(255); 
          reachedTop = true;
          onSlope = false; 
        }


        if (slopeSaved && reachedTop) {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Slope Angle:");
            lcd.setCursor(0, 1);
            lcd.print(slope, 1);
            lcd.print(" deg");
            delay(10000);


            slopeSaved = false;
            reachedTop = false;
        }
    }
}




// Functions

float verifyroll(float Ax, float Az) {
  float v1roll = (atan2(-Ax, Az) * -180.0 / PI);
  delayMicroseconds(100);
  float v2roll = (atan2(-Ax, Az) * -180.0 / PI);
  return (fabs(v1roll - v2roll) < 0.5) ? v1roll : 0;
}


void spin360(int spd){ 
  Right(spd); 
  delay(3050); 
  STOP(); }


void Forward(int spd){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, spd);
  analogWrite(ENB, spd-20);
}

void STOP(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void Right(int spd) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, spd);
  analogWrite(ENB, spd);
}