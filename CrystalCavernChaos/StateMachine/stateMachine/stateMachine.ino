#include <ESP32Servo.h>

const int numServos{7};
const int servoList[numServos]{12, 14, 25, 26, 27, 32, 33};
const int paddleList[numServos]{2, 4, 5, 15, 16, 17, 18};
bool beenPressed[numServos]{false, false, false, false, false, false, false};

const int irLed{13};
const int photoDiodeLed{5};
const int startStop{21};

unsigned long startMillis = 0;
unsigned long endMillis = 0;

Servo myservo;

void setup() {
  
  // serial communication initialization
  Serial.begin(9600);

  // initialize paddles
  for (int i = 0; i < numServos; ++i) {
    pinMode(paddleList[i], INPUT);
    //Serial.print("Servo "); Serial.print(i); Serial.print(" is "); Serial.println(digitalRead(paddleList[i]));
  }

  // initialize start/stop button
  pinMode(startStop, INPUT);

  // esp motor pwm initialization
  // Allow allocation of all timers
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50); // standard 50 hz servo

  Serial.println("-----------------Setup Complete-------------------");
}

void turnServo() {
  myservo.write(80);
  delay(300);
}

void resetServo() {
  myservo.write(0);
  delay(300);
}

void attachServo(int servoPin) {
  myservo.attach(servoPin, 500, 2400);
}

void detachServo() {
  myservo.detach();
}

bool checkPhotoDiode() {
  // Debugging Code:
  //Serial.print("  --");
  //Serial.println("checkPhotoDiode()");
  // state machine code:
  return false;
}

bool checkPenalties(int& numPenalties) {
  // Debugging Code:
  //Serial.print("  --");
  //Serial.println("checkPenalties()");
  // state machine code:
  if (numPenalties > 2) {
    return true;
  } else {
    return false;
  }
}

void checkPaddles(int& numPenalties, const int (&paddleList)[numServos], const int (&servolist)[numServos]) {
  // Debugging Code:
  //Serial.print("  --");
  //Serial.println("checkPaddles()");
  
  // state machine code
  for (int i = 0; i < numServos; i++) {
    if (digitalRead(paddleList[i]) == HIGH && beenPressed[i] == false) {
      digitalWrite(servoList[i], HIGH);
      Serial.print("\tButton ");
      Serial.print(i);
      Serial.println(" pressed");
      attachServo(servoList[i]);
      turnServo();
      detachServo();
      beenPressed[i] = true;
      numPenalties++;
    }
  }
}

bool checkStart() {
  return digitalRead(startStop);
}

void beginGame() {
  // Debugging Code:
  Serial.println("beginGame");
  startMillis = millis();
}

bool waitForRobot() {
  // Debugging Code:
  Serial.println("waitForRobot");
  
  // state machine code
  bool triggeredDiode{false};
  int numPenalties{0};
  bool maxPenalties{false};
  
  while (true) {


    if (checkStart() != 0) {
      return false;
    }
  
    triggeredDiode = checkPhotoDiode();
    checkPaddles(numPenalties, paddleList, servoList);
    maxPenalties = checkPenalties(numPenalties);
    
    if (triggeredDiode) {
      endMillis = millis();
      return true; // signifies that we should do the reset, so that's why we store waitForRobot() return value in variable doReset in main loop
    }
    else if (maxPenalties) {
      endMillis = millis();
      return false;
    }
  }
}

void won() {
  // Debugging Code:
  Serial.println("won");

  // state machine code
  while (!checkStart()) {delay(100);}
  delay(500);// this will make sure a second button press by the user isn't accidentally recognised twice, restarting the game.
}

void lost() {
  // Debugging Code:
  Serial.println("lost");

  // state machine code
  while (!checkStart()) {delay(100);}
  delay(500);// this will make sure a second button press by the user isn't accidentally recognised twice, restarting the game.
  
}

void prep() {
  // Debugging Code:
  Serial.println("prep");

  // reset all servos so rubble can be reloaded
  for (int i = 0; i < numServos; ++i) {
    beenPressed[i] = false;
    attachServo(servoList[i]);
    resetServo();
    detachServo();
  }
  
  // state machine code
  while (!checkStart()) {delay(100);}
  delay(500);// this will make sure a second button press by the user isn't accidentally recognised as a reset during the waitForRobot();
}

void loop() {
  // put your main code here, to run repeatedly:
  // static int numPenalties{0}; <-- not sure why this line was necesary to begin with... ask Michael what the thought process was behind the static variable
  beginGame();
  //delay(500);
  bool doReset{waitForRobot()};
  //delay(500);
  if (doReset) {
    won();
    //delay(500);
  } else {
    lost();
    Serial.println("---------------------");
    //delay(500);
  }
  prep();
  //delay(500);
}
