// Davey Agrinsoni
// Jacqueline Giggi
// Tuan Nguyen
// Microprocessor II Lab 2

#include<Wire.h>
const int MPU_ADDR = 0x68; // Address of GY-521 or MPU-6050
const int X_pin = 0; // analog pin connected to X output
const int Y_pin = 1; // analog pin connected to Y output
const int buzzer = 52; // assigning the output of the buzzer to pin 52
int buzzerinfo; // data from python 
int nb; //no buzzer

int16_t AcX,AcY,AcZ; //variables for gyro/accelerometer

void setup() {
  Wire.begin();
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  Serial.begin(9600);
  pinMode(X_pin, INPUT); //assigning inputs/outputs
  pinMode(Y_pin, INPUT);
  pinMode(buzzer, OUTPUT);
}

void loop() {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR,6,true);  // request a total of 6 registers for the accleration portion
  
  delay(250);

  while(Serial.available())
  {
  buzzerinfo = Serial.read(); // reading input serial monitor for python output 
  }
  
  if (buzzerinfo == 'b'){ // if buzzerinfo is set to b from python then the buzzer will make a sound
  tone (buzzer, 500, 1000);
  buzzerinfo = nb; // sets buzzer to nb to prevent continuous buzzing after initial turn on
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
