/********************************************************************
 * SparkFun Inventor's Kit
 * Example sketch 12
 * 
 * SPINNING A MOTOR
 * 
 *******************************************************************/

#include <Servo.h>

const int motorKiri  = 6;  // Connect the base of the transistor to pin 9.
const int motorKanan = 5;  // Even though it's not directly connected to the motor,
const int motorVCC = 11;   // we'll call it the 'motorPin'

const int trigPin = 12;
const int echoPin = 13;

Servo sweep, neck;  // servo control object

// defines variables
const int N = 20;
long duration,t1,t2;
float fdistance, ldistance, rdistance;
int A[N][N] = { 0 };
int *rbt;
int i=N-1, j=0, k,x,y;
int count=2;
int turn=0;
int lul=0;

// defines stages
bool stage1 = true;
bool stage2 = false;
bool uTurn = false;
bool scan = true;
bool EM = false;
bool mov = false;
bool refresh = true;
bool notcom=false;

// begin robot program
void setup()
{
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  //pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600); // Starts the serial communication

  pinMode(motorKiri, OUTPUT);  // set up the pin as an OUTPUT
  pinMode(motorKanan, OUTPUT);
  pinMode(motorVCC, OUTPUT);
  
  neck.attach(9, 675, 2830);
  sweep.attach(8, 675, 2830);
  
  *rbt = A[0][0];
  A[N-1][0] = (N*N)+1;
}


void loop()
{ // This example basically replicates a blink, but with the motorPin instead.
  int onTime = 3000;  // milliseconds to turn the motor on
  int offTime = 3000; // milliseconds to turn the motor off
  if(refresh==true) {
    delay(2000);
    refresh=false;
  }
  else {
    /*if(((i==N-1)&&(j==N-1))||((i==N-1)&&(j==N-1))){
      checkMatrices();
      if(notcom==true) backtrack();
    }else{*/
      if(stage1==true) Serial.println("Stage 1!");
      if(stage2==true) Serial.println("Stage 2!");
      if(scan==true) scanning();
      if(EM==true) enterMatrices();
      if(mov==true) robotMove();
    //}
  }
}

void scanning()
{
  scan=false;
  // ======= SERVO (NECK) =======
  neck.write(90);    // Tell servo to go to 90 degrees
  delay(1500);         // Pause to get it time to move
  usFront();
  
  //////////////////////////
  neck.write(180);   // Tell servo to go to 180 degrees (kiri)
  delay(1500);         // Pause to get it time to move
  usLeft();

  ///////////////////////////
  neck.write(0);     // Tell servo to go to 0 degrees (kanan)
  delay(1500);         // Pause to get it time to move
  usRight();
  
  neck.write(90);
  delay(1500);
  EM=true;
  
}

void enterMatrices()
{
  EM=false;
    if (fdistance > 20){
      Serial.println("F-1.1");
      if(stage1==true) {Serial.println("F-1.1.1"); if(A[i-1][j]==0) A[i-1][j] = 1;}
      else {Serial.println("F-1.1.2"); if(A[i+1][j]==0) A[i+1][j] = 1;}
    } else {
      Serial.println("F-1.-1");
      if(stage1==true) {Serial.println("F-1.-1.1"); if(A[i-1][j]==0) A[i-1][j] = -1;}
      else {Serial.println("F-1.-1.2"); if(A[i+1][j]==0) A[i+1][j] = -1;}
    }
  
    if (rdistance > 20){
      Serial.println("R-1.1");
      if(stage1==true && turn!=N-1) {Serial.println("R-1.1.1"); if(A[i][j+1]==0) A[i][j+1] = 1;}
      else {Serial.println("R-1.1.2"); if(turn!=0) if(A[i][j-1]==0) A[i][j-1] = 1;}
    } else {
      Serial.println("R-1.-1");
      if(stage1==true && turn!=N-1) {Serial.println("R-1.-1.1"); if(A[i][j+1]==0) A[i][j+1] = -1;}
      else {Serial.println("R-1.-1.2"); if(turn!=0) if(A[i][j-1]==0) A[i][j-1] = -1;}
    }
    
    if (ldistance > 20){
      Serial.println("L-1.1");
      if(stage1==true) {Serial.println("L-1.1.1"); if(turn!=0) if(A[i][j-1]==0) A[i][j-1] = 1;}
      else {Serial.println("L-1.1.2"); if(turn!=N-1) if(A[i][j+1]==0) A[i][j+1] = 1;}
    } else {
      Serial.println("L-1.-1");
      if(stage1==true) {Serial.println("L-1.-1.1"); if(turn!=0) if(A[i][j-1]==0) A[i][j-1] = -1;}
      else {Serial.println("L-1.-1.2"); if(turn!=N-1) if(A[i][j+1]==0) A[i][j+1] = -1;}
    }
  
  mov=true;
}

void robotMove()
{
  digitalWrite(motorVCC, HIGH);
  delay(500);
  if((i==N-1)&&(stage2==true)){
    uTurn = true;
    turn++;
    if (A[i][j+1]==1)
      utLeft();
  }
  else if((i==0)&&(stage1==true)){
    uTurn = true;
    turn++;
    if(A[i][j+1]==1)
      utRight();
  }
  if (uTurn==false){
    if(stage1==true) k=i-1;
    else k=i+1;
    Serial.println(A[k][j]);
    if (A[k][j]==1){  
      moveForward();
      if(stage1==true) *rbt = A[--i][j];
      else *rbt = A[++i][j];
      Serial.print(i);
      Serial.print(" ");
      Serial.println(j);
    } else if (A[k][j]==-1){
      uTurn = true;
      Serial.println("uTurn = true");
      turn++;
      Serial.println(A[i][j+1]);
      if(stage1==true){
        Serial.println("Right U-Turn!");
        if(A[i][j+1]==1)
            utRight();
      }
      else{
        Serial.println("Left U-Turn!");
        if (A[i][j+1]==1)
            utLeft();
      }
    }
  } else{
    if(stage1==true){
      Serial.println("false - 1");
      moveForward(); 
      *rbt=A[i][++j];
      if (A[i+1][j]==1){
        analogWrite(motorKiri, 250);
        analogWrite(motorKanan, 0);
        delay(400);
        moveForward();
        *rbt=A[++i][j];
        uTurn = false;
        stage1=false;
        stage2=true;
      }
    }
    else if(stage2==true){
      Serial.println("false - 2");
      moveForward(); 
      *rbt=A[i][++j];
      if (A[i-1][j]==1) {
        analogWrite(motorKiri, 0);
        analogWrite(motorKanan, 237);
        delay(400);
        moveForward();
        *rbt=A[--i][j];
        uTurn = false;
        stage1=true;
        stage2=false;
      }
    }
  }
  
  *rbt = count++;
  scan=true;
}

// defines moveset functions
void moveForward(){
  Serial.println("MOVE FORWARD");
  analogWrite(motorKiri, 215);
  analogWrite(motorKanan, 213);
  sweep.write(135);
  delay(500);
  usFront();
  if(fdistance<20) digitalWrite(motorVCC,LOW);
  sweep.write(45);
  delay(500);
  usFront();
  if(fdistance<20) digitalWrite(motorVCC,LOW);
  
  if(uTurn==false){  
    sweep.write(135);
    delay(620);
    usFront();
    if(fdistance<20) digitalWrite(motorVCC,LOW);
    sweep.write(45);
    delay(620);
    usFront();
    if(fdistance<20) digitalWrite(motorVCC,LOW);
    
    sweep.write(135);
    delay(620);
    usFront();
    if(fdistance<20) digitalWrite(motorVCC,LOW);
    sweep.write(45);
    delay(620);
    usFront();
    if(fdistance<20) digitalWrite(motorVCC,LOW);
  }
  
  sweep.write(90);
  delay(620);
  moveStop();
  
}

void moveRight(){
  analogWrite(motorKiri, 250);
  analogWrite(motorKanan, 0);
}

void utRight(){

  Serial.println("MOVE RIGHT");
  if(uTurn==true){
    moveRight();
    delay(1000);
    moveForward(); 
    *rbt=A[i][++j];
    Serial.print(i);
    Serial.print(" ");
    Serial.println(j);
    scanning();
    Serial.println(A[i+1][j]);
    if (A[i+1][j]==1) {
      Serial.println("MOVE RIGHT - second!");
      digitalWrite(motorVCC, HIGH);
      delay(500);
      moveRight();
      delay(1000);
      moveForward();
      *rbt=A[++i][j];
      Serial.print(i);
      Serial.print(" ");
      Serial.println(j);
      uTurn = false;
      stage1 = false;
      stage2 = true;
    }
  }
}

void moveLeft(){
  analogWrite(motorKiri, 0);
  analogWrite(motorKanan, 236);
}

void utLeft(){

  Serial.println("MOVE LEFT");
  if(uTurn==true){
    moveLeft();
    delay(1000);
    moveForward(); 
    *rbt=A[i][++j];
    scanning();
    if (A[i-1][j]==1) {
      Serial.println("MOVE LEFT - second!");
      digitalWrite(motorVCC, HIGH);
      delay(500);
      moveLeft();
      delay(1000);
      moveForward();
      *rbt=A[--i][j];
      Serial.print(i);
      Serial.print(" ");
      Serial.println(j);
      uTurn = false;
      stage1 = true;
      stage2 = false;
    }
  }
}

void usFront(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on 120 state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  while(digitalRead(echoPin)==0);
  t1 = micros();
  while(digitalRead(echoPin)==1);
  t2 = micros();
  
  duration = t2-t1; //pulseIn(echoPin, 120);
  // Calculating the distance
  fdistance= duration / 58.0; //duration*0.034/2;
  Serial.print("Front Distance: ");
  Serial.println(fdistance);

}

void usRight(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on 120 state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  while(digitalRead(echoPin)==0);
  t1 = micros();
  while(digitalRead(echoPin)==1);
  t2 = micros();
  
  duration = t2-t1; //pulseIn(echoPin, 120);
  // Calculating the distance
  rdistance= duration / 58.0; //duration*0.034/2;
  Serial.print("Right Distance: ");
  Serial.println(rdistance);
  
}

void usLeft(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on 120 state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  while(digitalRead(echoPin)==0);
  t1 = micros();
  while(digitalRead(echoPin)==1);
  t2 = micros();
  
  duration = t2-t1; //pulseIn(echoPin, 120);
  // Calculating the distance
  ldistance= duration / 58.0; //duration*0.034/2;
  Serial.print("Left Distance: ");
  Serial.println(ldistance);
  
}

void moveStop(){

  analogWrite(motorKiri,  0);
  analogWrite(motorKanan, 0);
  digitalWrite(motorVCC, LOW);
}

void checkMatrices(){
  for(x=0;x<N;x++){
    Serial.println(x);
    for(y=0;y<N;y++){
      if(A[x][y]==1) notcom=true;
    }
  }
}

void backtrack(){
  if(scan==true) scanning();
  if(EM==true) enterMatrices();
  if(mov==true) robotMove();
  
}

