#include <ESP32Servo.h>
 
Servo myservo;  // create servo object to control a servo
                // 16 servo objects can be created on the ESP32
 
int pos = 0; // default position    
                
int servoPin = 33;  // variable to store the servo position
                    // recommended PWM GPIO pins on the ESP32 include 2,4,12-19,21-23,25-27,32-33 



String input = "";
 
void setup() {
  
  // Allow allocation of all timers
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50); // standard 50 hz servo

  Serial.begin(9600);
  attachServo();
  
}

void turnServo() {
  myservo.write(0);
  delay(180);
}

void resetServo() {
  myservo.write(125);
  delay(300);
}

void attachServo() {
  myservo.attach(servoPin, 500, 2400);
}

void detachServo() {
  myservo.detach();
}
 
void loop() {
  while(!Serial.available()) {delay(100);}
  input = Serial.readStringUntil('\n');
  if (input == "on") {
    Serial.println("Received: on!");
    turnServo();
  } else if (input == "off") {
    Serial.println("Received: off!");
    resetServo();
  } else if (input == "detach") {
    Serial.println("Received: detach!");
    detachServo();
  } else if (input == "attach") {
    Serial.println("Received: attach!");
    // attaches the servo on pin 12 to the servo object
    // using default min/max of 1000us and 2000us
    // different servos may require different min/max settings
    // for an accurate 0 to 180 sweep
    attachServo();                   
  }
}
