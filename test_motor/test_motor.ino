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
const int motorIn1 = 8;   //leftback
const int motorIn2 = 9;   //leftforward
const int motorIn3 = 10;  //rightforward
const int motorIn4 = 11;  //rightback

const int timeInterval = 200;
long timeCount = 1000;
float durationRight = 0,      durationLeft = 0,     durationForward = 0;
float distanceRight = 0,      distanceLeft = 0,     distanceForward = 0;
float lastDistanceRight = 0,  lastDistanceLeft = 0, lastDistanceForward = 0; 
float x;
int rightCheck = 0, leftCheck = 0;
int checkStock = 0;
int deadEndFirst = 0, stockFirst = 0, directionLeftFirst = 0, directionRightFirst = 0;
int cntRight = 0, cntLeft = 0;

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
  
  
  buttonState = digitalRead(buttonPin);
  if(buttonState == 0) start=0;
  if(buttonState == 1) start=1;
  if(start == 1){  
    getDistance();  
    forward();
//    deadEndCheck();
//    turnCheck();
    stockCheck();
    deadEndCheck2();
    turnCheck2();
    driveDirectionCheck();
    SerialMonitor();

//    timeCount = timeCount + timeInterval/2;
//    delay(60);    
//    motorStop();
  }
  else{
//    getDistance(); 
 //   SerialMonitor();
    motorStop();
  }
}


void deadEndCheck(){
  if(distanceRight < 15.0 &&  distanceLeft < 15.0 && 8.0 < distanceForward  && distanceForward < 20.0 ){
//    Serial.println("dead end");
    deadEndFirst++;
    int deadEndStop = 0, tmp = 0;
    if(deadEndFirst ==2){
      motorStop();         delay(50);
      turnAround();        delay(450);
      while(deadEndStop == 0){
          getDistance();  
          buttonState = digitalRead(buttonPin);
          if(buttonState == 0){motorStop(); deadEndStop = 1;}
          turnAround();
          SerialMonitor();
            if(((distanceForward - lastDistanceForward) < 0.0 ) && distanceForward > 25.0)  deadEndStop = 1;
 //           if(tmp == 2)  deadEndStop = 1;
      }
      motorStop();         delay(50);
//      forward();           delay(300);
//      motorStop();
      deadEndFirst = 0;
    }
  }
}


void deadEndCheck2(){
  if(distanceRight < 15.0 &&  distanceLeft < 15.0 && 8.0 < distanceForward  && distanceForward < 20.0 ){
    deadEndFirst++;
    if(deadEndFirst ==2){
      motorStop();         delay(50);
      turnAround();        delay(660);
      motorStop();         delay(50);
//      forward();           delay(300);
//      motorStop();
      deadEndFirst = 0;
    }
  }
}

void turnCheck2(){
  float right = distanceRight - lastDistanceRight;
  float left = distanceLeft - lastDistanceLeft;
  float center = distanceForward - lastDistanceForward;
  if(distanceForward>25.0)  cntRight = 0;
  if(distanceLeft > 25.0 && distanceRight < 25.0 && distanceForward < 25.0){    //turn left
    motorStop();  delay(50);
    turnLeft();  delay(3.6*timeInterval);
    motorStop();  delay(50);
    forward();    delay(4.5*timeInterval);
    motorStop();  delay(50);        
//    Serial.println("turn left");
  }
  if(distanceLeft < 25.0 && distanceRight < 25.0 && distanceForward > 25.0){   //forward
      
  }
  if(distanceLeft < 25.0 && distanceRight > 25.0 && distanceForward < 25.0){   //turn right   
//    cntRight++;
//    if(cntRight == 2){
      motorStop();  delay(50);
      turnRight();  delay(2.5*timeInterval);
      motorStop();  delay(50);
      forward();    delay(4*timeInterval);
      motorStop();  delay(50); 
//      Serial.println("turn right");      
//      cntRight = 0;
//    }
  }  
  if(distanceLeft > 25.0 && distanceRight < 25.0 && distanceForward > 25.0){   //left and forward -> turn left
    motorStop();  delay(50);
    turnLeft();  delay(3.6*timeInterval);
    motorStop();  delay(50);
    forward();    delay(4*timeInterval);
    motorStop();  delay(50);        
//    Serial.println("turn left");
  }
  if(distanceLeft < 25.0 && distanceRight > 25.0 && distanceForward > 25.0){   //forward and right -> forward
      
  }
  if(distanceLeft > 25.0 && distanceRight > 25.0 && distanceForward < 25.0){   //left anf right -> turn left
    motorStop();  delay(50);
    turnLeft();  delay(3.6*timeInterval);
    motorStop();  delay(50);
    forward();    delay(4*timeInterval);
    motorStop();  delay(50);        
//    Serial.println("turn left");
  }
}

void turnCheck(){
  float right = distanceRight - lastDistanceRight;
  float left = distanceLeft - lastDistanceLeft;
  float center = distanceForward - lastDistanceForward;
  long tmp1 = 0, tmp2 = 0, tmp3 = 0, tmp4 = 0;
  if(distanceForward>30.0)  cntRight = 0;
  if(distanceLeft > 30.0 && distanceRight < 30.0 && distanceForward < 30.0){    //turn left
    int leftStop = 0, tmp1 = 0;
    motorStop();  delay(50);
    turnLeft();  delay(2.0*timeInterval);
    while(leftStop == 0){
          getDistance();  
          buttonState = digitalRead(buttonPin);
          if(buttonState == 0){motorStop(); leftStop = 1;}
          turnLeft(); delay(60);
          SerialMonitor();
          if(((distanceForward - lastDistanceForward) < 0.0 ) && distanceForward > 25.0 )  tmp1 = 1;
          if(tmp1 == 2)  leftStop = 1;
    }
    motorStop();  delay(50);
    forward();    delay(4.5*timeInterval);
    motorStop();  delay(50);        
//    Serial.println("turn left");
  }
  if(distanceLeft < 30.0 && distanceRight < 30.0 && distanceForward > 30.0){   //forward
      
  }
  if(distanceLeft < 30.0 && distanceRight > 30.0 && distanceForward < 30.0){   //turn right   
    cntRight++;
    int rightStop = 0, tmp2 = 0;
    if(cntRight == 2){
      motorStop();  delay(50);
      turnRight();  delay(1.2*timeInterval);
      while(rightStop == 0){
        getDistance();  
        buttonState = digitalRead(buttonPin);
        if(buttonState == 0){motorStop(); rightStop = 1;}
        turnRight();
        SerialMonitor();
        if(((distanceForward - lastDistanceForward) < 0.0 ) && distanceForward > 25.0)  tmp2++;
        if(tmp2 == 2)  rightStop = 1;
      }
      motorStop();  delay(50);
      forward();    delay(4*timeInterval);
      motorStop();  delay(50); 
      Serial.println("turn right");      
      cntRight = 0;
    }
  }  
  if(distanceLeft > 30.0 && distanceRight < 30.0 && distanceForward > 30.0){   //left and forward -> turn left
    int leftForwardStop = 0, tmp = 0;
    motorStop();  delay(50);
    turnLeft();  delay(2.0*timeInterval);
    while(leftForwardStop == 0){
          getDistance();  
          buttonState = digitalRead(buttonPin);
          if(buttonState == 0){motorStop(); leftForwardStop = 1;}
          turnLeft();
          SerialMonitor();
          if(((distanceForward - lastDistanceForward) < 0.0 ) && distanceForward > 25.0) leftForwardStop++;
 //         if(tmp == 2) leftForwardStop = 1;
    }
    motorStop();  delay(50);
    forward();    delay(4*timeInterval);
    motorStop();  delay(50);        
    Serial.println("turn left");
  }
  if(distanceLeft < 30.0 && distanceRight > 30.0 && distanceForward > 30.0){   //forward and right -> forward
      
  }
  if(distanceLeft > 30.0 && distanceRight > 30.0 && distanceForward < 30.0){   //left anf right -> turn left
    int leftRightStop = 0, tmp3 = 0;
    motorStop();  delay(50);
    turnLeft();  delay(2.0*timeInterval);
    while(leftRightStop == 0){
          getDistance();  
          buttonState = digitalRead(buttonPin);
          if(buttonState == 0){motorStop(); leftRightStop = 1;}
          turnLeft();
          SerialMonitor();
          if(((distanceForward - lastDistanceForward) < 0.0 ) &&  (distanceForward > 25.0))  leftRightStop++;
//          if(tmp3 == 2)  leftRightStop = 1;
    }
    motorStop();  delay(50);
    forward();    delay(4*timeInterval);
    motorStop();  delay(50);        
    Serial.println("turn left");
  }
}

void driveDirectionCheck(){ 
  if(distanceLeft<8.3 && distanceRight<30.0  && distanceForward > 10.0){
    forwardRight(); delay(20);
    forward();
    Serial.println("forwardRight");
  }
  if(distanceRight<8.1 && distanceLeft<30.0  && distanceForward > 10.0){
    forwardLeft(); delay(20);
    forward(); 
    Serial.println("forwardLeft");
  }
  
  if(distanceRight>8.5 && distanceLeft>30.0){
    forwardRight(); delay(20);
    forward(); 
    Serial.println("forwardLeft");
  }
  if(distanceLeft>8.5 && distanceRight>30.0){
    forwardLeft(); delay(20);
    forward();
    Serial.println("forwardRight");
  }
  if(distanceRight<8.0 && distanceLeft>30.0){
    forwardLeft(); delay(20);
    forward(); 
    Serial.println("forwardLeft");
  }
  if(distanceLeft<8.4 && distanceRight>30.0){
    forwardRight(); delay(20);
    forward();
    Serial.println("forwardRight");
  }
  /*
  float x = distanceRight - distanceLeft;
  if(distanceRight < 30.0 && distanceLeft < 30.0){
    if(x > 2.0){
        forwardLeft();  delay(100);
        forwardRight(); delay(100);
        forward();
//      turnLeft();  delay(50);
//      motorStop();  delay(50);
//      Serial.println("forwardLeft");
    }
    if(x < -2.0){
      forwardRight(); delay(100);
      forwardLeft();  delay(100);
      forward();
//      turnRight();  delay(50);
//      motorStop();  delay(50);
//      Serial.println("forwardRight");
    }
  }
*/
/*
  if(distanceRight < 20.0 && distanceLeft < 20.0){
    if((distanceRight + distanceLeft) > 22.0){
      if(distanceLeft < distanceRight){
//          directionLeftFirst++;
//          if(directionLeftFirst == 2){
            motorStop();  delay(50);
            turnRight();  delay(100);
            motorStop();  delay(50);
            forward();   delay(150);
            Serial.println("turn right");
//            directionLeftFirst = 0;
//          }
        }
        if(distanceRight < distanceLeft){
//          directionRightFirst++;
//          if(directionRightFirst == 2){
            motorStop();  delay(50);
//            backward();   delay(150);
            turnLeft();  delay(100);
            motorStop();  delay(50);
            forward();   delay(150);
            Serial.println("turn left");
            directionRightFirst = 0;
//          }
        }
    }
  }
  */
}

void stockCheck(){
  float right = lastDistanceRight - distanceRight;
  float left = lastDistanceLeft - distanceLeft;
  float forward = lastDistanceForward - distanceForward;
//  Serial.println(forward);
//  if((right >= -2.0 && right <= 2.0) && (left >= -2.0 && left <= 2.0) && (forward >= -2.0 && forward <= 2.0))
/*  if(forward >= -2.0 && forward <= 2.0 && distanceForward < 10.0)  
    checkStock += timeInterval;
  if(distanceForward > 10.0)checkStock = 0;
  if(checkStock >= 600){
    Serial.println("stock");
    backward();   delay(200);
    checkStock = 0;
    if(distanceRight > distanceLeft){
      turnLeft(); delay(640); 
      motorStop();  delay(50);
    }
    if(distanceRight < distanceLeft){
      turnRight(); delay(440); 
      motorStop();  delay(50);
    }
  }
*/
  if( (distanceRight+distanceLeft) > 18.0 && distanceForward < 8.0 ){
    motorStop();  delay(50);
    backward(); delay(450);
    motorStop();  delay(50);
    if(distanceRight < distanceLeft){
      if(distanceLeft > 5.0) {turnLeft(); delay(150); }
      else if(distanceLeft > 2.5) {turnLeft(); delay(200); }
      else{turnLeft(); delay(200); }
      motorStop();  delay(50);
    }
    else if(distanceRight > distanceLeft){
      turnRight(); delay(200); 
      motorStop();  delay(50);
    }
  }
}

void getDistance(){ 
  lastDistanceRight = distanceRight;
  lastDistanceLeft = distanceLeft; 
  lastDistanceForward = distanceForward; 
  digitalWrite(trigRight, LOW);  delayMicroseconds(5);
  digitalWrite(trigRight, HIGH);  delayMicroseconds(60);
  digitalWrite(trigRight, LOW);
  durationRight = pulseIn(echoRight, HIGH);
  distanceRight = durationRight/58.2;
  if(distanceRight > 2500.0)  distanceRight = lastDistanceRight;

  digitalWrite(trigLeft, LOW); delayMicroseconds(5);
  digitalWrite(trigLeft, HIGH); delayMicroseconds(60);
  digitalWrite(trigLeft, LOW);
  durationLeft = pulseIn(echoLeft, HIGH);
  distanceLeft = durationLeft/58.2;
  if(distanceLeft > 2500.0)  distanceLeft = lastDistanceLeft;
  
  digitalWrite(trigForward, LOW);  delayMicroseconds(5);
  digitalWrite(trigForward, HIGH);  delayMicroseconds(60);
  digitalWrite(trigForward, LOW);
  durationForward = pulseIn(echoForward, HIGH);
  distanceForward = durationForward/58.2;
  if(distanceForward > 2000.0)  distanceForward = lastDistanceForward;
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
  analogWrite(motorIn2, 150);
  analogWrite(motorIn3, 150);
  analogWrite(motorIn4, 0);
}

void forwardRight(){
  analogWrite(motorIn1, 0);
  analogWrite(motorIn2, 150);
  analogWrite(motorIn3, 125);
  analogWrite(motorIn4, 0);
}

void forwardLeft(){
  analogWrite(motorIn1, 0);
  analogWrite(motorIn2, 130);
  analogWrite(motorIn3, 150);
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
  analogWrite(motorIn2, 220);
  analogWrite(motorIn3, 0);
  analogWrite(motorIn4, 0);
}

void turnLeft(){
  analogWrite(motorIn1, 0);
  analogWrite(motorIn2, 0);
  analogWrite(motorIn3, 220);
  analogWrite(motorIn4, 0);
}

void turnAround(){
  analogWrite(motorIn1, 200);
  analogWrite(motorIn2, 0);
  analogWrite(motorIn3, 200);
  analogWrite(motorIn4, 0);
}
