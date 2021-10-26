#include <SoftwareSerial.h>
SoftwareSerial serialTest(9, 10);//serial object creation for the camera and arduino
//receiver, transmitter for Camera
float motor_a = 0;
float motor_b = 0;

int drive_speed = 100; //forward drive speed
int error = 0;
int turn_speed = 0;
float prev_error = 0;
float diff_error;
float kp = 1.25;
float kd = 0.9;
enum STATE {CHASE, RUN, SEARCH, STOP};
STATE state = CHASE;

//ULTRASONIC
const int trig = 11; // send signal
const int echo = 12; // back to board
float distance = 0;

//motor pins
const int AIN1 = 7;
const int AIN2 = 4;
const int PWMA = 6;
const int BIN1 = 2;
const int BIN2 = 3;
const int PWMB = 5;

void setup() {

  Serial.begin(9600);//general Serial to connect to computer
  serialTest.begin(9600); // serial software object
  //between camera and arduino
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  delay(100);

}

void loop() {
  
    // Camera
    prev_error = error;
    if (serialTest.available() > 0) {
      error = serialTest.parseInt();
    }
    diff_error = error - prev_error;
    turn_speed = kp * error + kd * diff_error; // check
  
    if (error == 6969) { // Close and sees April Tag
      state = RUN;
    }
    else if (error == 1234) { // No tag
      state = SEARCH;
    }
    else {
      state = CHASE;
    }
  
    /* ----- EXECUTE STATES -----*/
    if (state == CHASE) {
  
      motor_a = drive_speed - turn_speed;
      motor_b = drive_speed + turn_speed;
  
      driveMotors(motor_a, motor_b);
      delay(50);
    }
    else if (state == RUN) {
      digitalWrite(AIN1, LOW);
      digitalWrite(AIN2, LOW);
      digitalWrite(BIN1, LOW);
      digitalWrite(BIN2, LOW);
      delay(50);
    }
    else if (state == SEARCH) {
      //turn here
      digitalWrite(AIN1, LOW);
      digitalWrite(AIN2, HIGH);
      digitalWrite(BIN1, HIGH);
      digitalWrite(BIN2, LOW);
      analogWrite(PWMA, 100);
      analogWrite(PWMB, 100);
      delay(50);
    }
}
void driveMotors(float leftmotor, float rightmotor) {
  if (leftmotor > 0) {
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
  }
  else if (leftmotor < 0) {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
  }
  else { //for 0
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, LOW);
  }
  if (rightmotor > 0) {
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
  }
  else if (rightmotor < 0) {
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
  }
  else {
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, LOW);
  }
  leftmotor = constrain(leftmotor, -255, 255);
  rightmotor = constrain(rightmotor, -255, 255);
  leftmotor = abs(leftmotor);
  rightmotor = abs(rightmotor);

  analogWrite(PWMA, leftmotor);
  analogWrite(PWMB, rightmotor);
}
float getDistance() {
  digitalWrite(trig, HIGH); //PULSE ON
  delayMicroseconds(10);
  digitalWrite(trig, LOW); //PULSE OFF
  float echoTime = pulseIn(echo, HIGH);//HIGH VOLTAGE, RECEIVES VALUE
  float calculateDistance = echoTime / 148.0;
  return calculateDistance;
}
float filterDist(){
  int num = 2;
  float maxDist = 0;
  float minDist = 500;
  float avgDist = 0;
  for (int i = 0;i<num;i++){
    float nDist = getDistance();
    avgDist += nDist;
    if (nDist > maxDist){
      maxDist = nDist;
    }
    else if (nDist < minDist){
      minDist = nDist;
    }
  }
  avgDist /= num;
  if (maxDist - minDist > 10){
    return minDist;
  }
  else{
    return avgDist;
  }
}
