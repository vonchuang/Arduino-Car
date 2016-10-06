const int trig = 5;
const int echo = 6;
const int timeInterval = 200;
int timeCount = 1000;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  float duration, distance;
  digitalWrite(trig, HIGH);
  delayMicroseconds(1000);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);
  distance = (duration/2)/29;
  if(distance < 500){
    Serial.print("Data:");
    Serial.print(timeCount/1000);
    Serial.print(", distance = ");
    Serial.print(distance);
    Serial.println(" cm");
  }
  timeCount = timeCount + timeInterval;
  delay(timeInterval);
}
