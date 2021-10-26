#include <SoftwareSerial.h>
SoftwareSerial serialTest(9, 10);//serial object creation for the camera and arduino
//receiver, transmitter for Camera
float motor_a = 0;
float motor_b = 0;
float kp = 0.7;
int drive_speed = 100; //forward drive speed
int error = 0;
int turn_speed = 0;
float prev_error = 0;
float diff_error;
float kd = 0.6;

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
}

void loop() {
  prev_error = error;
  error = serialTest.parseInt();
  diff_error = error - prev_error;

  turn_speed = kp * error + kd * diff_error;
  motor_a = drive_speed - turn_speed;
  motor_b = drive_speed + turn_speed;
  
  driveMotors(motor_a, motor_b);
  delay(50);
}

void driveMotors(int motor_a, int motor_b) {
  if (motor_a > 0) { 
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
  }
  else if (motor_a < 0) {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
  }
  if (motor_b > 0) {
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
  }
  else if (motor_b < 0) {
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
  }
  motor_a = constrain(motor_a, -255, 255);
  motor_b = constrain(motor_b, -255, 255);
  motor_a = abs(motor_a);
  motor_b = abs(motor_b);

  analogWrite(PWMA, motor_a);
  analogWrite(PWMB, motor_b);
}
