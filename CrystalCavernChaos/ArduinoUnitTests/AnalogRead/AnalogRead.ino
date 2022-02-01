int const photoDiode{35};

void setup() {
  // put your setup code here, to run once:
  pinMode(photoDiode, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(analogRead(photoDiode));
  delay(500);  
}
