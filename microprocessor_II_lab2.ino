#include<Wire.h>
const int MPU_ADDR = 0x68; // Address of GY-521 or MPU-6050
const int X_pin = 0; // analog pin connected to X output
const int Y_pin = 1; // analog pin connected to Y output
const int buzzer = 52;
int buzzerinfo;
int nb;

int16_t AcX,AcY,AcZ; //variables for gyro

void setup() {
  Wire.begin();
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  Serial.begin(9600);
  pinMode(X_pin, INPUT);
  pinMode(Y_pin, INPUT);
  pinMode(buzzer, OUTPUT);
}

void loop() {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR,14,true);  // request a total of 14 registers
  
  delay(250);

  while(Serial.available())
  {
  buzzerinfo = Serial.read();
  }
  
  if (buzzerinfo == 'b'){
  tone (buzzer, 500, 1000);
  buzzerinfo = nb;
  }

  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  
  if (((900 < analogRead(X_pin) && 450 < analogRead(Y_pin)< 500)) || (AcX < -7000)) 
  Serial.println("r");
  else if (((450 < analogRead(X_pin) < 500 && analogRead(Y_pin)< 200)) || (AcY < -7000))
  Serial.println("u");
  else if (((analogRead(X_pin) < 100 && 450 < analogRead(Y_pin)< 500)) || (7000 < AcX))
  Serial.println("l");
  else if (((450 < analogRead(X_pin) < 550 && 800 < analogRead(Y_pin))) || (7000 < AcY))
  Serial.println("d");
  else
  Serial.println("n");
}
