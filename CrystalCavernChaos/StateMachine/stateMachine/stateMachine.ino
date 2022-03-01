#include <ESP32Servo.h>
#include <LiquidCrystal_I2C.h>
#define TRIGGER_DEDUCTION 100000

const int numServos{7};
const int servoList[numServos]{12, 14, 25, 26, 27, 32, 33};
const int paddleList[numServos]{2, 4, 5, 15, 16, 17, 18};
bool beenPressed[numServos]{false, false, false, false, false, false, false};
int numPenalties{0};

const int irLed{13};
const int photoDiodeLed{5};
const int startStop{19};

unsigned long startMillis = 0;
unsigned long endMillis = 0;

const int lcdColumns = 16;
const int lcdRows = 2;
// SDA pin will be default: 22
// SDC pin will be default: 21

// Instance must be created... common port connection seems to be 0x27
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  

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

  // initialize LCD and backlight
  lcd.init();
  lcd.backlight();

  Serial.println("-----------------Setup Complete-------------------");
}

void printToRow(int pos, const String& text) {
  static String previousText = "foo";

  if (previousText == text) {
    // They're trying to reprint what's already been printed so don't do anyting!!!
    Serial.print("already printed: ");
    Serial.println(text);
    return;
  }

  if (text.length() < 0 || text.length() > 16) {
    Serial.println("Text TOO Large!!!");
    previousText = text;
    return;
  } else {
    Serial.print("Valid print: ");
    Serial.println(text);
    lcd.setCursor(0,pos);
    Serial.println(previousText);
    for (int i = 0; i < text.length(); ++i) {
      if (previousText[i] == text[i]) {
        // They're trying to reprint what's already been printed so don't do anyting!!!
        Serial.println(String(previousText[i])+String(text[i]));
        continue;
      } else {
        lcd.setCursor(i,pos);
        lcd.print(text[i]);
      }
    }
    for (int i = text.length(); i < 16; ++i) {
      lcd.setCursor(i, pos);
      lcd.print(' ');
    }
  }

  previousText = text;
}

void printScreen(const String& top, const String& bottom) {
  if (top == "") {}
  else
    printToRow(0, top);

  if (bottom == "") {}
  else
    printToRow(1, bottom);
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

int getTime() {
  unsigned long currTime = millis() - startMillis;
  if (currTime > 999999) {
    return 999999;
  } else {
    return currTime;
  }
}

int getTotalTime() {
  unsigned long totalTime = endMillis - startMillis;
  if (totalTime > 999999) {
    return 999999;
  } else {
    return totalTime;
  }
}

int getScore() {
  unsigned long totalTime = getTotalTime();
  return totalTime - TRIGGER_DEDUCTION * numPenalties;
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
  printScreen("Starting", "Momentarily!");
  startMillis = millis();
}

bool waitForRobot() {
  // Debugging Code:
  Serial.println("waitForRobot");
  
  // state machine code
  bool triggeredDiode{false};
  bool maxPenalties{false};
  
  while (true) {
    printScreen("Time: " + String(getTime()), "Penalties: " + String(numPenalties));

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
  printScreen("Win Time: " + getTotalTime(), "Score:    " + getScore());

  // state machine code
  while (!checkStart()) {delay(100);}
  delay(500);// this will make sure a second button press by the user isn't accidentally recognised twice, restarting the game.
}

void lost() {
  // Debugging Code:
  Serial.println("lost");
  printScreen("Disqualified by", "3 penalties");

  // state machine code
  while (!checkStart()) {delay(100);}
  delay(500);// this will make sure a second button press by the user isn't accidentally recognised twice, restarting the game.
  
}

void prep() {
  // Debugging Code:
  Serial.println("prep");

  numPenalties = 0;

  // reset all servos so rubble can be reloaded
  for (int i = 0; i < numServos; ++i) {
    beenPressed[i] = false;
    attachServo(servoList[i]);
    resetServo();
    detachServo();
  }

  printScreen("Replace", "Rubble!");
  
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
