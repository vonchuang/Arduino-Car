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
float durationRight = 0,      durationLeft = 0,     durationForward = 0;
float distanceRight = 0,      distanceLeft = 0,     distanceForward = 0;
float lastDistanceRight = 0,  lastDistanceLeft = 0, lastDistanceForward = 0; 
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
  if(buttonState == 0) start=0;
  if(buttonState == 1) start=1;
  if(start == 1){  
    getDistance();

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
          run = true;  // 停止馬達
          break;
        case 's':
          motorStop();
          run = false;  // 停止馬達
          break;
        case 'x':         
          turnRight();  delay(4.0*timeInterval);
          motorStop();  delay(50);
 /*         forward();    delay(timeInterval);
          motorStop();  delay(50);
          turnLeft();   delay(timeInterval);
          forward();    delay(timeInterval);
          motorStop();  delay(50);
*/          run = false;  // 停止馬達
          break;
        case 'z':         
          for(int i=0;i<10;++i){
            getDistance();
            driveDirectionCheck();
            forward();           
//            SerialMonitor();
            timeCount = timeCount + timeInterval;
            delay(timeInterval/2);
          }
          run = false;  // 停止馬達
          break;     
      }
    }
    /*
    if(run == true){
      for(int i=0;i<5;++i){
        forward();
        turnCheck();
        SerialMonitor();
        timeCount = timeCount + timeInterval;
        delay(timeInterval);
      }
    }
    
    forward();
    deadEndCheck();
    turnCheck();
    stockCheck();
 //   driveCenterCheck();
//    driveDirectionCheck();
    SerialMonitor();
*/
    timeCount = timeCount + timeInterval;
    delay(timeInterval);    
    motorStop();
  }
  else motorStop();
}


void deadEndCheck(){
  if(distanceRight < 15.0 &&  distanceLeft < 15.0 && distanceForward < 22.0 ){
    Serial.println("dead end");
    motorStop();         delay(200);
    turnAround();        delay(625);
    motorStop();         delay(200);
    forward();           delay(500);
    motorStop();
  }
}

void turnCheck(){
  float right = distanceRight - lastDistanceRight;
  float left = distanceLeft - lastDistanceLeft;
  float center = distanceForward - lastDistanceForward;
  
  if(distanceLeft > 30.0 && distanceRight < 30.0 && distanceForward < 30.0){    //turn left
    turnLeft();  delay(3.2*timeInterval);
    motorStop();  delay(50);
    forward();    delay(2.5*timeInterval);
    motorStop();  delay(50);        
    Serial.println("turn left");
  }
  if(distanceLeft < 30.0 && distanceRight < 30.0 && distanceForward > 30.0){   //forward
      
  }
  if(distanceLeft < 30.0 && right > 30.0 && distanceForward < 30.0){   //turn right    
    turnRight();  delay(2.2*timeInterval);
    motorStop();  delay(50);
    forward();    delay(2.5*timeInterval);
    motorStop();  delay(50); 
    Serial.println("turn right");      
  }  
  if(distanceLeft < 30.0 && distanceRight < 30.0 && distanceForward > 30.0){   //left and forward -> turn left
    turnLeft();  delay(3.2*timeInterval);
    motorStop();  delay(50);
    forward();    delay(2.5*timeInterval);
    motorStop();  delay(50);        
    Serial.println("turn left");
  }
  if(distanceLeft < 30.0 && distanceRight < 30.0 && distanceForward > 30.0){   //forward and right -> forward
      
  }
  if(distanceLeft < 30.0 && distanceRight < 30.0 && distanceForward > 30.0){   //left anf right -> turn left
    turnLeft();  delay(3.2*timeInterval);
    motorStop();  delay(50);
    forward();    delay(2.5*timeInterval);
    motorStop();  delay(50);        
    Serial.println("turn left");
  }
}

//TODO
void driveDirectionCheck(){ 
  float x = distanceRight - distanceLeft;
  Serial.println(x);
  if(distanceRight < 30.0 && distanceLeft < 30.0){
    if(x > 5.0){
      turnLeft();  delay(timeInterval);
      motorStop();  delay(50);
      Serial.println("turn right");
    }else if(x < -5.0){
      turnRight();  delay(1.2*timeInterval);
      motorStop();  delay(50);
      Serial.println("turn right");
    }
  }
}

void driveCenterCheck(){
    if(distanceRight > distanceLeft && distanceLeft < 4.0 && distanceForward > 30.0){
        turnRight();  delay(timeInterval);
        motorStop();  delay(50);
        forward();    delay(timeInterval);
        motorStop();  delay(50);
        turnLeft();   delay(timeInterval);
        forward();    delay(timeInterval);
        motorStop();  delay(50);
    }else if(distanceRight < distanceLeft && distanceRight < 4.0 && distanceForward > 30.0){
        turnLeft();  delay(1.2*timeInterval);
        motorStop();  delay(50);
        forward();    delay(timeInterval);
        motorStop();  delay(50);
        turnRight();   delay(0.7*timeInterval);
        forward();    delay(timeInterval);
        motorStop();  delay(50);
    }
  
}

void stockCheck(){
  float right = lastDistanceRight - distanceRight;
  float left = lastDistanceLeft - distanceLeft;
  float forward = lastDistanceForward - distanceForward;
  if((right >= -2.0 && right <= 2.0) && (left >= -2.0 && left <= 2.0) && (forward >= -2.0 && forward <= 2.0))
    checkStock += timeInterval;
  else checkStock = 0;
  if(checkStock >= 1000){
    Serial.println("stock");
    backward();
    delay(400);
  }
}

void getDistance(){ 
  digitalWrite(trigRight, LOW);  delayMicroseconds(2);
  digitalWrite(trigRight, HIGH);  delayMicroseconds(10);
  digitalWrite(trigRight, LOW);
  durationRight = pulseIn(echoRight, HIGH);
  distanceRight = durationRight/58.2;
  if(distanceRight > 3000.0)  distanceRight = lastDistanceRight;

  digitalWrite(trigLeft, LOW); delayMicroseconds(2);
  digitalWrite(trigLeft, HIGH); delayMicroseconds(10);
  digitalWrite(trigLeft, LOW);
  durationLeft = pulseIn(echoLeft, HIGH);
  distanceLeft = durationLeft/58.2;
  if(distanceLeft > 3000.0)  distanceLeft = lastDistanceLeft;
  
  digitalWrite(trigForward, LOW);  delayMicroseconds(2);
  digitalWrite(trigForward, HIGH);  delayMicroseconds(10);
  digitalWrite(trigForward, LOW);
  durationForward = pulseIn(echoForward, HIGH);
  distanceForward = durationForward/58.2;
  if(distanceForward > 3000.0)  distanceForward = lastDistanceForward;
}

void SerialMonitor(){
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
  analogWrite(motorIn1, 0);
  analogWrite(motorIn2, 0);
  analogWrite(motorIn3, 0);
  analogWrite(motorIn4, 0);
}

void forward(){
  analogWrite(motorIn1, 0);
  analogWrite(motorIn2, 200);
  analogWrite(motorIn3, 200);
  analogWrite(motorIn4, 0);
}

void backward(){
  analogWrite(motorIn1, 200);
  analogWrite(motorIn2, 0);
  analogWrite(motorIn3, 0);
  analogWrite(motorIn4, 200);
}

void turnRight(){
  analogWrite(motorIn1, 0);
  analogWrite(motorIn2, 255);
  analogWrite(motorIn3, 0);
  analogWrite(motorIn4, 0);
}

void turnLeft(){
  analogWrite(motorIn1, 0);
  analogWrite(motorIn2, 0);
  analogWrite(motorIn3, 255);
  analogWrite(motorIn4, 0);
}

void turnAround(){
  analogWrite(motorIn1, 200);
  analogWrite(motorIn2, 0);
  analogWrite(motorIn3, 200);
  analogWrite(motorIn4, 0);
}
