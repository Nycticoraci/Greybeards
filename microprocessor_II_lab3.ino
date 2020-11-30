// Davey Agrinsoni
// Jacqueline Giggi
// Tuan Nguyen

#include <LiquidCrystal.h>
#include <DS3231.h>
#include <Wire.h>

DS3231 clock; // Needed for RTC module
RTCDateTime dt;
LiquidCrystal lcd(7, 8, 9, 10, 11, 12); // Digital PWM 7-12 Control LCD

const int ENABLE = 5; // Digital PWM 5 enables DC Motor
const int DIRA = 3; // Digital PWM 3 controls Direction A of DC Motor
const int DIRB = 4; // Digital PWM 4 controls Direction B of DC Motor
volatile int SPEEDBUTTON = 19; // Button to change speed
volatile int DIRECTIONBUTTON = 18; // Button to change direction

int initial = 0;
int lcdspeed = 0;
int lcddirection = 0;
int speedstate = 0;
int directionstate = 0;
int currenttime = 0;
int previoustime = 0;

void setup() {
  pinMode(ENABLE, OUTPUT);
  pinMode(DIRA, OUTPUT); //Clockwise (Orientation = Facing DC Motor with Shaft going towards yourself)
  pinMode(DIRB, OUTPUT); //Counter Clockwise
  pinMode(SPEEDBUTTON, INPUT_PULLUP);
  pinMode(DIRECTIONBUTTON, INPUT_PULLUP);

  Serial.begin(9600);

  clock.begin();
  clock.setDateTime(__DATE__, __TIME__);

  lcd.begin(16, 2); //Initial startup display

  digitalWrite(DIRA, HIGH); //Initially set to clockwise starting
  digitalWrite(DIRB, LOW);

  attachInterrupt(digitalPinToInterrupt(SPEEDBUTTON), fanspeed_ISR, RISING); //Initially wanted falling edge but buttons were finicky and this worked more reliably
  attachInterrupt(digitalPinToInterrupt(DIRECTIONBUTTON), fandirection_ISR, RISING); //When the button is released from being pressed the ISR is activated
}

void loop() {
  dt = clock.getDateTime(); //Get time from RTC
  currenttime = millis();

  if (initial == 0) {
    lcd.setCursor(0, 0);
    lcd.print("Speed/Dir ");
    lcd.setCursor(0, 1);
    lcd.print(dt.month);  lcd.print("-");
    lcd.print(dt.day);    lcd.print(" ");
    lcd.print(dt.hour);   lcd.print(":");
    lcd.print(dt.minute); lcd.print(":");
    lcd.print(dt.second); lcd.print("   ");
    lcd.setCursor(10, 0);
    lcd.print("MAX-");
    lcd.setCursor(14, 0);
    lcd.print("C ");
    initial = 1;
  }

  if (currenttime - previoustime >= 1000) { // if 1 second elapses since start then use update_ISR for LCD Screen
    previoustime = currenttime;
    update_ISR();
  }

  if ((0 <= dt.second) && (dt.second < 30)){
    fanrun();
  }
  if (dt.second == 30) {
    analogWrite(ENABLE, 0);
  }

  delay(500);
}


void update_ISR() {
  lcd.setCursor(0, 0);
  lcd.print("Speed/Dir ");
  lcd.setCursor(0, 1);
  lcd.print(dt.month);  lcd.print("-");
  lcd.print(dt.day);    lcd.print(" ");
  lcd.print(dt.hour);   lcd.print(":");
  lcd.print(dt.minute); lcd.print(":");
  lcd.print(dt.second); lcd.print("   ");
  if (speedstate == 0) {
    lcd.setCursor(10, 0);
    lcd.print("MAX-");
  }
  else if (speedstate == 1) {
    lcd.setCursor(10, 0);
    lcd.print("3/4-");
  }
  else if (speedstate == 2) {
    lcd.setCursor(10, 0);
    lcd.print("1/2-");
  }
  else {
    lcd.setCursor(10, 0);
    lcd.print("OFF-");
  }
  if (directionstate == 0) {
    lcd.setCursor(14, 0);
    lcd.print("C ");
  }
  else {
    lcd.setCursor(14, 0);
    lcd.print("CC");
  }
}

void fanspeed_ISR() { // always interrupts when SPEEDBUTTON is pressed to update info
  lcdspeed += 1;
  speedstate += 1;
  if ((speedstate > 3) || (speedstate < 0)) {
    speedstate = 0;
  }
}

void fandirection_ISR() { // always interrupts when DIRECTIONBUTTON is pressed to update info
  lcddirection += 1;
  directionstate += 1;
  if (lcddirection == 1) {
    digitalWrite(DIRA, LOW);
    digitalWrite(DIRB, HIGH);
    lcd.setCursor(14, 0);
    lcd.print("CC");
    directionstate = 1;
  }
  else {
    digitalWrite(DIRA, HIGH);
    digitalWrite(DIRB, LOW);
    lcd.setCursor(14, 0);
    lcd.print("C ");
    directionstate = 0;
    lcddirection = 0;
  }
}

void fanrun() {
  if (lcdspeed == 0) { //turns on fan at second 0 and depends on lcdspeed button to determine speed
    analogWrite(ENABLE, 255); //enable on max speed
  }
  else if (lcdspeed == 1) {
    analogWrite(ENABLE, 191); //enable on 3/4 speed
  }
  else if (lcdspeed == 2) {
    analogWrite(ENABLE, 128); //enable on 1/2 speed
  }
  else if (lcdspeed == 3) {
    analogWrite(ENABLE, 0);
  }
  else {
    analogWrite(ENABLE, 255);
    speedstate = 0;
    lcdspeed = 0;
  }
}
