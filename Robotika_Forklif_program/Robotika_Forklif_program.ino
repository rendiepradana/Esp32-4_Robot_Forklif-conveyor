#include <PS4Controller.h>

int enableRightMotor = 14;
int rightMotorPin1 = 26;
int rightMotorPin2 = 27;
int enableLeftMotor = 23;
int leftMotorPin1 = 18;
int leftMotorPin2 = 19;

int pnumaticValve = 13;

const int PWMFreq = 1000; /* 1 KHz */
const int PWMResolution = 8;
const int rightMotorPWMSpeedChannel = 4;
const int leftMotorPWMSpeedChannel = 5;

void notify()
{
  int rightMotorSpeed = 0;
  int leftMotorSpeed = 0;

  if (PS4.Up()) {
    leftMotorSpeed = 80;  // Motor DC 1 maju saat tombol "Up" ditekan
  } 
  if (PS4.Down()) {
    leftMotorSpeed = -80;  // Motor DC 1 mundur saat tombol "Down" ditekan
  }

  if (PS4.Left()) {
    rightMotorSpeed = 255;  // Motor DC 2 maju saat tombol "Left" ditekan
  } 
  if (PS4.Right()) {
    rightMotorSpeed = -255;  // Motor DC 2 mundur saat tombol "Right" ditekan
  }
  digitalWrite(pnumaticValve,PS4.Cross());

  rotateMotor(rightMotorSpeed, leftMotorSpeed);
}

void onConnect()
{
  Serial.println("Connected!.");
}

void onDisConnect()
{
  rotateMotor(0, 0);
  Serial.println("Disconnected!.");
}

void rotateMotor(int rightMotorSpeed, int leftMotorSpeed)
{
  digitalWrite(enableRightMotor, HIGH);
  digitalWrite(enableLeftMotor, HIGH);

  if (rightMotorSpeed < 0) {
    digitalWrite(rightMotorPin1, LOW);
    digitalWrite(rightMotorPin2, HIGH);
  } else if (rightMotorSpeed > 0) {
    digitalWrite(rightMotorPin1, HIGH);
    digitalWrite(rightMotorPin2, LOW);
  } else {
    digitalWrite(rightMotorPin1, LOW);
    digitalWrite(rightMotorPin2, LOW);
  }

  if (leftMotorSpeed < 0) {
    digitalWrite(leftMotorPin1, LOW);
    digitalWrite(leftMotorPin2, HIGH);
  } else if (leftMotorSpeed > 0) {
    digitalWrite(leftMotorPin1, HIGH);
    digitalWrite(leftMotorPin2, LOW);
  } else {
    digitalWrite(leftMotorPin1, LOW);
    digitalWrite(leftMotorPin2, LOW);
  }

  ledcWrite(rightMotorPWMSpeedChannel, abs(rightMotorSpeed));
  ledcWrite(leftMotorPWMSpeedChannel, abs(leftMotorSpeed));
}

void setUpPinModes()
{
  pinMode(enableRightMotor, OUTPUT);
  pinMode(rightMotorPin1, OUTPUT);
  pinMode(rightMotorPin2, OUTPUT);
  pinMode(enableLeftMotor, OUTPUT);
  pinMode(leftMotorPin1, OUTPUT);
  pinMode(leftMotorPin2, OUTPUT);
  pinMode(pnumaticValve, OUTPUT);

  // Set up PWM for motor speed
  ledcSetup(rightMotorPWMSpeedChannel, PWMFreq, PWMResolution);
  ledcSetup(leftMotorPWMSpeedChannel, PWMFreq, PWMResolution);
  ledcAttachPin(enableRightMotor, rightMotorPWMSpeedChannel);
  ledcAttachPin(enableLeftMotor, leftMotorPWMSpeedChannel);

  rotateMotor(0, 0);
}

void setup()
{
  setUpPinModes();
  Serial.begin(115200);
  PS4.begin("1a:2b:3c:01:01:01");
  PS4.attach(notify);
  PS4.attachOnConnect(onConnect);
  PS4.attachOnDisconnect(onDisConnect);
  Serial.println("Ready.");
}

void loop()
{
}