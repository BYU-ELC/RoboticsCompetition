int const photoDiode{35};
int const irLED{13};

int timePrevious{};
int timeCurrent{};
int timeDiff{1000};

bool ledState{true};

void setup() {
  // put your setup code here, to run once:
  pinMode(photoDiode, INPUT);
  pinMode(irLED, OUTPUT);
  Serial.begin(9600);
  timePrevious = millis();
}

void loop() {
  // put your main code here, to run repeatedly:  
  timeCurrent = millis();
  if (timeCurrent-timePrevious >= timeDiff) {
    if (ledState) {
      digitalWrite(irLED, LOW);
      ledState = false;
    }
    else {
      digitalWrite(irLED, HIGH);
      ledState = true;      
    }
    timePrevious = millis();
  }
  Serial.println(analogRead(photoDiode));
  delay(200);  
}
