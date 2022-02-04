const int numServos{7};
const int servoList[numServos]{12, 14, 25, 26, 27, 32, 33};
const int paddleList[numServos]{2, 4, 5, 15, 16, 17, 18};

const int irLed{13};
const int photoDiodeLed{5};
const int startStop{0};

bool doReset{false};

void setup() {
  Serial.begin(9600);
  Serial.println("In setup: Connecting Servos");
}

void checkPhotoDiode() {
  // Debugging Code:
  
  // state machine code:
  
}

void checkPenalties() {
  // Debugging Code:

  // state machine code:
}

void checkPaddles(int (&paddleList)[numServos], int (&servolist)[numServos]) {
  for (int i = 0; i < numServos; i++) {
    
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

  bool triggeredDiode{false}
  bool threePenalties{false};
  checkPaddles(paddleList, servolist);
  
  while (true) {

    triggeredDiode = checkPhotoDiode();
    threePenalties = checkPenalty();
    
    if (triggeredDiode)
      return true; // signifies that we should do the reset, so that's why we store waitForRobot() return value in variable doReset in main loop
    else if (threePenalties)
      return false;
  }
}

void reset() {
  // Debugging Code:
  Serial.println("waitForRobot");

  // state machine code
  
}

void prep() {
  // Debugging Code:
  Serial.println("prep");
  
  // state machine code
  
}

void loop() {
  // put your main code here, to run repeatedly:
  static int numPenalties{0};
  beginGame();
  doReset = waitForRobot();
  if (doReset)
    reset();
  else
    lost();
  prep();
}
