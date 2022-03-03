
const int irLed{13};
const int photoDiodeLed{35};


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(irLed, OUTPUT);
  digitalWrite(irLed, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(analogRead(photoDiodeLed));
}
