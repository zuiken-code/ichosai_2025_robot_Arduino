#include <Wire.h>

const int leftMotorPin = 3;    // PWMピン
const int rightMotorPin = 5;
const int potPin = A0;         // 可変抵抗を接続するアナログピン

unsigned long lastReceiveTime = 0;  
const unsigned long timeout = 1000; // 1秒

float maxScale = 1.5;  // 最大倍率（変数で変更可）

void setup() {
  Wire.begin(0x8);  

  pinMode(leftMotorPin, OUTPUT);
  pinMode(rightMotorPin, OUTPUT);

  Wire.onReceive(receiveEvent);

  // 初期状態は停止
  analogWrite(leftMotorPin, 0);
  analogWrite(rightMotorPin, 0);
}

void loop() {
  unsigned long now = millis();

  // フェールセーフ：一定時間信号が来なかったら停止
  if (now - lastReceiveTime > timeout) {
    analogWrite(leftMotorPin, 0);
    analogWrite(rightMotorPin, 0);
  }

  delay(10);
}

void receiveEvent(int numBytes) {
  if (numBytes >= 3) {
    int cmd = Wire.read();
    int left_pwm = Wire.read();
    int right_pwm = Wire.read();

    // 可変抵抗の値を読み取る (0〜1023)
    int potValue = analogRead(potPin);

    // 未接続 or ほぼ0なら1.0倍固定
    float scale;
    if (potValue < 10) {  // 閾値は環境に合わせて調整
      scale = 1.0;
    } else {
      // 0〜1023 → 1.0〜maxScale にマッピング
      scale = 1.0 + ( (float)potValue / 1023.0 ) * (maxScale - 1.0);
    }

    // PWM値に倍率をかける（255を超えないように制限）
    int left_scaled = constrain((int)(left_pwm * scale), 0, 255);
    int right_scaled = constrain((int)(right_pwm * scale), 0, 255);

    analogWrite(leftMotorPin, left_scaled);
    analogWrite(rightMotorPin, right_scaled);

    lastReceiveTime = millis();  // 受信時刻を更新
  }
}
