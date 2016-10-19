//button
const int buttonPin = 12;
int buttonState = 1;
int start = 0;

//for HC-SR04
const int trigRight = 2;  //right
const int echoRight = 3;
const int trigLeft = 4;  //left
const int echoLeft = 5;
const int trigForward = 7;  //forward
const int echoForward = 6;

//for L298N
const int motorIn1 = 8;
const int motorIn2 = 9;
const int motorIn3 = 10;
const int motorIn4 = 11;

const int timeInterval = 200;
long timeCount = 1000;
float durationRight = 0, distanceRight = 0;
float durationLeft = 0, distanceLeft = 0;
float durationForward = 0, distanceForward = 0;
float lastDistanceRight = 0, lastDistanceLeft = 0, lastDistanceForward = 0; 
float x;
int checkStock = 0;

//for test
char cmd;
boolean run = true;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(trigRight, OUTPUT);
  pinMode(echoRight, INPUT);
  pinMode(trigLeft, OUTPUT);
  pinMode(echoLeft, INPUT);
  pinMode(trigForward, OUTPUT);
  pinMode(echoForward, INPUT);
  pinMode(motorIn1, OUTPUT);
  pinMode(motorIn2, OUTPUT);
  pinMode(motorIn3, OUTPUT);
  pinMode(motorIn4, OUTPUT);
  pinMode(buttonPin, INPUT);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  lastDistanceRight = distanceRight;
  lastDistanceLeft = distanceLeft; 
  lastDistanceForward = distanceForward; 
  
  buttonState = digitalRead(buttonPin);
  if(buttonState == 0) start=1;
  if(buttonState == 1) start=0;
  
  if(start == 1){  
    trigAndEcho();

    if (Serial.available() > 0) {
    cmd = Serial.read();
    
      switch (cmd) {
        case 'f':    // 接收到'w'，前進。
          forward();
          run = true; // 啟動馬達
          break;
        case 'b':   // 接收到'x'，後退。
          backward();
          run = true; // 啟動馬達
          break;
        case 'l':   // 接收到'a'，左轉。
          turnLeft();
          run = true; // 啟動馬達
          break;
        case 'r':   // 接收到'd'，右轉。
          turnRight();
          run = true; // 啟動馬達
          break;
        case 'a':
          turnAround();
          delay(650);
          run = true;  // 停止馬達
          break;
        case 's':
          motorStop();
          run = false;  // 停止馬達
          break;
      }
    }
    /*
    if(run == true){
      for(int i=0;i<5;++i){
        forward();
        deadEndCheck();
        SerialMonitor();
        timeCount = timeCount + timeInterval;
        delay(timeInterval);
      }
    }
    */
    motorStop();
  }
}

void deadEndCheck(){
  if(distanceRight < 30.0 &&  distanceLeft < 30.0 && distanceForward < 30.0 ){
    motorStop();
    delay(200);
    turnAround();    
    delay(650);
    motorStop();
    delay(200);
    forward();
  }
}

void driveDirection(){
  if(distanceRight > distanceLeft) x = distanceRight - distanceLeft;
    else x = distanceLeft - distanceRight;
    if(x < 1.0 ) forward();
    else if( distanceRight > distanceLeft ){
      turnRight();
      delay(x*2*timeInterval);
      forward();
    }
    else if(distanceRight < distanceLeft ){
      turnLeft();
      delay(x*2*timeInterval);
      forward();
    }
}

void stockCheck(){
  float tmp1 = lastDistanceRight - distanceRight;
  float tmp2 = lastDistanceLeft - distanceLeft;
  float tmp3 = lastDistanceForward - distanceForward;
  if((tmp1 >= -0.01 && tmp1 <= 0.01) && (tmp2 >= -0.01 && tmp2 <= 0.01) && (tmp3 >= -0.01 && tmp3 <= 0.01))
    checkStock += timeInterval;
  else checkStock = 0;
  if(checkStock >= 1000){
    backward();
    delay(1000);
  }
}

void trigAndEcho(){
  digitalWrite(trigRight, HIGH);
  delayMicroseconds(1000);
  digitalWrite(trigRight, LOW);
  durationRight = pulseIn(echoRight, HIGH);
  distanceRight = (durationRight/2)/29;

  digitalWrite(trigLeft, HIGH);
  delayMicroseconds(1000);
  digitalWrite(trigLeft, LOW);
  durationLeft = pulseIn(echoLeft, HIGH);
  distanceLeft = (durationLeft/2)/29;

  digitalWrite(trigForward, HIGH);
  delayMicroseconds(1000);
  digitalWrite(trigForward, LOW);
  durationForward = pulseIn(echoForward, HIGH);
  distanceForward = (durationForward/2)/29;

}

void SerialMonitor(){
  
//  Serial.print(timeCount/1000);
  Serial.print(" Right = ");
  Serial.print(distanceRight);
  Serial.print(" cm  ;");

  Serial.print(", Left = ");
  Serial.print(distanceLeft);
  Serial.print(" cm");
    
  Serial.print(", Forward = ");
  Serial.print(distanceForward);
  Serial.println(" cm");
}

void motorStop(){
  digitalWrite(motorIn1, LOW);
  digitalWrite(motorIn2, LOW);
  digitalWrite(motorIn3, LOW);
  digitalWrite(motorIn4, LOW);
}

void forward(){
  digitalWrite(motorIn1, LOW);
  digitalWrite(motorIn2, HIGH);
  digitalWrite(motorIn3, HIGH);
  digitalWrite(motorIn4, LOW);
}

void backward(){
  digitalWrite(motorIn1, HIGH);
  digitalWrite(motorIn2, LOW);
  digitalWrite(motorIn3, LOW);
  digitalWrite(motorIn4, HIGH);
}

void turnLeft(){
  digitalWrite(motorIn1, LOW);
  digitalWrite(motorIn2, HIGH);
  digitalWrite(motorIn3, LOW);
  digitalWrite(motorIn4, LOW);
}

void turnRight(){
  digitalWrite(motorIn1, LOW);
  digitalWrite(motorIn2, LOW);
  digitalWrite(motorIn3, HIGH);
  digitalWrite(motorIn4, LOW);
}

void turnAround(){
  digitalWrite(motorIn1, HIGH);
  digitalWrite(motorIn2, LOW);
  digitalWrite(motorIn3, HIGH);
  digitalWrite(motorIn4, LOW);
}
