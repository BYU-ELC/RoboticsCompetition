const int numServos{7};
const int servoList[numServos]{12, 14, 25, 26, 27, 32, 33};
const int buttonList[numServos]{2, 4, 5, 15, 16, 17, 18};

const int irLed{13};
const int photoDiodeLed{5};
const int startStop{0};

void setup() {
  Serial.begin(9600);
  Serial.println("Connecting Servos");
  
}

void loop() {
  // put your main code here, to run repeatedly:
  static int numPenalties{0};
  //begin();
  //waitForRobot(); --> return decision for reset or lost
  //reset() / lost();
  //prep();
}
