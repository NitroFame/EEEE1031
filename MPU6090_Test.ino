#include <Wire.h>
#include <MPU6050.h>
#include <LiquidCrystal.h>

MPU6050 mpu;
LiquidCrystal lcd(8,9,4,5,6,7);

void setup() {
  lcd.begin(16, 2);
  lcd.clear();

  Wire.begin();
  mpu.initialize();

  if (!mpu.testConnection()) {
    lcd.print("MPU not connected!");
    while(1);
  }
}

float verifyroll(float Ax, float Az) {
  float r1 = atan2(-Ax, Az) * -180.0 / PI;
  delayMicroseconds(100);
  float r2 = atan2(-Ax, Az) * -180.0 / PI;
  return (fabs(r1 - r2) < 0.5) ? r1 : 0;
}

void loop() {
  int16_t ax, az;
  mpu.getAcceleration(&ax, NULL, &az);

  float Ax = ax / 16384.0;
  float Az = az / 16384.0;

  float roll = verifyroll(Ax, Az);

  lcd.clear();
  lcd.print("Roll: ");
  lcd.print(roll);
  delay(100);
}
