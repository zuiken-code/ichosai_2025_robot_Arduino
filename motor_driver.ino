#include <Wire.h>

const int leftMotorPin = 3;   // PWMピン
const int rightMotorPin = 5;

void setup() {
  Wire.begin(0x8);  // ArduinoのI2Cアドレス

  pinMode(leftMotorPin, OUTPUT);
  pinMode(rightMotorPin, OUTPUT);

  Wire.onReceive(receiveEvent);
}

void loop() {
  delay(10);  // メイン処理はなし
}

void receiveEvent(int numBytes) {
  int cmd = Wire.read();
  int left_pwm = Wire.read();
  int right_pwm = Wire.read();

  analogWrite(leftMotorPin, left_pwm);
  analogWrite(rightMotorPin, right_pwm);
}

