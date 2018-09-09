/*
   202 Robot Line Follow with Flags
   Version 2.1
   Created: 4/12/2018
   Author :SDP and NLP
*/

//REQUIRED HEADERS-------------------------------------------------------
#include <AFMotor.h>                //Adafruit Motor Controller V1.2 Library

//VARIABLES---------------------------------------------------------------
int motorSpeed = 255 ;              //motor speed bwtween 0 and 255: a value of less than 150 does not usually work
int pivotSpeed = motorSpeed / 50;   //pivot motor speed
int turnSpeed = motorSpeed / 4;     //turning motor speed
int t = 1;                          //millisecond time between loops: Smaller is better but increases "twitchiness"
int sensorDelay = 10;               //Delay in Sensor Motions: for HC SR04 Ultrasonic Sensor
int offLineTime = 10;               //Time to turn and to drive forward once flag is set and called forward
int startButton = 13;               //Start button on pin 13
int leftBumper = 0;                 //Left Bumper on pin 0 (TX)
int rightBumper = 1;                 //Right Bumper on pin 1 (RX)


int farLeft = 0;                    //Channel 0 initialization
int left = 0;                       //Channel 1 initialization
int middle = 0;                     //Channel 2 initialization
int right = 0;                      //Channel 3 initialization
int farRight = 0;                   //Channel 4 initialization
int leftFlag = 0;                   //Flag when last known sensor value is the left sensor: used when line is lost
int rightFlag = 0;                  //Flag when last known sensor value is the right sensor: used when line is lost
/*const int trigpinR=0; //Sensor trigger pin
  const int trigpinl=0; //Sensor trigger pin
  const int echopinR=1; //Right sensor echo pin
  const int echopinL=2; //Left sensor echo pin
  long durationR;
  int distanceR;
  long durationL;
  int distanceL; */

//DEFINITIONS-------------------------------------------------------------
AF_DCMotor leftMotor(1, MOTOR12_64KHZ);  // create motor #2, 64KHz
AF_DCMotor rightMotor(2, MOTOR12_64KHZ); // create motor #1, 64KHz


//SETUP XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
void setup() {
  Serial.begin(9600);                   // set up Serial port at 9600 Hz: used for serial monitor

  leftMotor.setSpeed(motorSpeed);      // set the initial LM speed to motorSpeed
  rightMotor.setSpeed(motorSpeed);     // set the initial RM speed to motorSpeed

  pinMode(startButton, INPUT);
  pinMode(leftBumper, INPUT);
  pinMode(RightBumper, INPUT);

  /*  pinMode(trigpin, OUTPUT);
    pinMode(echopinR, INPUT);
    pinMode(echopinL, INPUT);*/

}  //end setup

void loop() { //Begin Driving Loop XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

  //WAIT FOR START BUTTON or switch -------------------------------------------------------------

  // not yet installed OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO

//READ SENSORS and BUTTON---------------------------------------------------------------------------------
  //Sensor values are inverted with Black line sensor array
  farLeft = 1 - digitalRead(A0);
  left = 1 - digitalRead(A1);
  middle = 1 - digitalRead(A2);
  right = 1 - digitalRead(A3);
  farRight = 1 - digitalRead(A4);


//CREATE VECTOR FROM SENSOR READINGS-----------------------------------------------------------
  int sensorValues[5] = {farLeft, left, middle, right, farRight};
  //5 digit array
  /*sensorValues[0] = farLeft;
    sensorValues[1] = Left;
    sensorValues[2] = middle;
    sensorValues[3] = right;
    sensorValues[4] = farRight;*/


  Serial.println(sensorValues[1]);

//RUN IF/ELSEIF LOOP OF MOTION-----------------------------------------------------------------

  if (startButton == 1) {
    //wait for button release to begin program
    delay(100);
  }
  else if (leftBumper == 1) {           //Test to see if the left bumper is pressed
    pivotRight();
    delay(offLineTime);
    driveForward();
    delay(offLineTime);
  }
  else if (rightBumper == 1) {         //Test to see if the right bumper is pressed
    pivotLeft();
    delay(offLineTime);
    driveForward();
    delay(offLineTime);
  }
  else if (sensorValues[0] == 0 && sensorValues[1] == 0 && sensorValues[2] == 1 && sensorValues[3] == 0 && sensorValues[4] == 0) {
    leftFlag = 0;                       //Resets Flag
    rightFlag = 0;                      //Resets Flag
    driveForward();
    delay(t);
  }
  else if (sensorValues[0] == 0 && sensorValues[1] == 1 && sensorValues[2] == 0 && sensorValues[3] == 0 && sensorValues[4] == 0) {
    leftFlag = 0;                       //Resets Flag
    rightFlag = 0;                      //Resets Flag
    turnLeft();
    delay(t);
  }
  else if (sensorValues[0] == 1 && sensorValues[1] == 1 && sensorValues[2] == 0 && sensorValues[3] == 0 && sensorValues[4] == 0) {
    leftFlag = 0;                       //Resets Flag
    rightFlag = 0;                      //Resets Flag
    pivotLeft();
    delay(t);
  }
  else if (sensorValues[0] == 1 && sensorValues[1] == 0 && sensorValues[2] == 0 && sensorValues[3] == 0 && sensorValues[4] == 0) {
    leftFlag = 1;                       //Sets Left Flag
    rightFlag = 0;                      //Resets Flag
    pivotLeft();
    delay(t);
  }
  else if (sensorValues[0] == 0 && sensorValues[1] == 0 && sensorValues[2] == 0 && sensorValues[3] == 1 && sensorValues[4] == 0) {
    leftFlag = 0;                       //Resets Flag
    rightFlag = 0;                      //Resets Flag
    turnRight();
    delay(t);
  }
  else if (sensorValues[0] == 0 && sensorValues[1] == 0 && sensorValues[2] == 0 && sensorValues[3] == 1 && sensorValues[4] == 1) {
    leftFlag = 0;                       //Resets Flag
    rightFlag = 0;                      //Resets Flag
    pivotRight();
    delay(t);
  }
  else if (sensorValues[0] == 0 && sensorValues[1] == 0 && sensorValues[2] == 1 && sensorValues[3] == 1 && sensorValues[4] == 0) {
    leftFlag = 0;                       //Resets Flag
    rightFlag = 0;                      //Resets Flag
    turnRight();
    delay(t);
  }
  else if (sensorValues[0] == 0 && sensorValues[1] == 0 && sensorValues[2] == 0 && sensorValues[3] == 0 && sensorValues[4] == 1) {
    leftFlag = 0;                       //Resets Flag
    rightFlag = 1;                      //Sets Right Flag
    pivotRight();
    delay(t);
  }
  else if (sensorValues[0] == 1 && sensorValues[1] == 1 && sensorValues[2] == 1 && sensorValues[3] == 1 && sensorValues[4] == 1) {
    leftFlag = 0;                       //Resets Flag
    rightFlag = 0;                      //Resets Flag
    motorStop();
    delay(10 * t);
  }
  else if (sensorValues[0] == 1 && sensorValues[1] == 1 && sensorValues[2] == 1 && sensorValues[3] == 1 && sensorValues[4] == 1) {

    if (rightFlag == 1) {                //If last known sensor was on the left
      pivotRight();
      delay(offLineTime);
      driveForward();
      delay(offLineTime);
    }
    else if (leftFlag == 1) {            //If last known sensor was on the right
      pivotLeft();
      delay(offLineTime);
      driveForward();
      delay(offLineTime);

    }
    else {
      //do nothing
    }
  }
  else {
    driveForward();                     //drive forward
    delay(t);
  }

//BREAK IF BUTTON IS PRESSED-------------------------------------------------------------

  //not yet installed

} //end void loop XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

//FUNCTIONS-------------------------------------------------------------
void driveForward() { //Forward Function
  leftMotor.setSpeed(motorSpeed);      // set the speed to motorSpeed
  rightMotor.setSpeed(motorSpeed);     // set the speed to motorSpeed

  leftMotor.run(FORWARD);              // turn LM on going forward
  rightMotor.run(FORWARD);             // turn RM on going forward
}
void driveBackward() { //ReverseFunction
  leftMotor.setSpeed(motorSpeed);     // set the speed to motorSpeed
  rightMotor.setSpeed(motorSpeed);    // set the speed to motorSpeed

  leftMotor.run(BACKWARD);             // turn LM on going in reverse
  rightMotor.run(BACKWARD);            // turn RM on going in reverse
}
void pivotLeft() {
  leftMotor.setSpeed(pivotSpeed);     // set the speed to pivotSpeed
  rightMotor.setSpeed(motorSpeed);    // set the speed to motorSpeed

  leftMotor.run(FORWARD);              // turn LM on going forward
  rightMotor.run(FORWARD);             // turn RM on going in reverse
}
void pivotRight() {
  leftMotor.setSpeed(motorSpeed);     // set the speed to motorSpeed
  rightMotor.setSpeed(pivotSpeed);    // set the speed to pivotSpeed

  leftMotor.run(FORWARD);             // turn LM on going in reverse
  rightMotor.run(FORWARD);            // turn RM on going forward
}
void turnRight() {
  leftMotor.setSpeed(motorSpeed);    // set the speed to motorSpeed
  rightMotor.setSpeed(turnSpeed);    // set the speed to turnSpeed

  leftMotor.run(FORWARD);              // turn LM off
  rightMotor.run(FORWARD);             // turn RM on going forward
}
void turnLeft() {
  leftMotor.setSpeed(turnSpeed);      // set the speed to turnSpeed
  rightMotor.setSpeed(motorSpeed);    // set the speed to motorSpeed

  leftMotor.run(FORWARD);               // turn LM on going forward
  rightMotor.run(FORWARD);              // turn RM off
}
void motorStop() {
  leftMotor.run(RELEASE);               // turn LM off
  rightMotor.run(RELEASE);              // turn RM off
}