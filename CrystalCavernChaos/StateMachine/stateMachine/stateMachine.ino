const int numServos{7};
const int servoList[numServos]{12, 14, 25, 26, 27, 32, 33};
const int paddleList[numServos]{2, 4, 5, 15, 16, 17, 18};
const bool beenPressed[numServos]{false, false, false, false, false, false, false};

const int irLed{13};
const int photoDiodeLed{5};
const int startStop{0};

void setup() {
  Serial.begin(9600);
  Serial.println("In setup: Connecting Servos");

  for (int i = 0; i < numServos; ++i) {
    pinMode(paddleList[i], INPUT);
    Serial.print("Servo "); Serial.print(i); Serial.print(" is "); Serial.println(digitalRead(paddleList[i]));
  }

  Serial.println("-----------------Setup Complete-------------------");
}

bool checkPhotoDiode() {
  // Debugging Code:
  Serial.print("  --");
  Serial.println("checkPhotoDiode()");
  // state machine code:
  return false;
}

bool checkPenalties(int& numPenalties) {
  // Debugging Code:
  Serial.print("  --");
  Serial.println("checkPenalties()");
  // state machine code:
  if (numPenalties > 2) {
    return true;
  } else {
    return false;
  }
}

void checkPaddles(int& numPenalties, const int (&paddleList)[numServos], const int (&servolist)[numServos]) {
  // Debugging Code:
  Serial.print("  --");
  Serial.println("checkPaddles()");
  
  // state machine code
  for (int i = 0; i < numServos; i++) {
    if (digitalRead(paddleList[i]) == HIGH && beenPressed[i] == false) {
      digitalWrite(servoList[i], HIGH);
      Serial.print("\tButton ");
      Serial.print(i);
      Serial.println(" pressed");
      numPenalties++;
    }
  }
}

void beginGame() {
  // Debugging Code:
  Serial.println("beginGame");
  
  // state machine code
  
}

bool waitForRobot() {
  // Debugging Code:
  Serial.println("waitForRobot");
  
  // state machine code

  
  
  
  bool triggeredDiode{false};
  int numPenalties{0};
  bool maxPenalties{false};
  
  while (true) {

    triggeredDiode = checkPhotoDiode();
    checkPaddles(numPenalties, paddleList, servoList);
    maxPenalties = checkPenalties(numPenalties);
    
    if (triggeredDiode)
      return true; // signifies that we should do the reset, so that's why we store waitForRobot() return value in variable doReset in main loop
    else if (maxPenalties)
      return false;
  }
}

void won() {
  // Debugging Code:
  Serial.println("waitForRobot");

  // state machine code
  
  
}

void lost() {
  // Debugging Code:
  Serial.println("lost");

  // state machine code
  
}

void prep() {
  // Debugging Code:
  Serial.println("prep");
  
  // state machine code
  
}

void loop() {
  // put your main code here, to run repeatedly:
  // static int numPenalties{0}; <-- not sure why this line was necesary to begin with... ask Michael what the thought process was behind the static variable
  beginGame();
  delay(500);
  bool doReset{waitForRobot()};
  delay(500);
  if (doReset) {
    won();
    delay(500);
  } else {
    lost();
    Serial.println("---------------------");
    delay(500);
  }
  prep();
  delay(500);
}
