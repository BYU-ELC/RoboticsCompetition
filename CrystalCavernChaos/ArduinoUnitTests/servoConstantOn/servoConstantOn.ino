#include <ESP32Servo.h>

int servoPin = 33;
Servo myservo;

void setup() {
  // put your setup code here, to run once:
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50); // standard 50 hz servo

  Serial.begin(9600);
  myservo.attach(servoPin, 500, 2400);
  myservo.write(125);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Servo on!");
  delay(200);
}
