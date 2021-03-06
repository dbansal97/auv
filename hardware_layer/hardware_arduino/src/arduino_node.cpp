// Copyright 2016 AUV-IITK
#include <ros.h>
#include <Arduino.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Float64.h>

// /*************************** for AUV ****************************/
// #define led 13

// #define pwmPinWest 5
// #define pwmPinEast 4
// #define directionPinWest1 27
// #define directionPinWest2 26
// #define directionPinEast1 29
// #define directionPinEast2 28

// #define pwmPinNorthSway 5
// #define pwmPinSouthSway 4
// #define directionPinNorthSway1 27
// #define directionPinNorthSway2 26
// #define directionPinSouthSway1 29
// #define directionPinSouthSway2 28

// #define pwmPinNorthUp 5
// #define pwmPinSouthUp 4
// #define directionPinNorthUp1 27
// #define directionPinNorthUp2 26
// #define directionPinSouthUp1 29
// #define directionPinSouthUp2 28

/************************ for New Ground Testing Bot *******************/
#define led 13  // 13

#define pwmPinWest 5         // 3
#define pwmPinEast 2         // 2
#define directionPinWest1 6  // 31
#define directionPinWest2 7  // 30
#define directionPinEast1 3  // 33
#define directionPinEast2 4  // 32

#define pwmPinNorthSway 23         // 3
#define pwmPinSouthSway 13         // 2
#define directionPinNorthSway1 8   // 31
#define directionPinNorthSway2 9   // 30
#define directionPinSouthSway1 11  // 33
#define directionPinSouthSway2 12  // 32

#define pwmPinNorthUp 13        // 3
#define pwmPinSouthUp 13        // 2
#define directionPinNorthUp1 0  // 31
#define directionPinNorthUp2 0  // 30
#define directionPinSouthUp1 0  // 33
#define directionPinSouthUp2 0  // 32

#define analogPinPressureSensor A0
/************************ for old ground bot ******************************/
// #define pwmPinWest 4
// #define pwmPinEast 5
// #define directionPinWest1 2
// #define directionPinWest2 3
// #define directionPinEast1 7
// #define directionPinEast2 6
// #define led 13

const float c092 = 506.22;
const float s092 = -2.65;
const float c093 = 448.62;
const float s093 = -2.92;
const float c099 = 397.65;   // reference as their graph is at lowest
const float s099 = -2.71;  // reference as their graph is at lowest
const float c113 = 539.85;
const float s113 = -3.38;
const float c117 = 441.32;
const float s117 = -3.03;
const float c122 = 547.39;
const float s122 = -2.93;

int Delay = 1500;
bool isMovingForward = true;
float v;
std_msgs::Float64 voltage;
ros::NodeHandle nh;

int btd092(int pwm)
{
  pwm = (c099 + s099 * pwm - c092) / (s092);
  return pwm;
}

int btd093(int pwm)
{
  pwm = (c099 + s099 * pwm - c093) / (s093);
  return pwm;
}

int btd099(int pwm)
{
  return pwm;
}

int btd113(int pwm)
{
  pwm = (c099 + s099 * pwm - c113) / (s113);
  return pwm;
}

int btd117(int pwm)
{
  pwm = (c099 + s099 * pwm - c117) / (s117);
  return pwm;
}

int btd122(int pwm)
{
  pwm = (c099 + s099 * pwm - c122) / (s122);
  return pwm;
}


void thrusterNorthUp(int pwm, int isUpward)
{
  pwm = btd122(pwm);
  analogWrite(pwmPinNorthUp, pwm);
  if (isUpward)
  {
    digitalWrite(directionPinNorthUp1, HIGH);
    digitalWrite(directionPinNorthUp2, LOW);
  }
  else
  {
    digitalWrite(directionPinNorthUp1, LOW);
    digitalWrite(directionPinNorthUp2, HIGH);
  }
}

void thrusterSouthUp(int pwm, int isUpward)
{
  pwm = btd117(pwm);
  analogWrite(pwmPinSouthUp, pwm);
  if (isUpward)
  {
    digitalWrite(directionPinSouthUp1, HIGH);
    digitalWrite(directionPinSouthUp2, LOW);
  }
  else
  {
    digitalWrite(directionPinSouthUp1, LOW);
    digitalWrite(directionPinSouthUp2, HIGH);
  }
}

void thrusterNorthSway(int pwm, int isRight)
{
  pwm = btd113(pwm);
  analogWrite(pwmPinNorthSway, pwm);
  if (isRight)
  {
    digitalWrite(directionPinNorthSway1, HIGH);
    digitalWrite(directionPinNorthSway2, LOW);
  }
  else
  {
    digitalWrite(directionPinNorthSway1, LOW);
    digitalWrite(directionPinNorthSway2, HIGH);
  }
}

void thrusterSouthSway(int pwm, int isRight)
{
  pwm = btd099(pwm);
  analogWrite(pwmPinSouthSway, pwm);
  if (isRight)
  {
    digitalWrite(directionPinSouthSway1, HIGH);
    digitalWrite(directionPinSouthSway2, LOW);
  }
  else
  {
    digitalWrite(directionPinSouthSway1, LOW);
    digitalWrite(directionPinSouthSway2, HIGH);
  }
}

void thrusterEast(int pwm, int isForward)
{
  pwm = btd093(pwm);
  analogWrite(pwmPinEast, pwm);
  if (isForward)
  {
    digitalWrite(directionPinEast1, HIGH);
    digitalWrite(directionPinEast2, LOW);
  }
  else
  {
    digitalWrite(directionPinEast1, LOW);
    digitalWrite(directionPinEast2, HIGH);
  }
}

void thrusterWest(int pwm, int isForward)
{
  pwm = btd092(pwm);
  analogWrite(pwmPinWest, pwm);
  if (isForward)
  {
    digitalWrite(directionPinWest1, HIGH);
    digitalWrite(directionPinWest2, LOW);
  }
  else
  {
    digitalWrite(directionPinWest1, LOW);
    digitalWrite(directionPinWest2, HIGH);
  }
}

void PWMCbForward(const std_msgs::Int32& msg)
{
  if (msg.data > 0)
  {
    thrusterEast(255 - msg.data, true);
    thrusterWest(255 - msg.data, true);
  }
  else
  {
    thrusterEast(255 + msg.data, false);
    thrusterWest(255 + msg.data, false);
  }
  isMovingForward = true;
}

void PWMCbSideward(const std_msgs::Int32& msg)
{
  if (msg.data > 0)
  {
    thrusterNorthSway(255 - msg.data, true);
    thrusterSouthSway(255 - msg.data, true);
  }
  else
  {
    thrusterNorthSway(255 + msg.data, false);
    thrusterSouthSway(255 + msg.data, false);
  }
  isMovingForward = false;
}

void PWMCbUpward(const std_msgs::Int32& msg)
{
  if (msg.data > 0)
  {
    thrusterNorthUp(255 - msg.data, true);
    thrusterSouthUp(255 - msg.data, true);
  }
  else
  {
    thrusterNorthUp(255 + msg.data, false);
    thrusterSouthUp(255 + msg.data, false);
  }
}

void PWMCbTurn(const std_msgs::Int32& msg)
{
  if (isMovingForward)
  {
    if (msg.data > 0)
    {
      thrusterEast(255 - msg.data, true);
      thrusterWest(255 - msg.data, false);
    }
    else
    {
      thrusterEast(255 + msg.data, false);
      thrusterWest(255 + msg.data, true);
    }
  }
  else
  {
    if (msg.data > 0)
    {
      thrusterNorthSway(255 - msg.data, false);
      thrusterSouthSway(255 - msg.data, true);
    }
    else
    {
      thrusterNorthSway(255 + msg.data, true);
      thrusterSouthSway(255 + msg.data, false);
    }
  }
}

ros::Subscriber<std_msgs::Int32> subPwmForward("/pwm/forward", &PWMCbForward);
ros::Subscriber<std_msgs::Int32> subPwmSideward("/pwm/sideward", &PWMCbSideward);
ros::Subscriber<std_msgs::Int32> subPwmUpward("/pwm/upward", &PWMCbUpward);
ros::Subscriber<std_msgs::Int32> subPwmTurn("/pwm/turn", &PWMCbTurn);
ros::Publisher ps_voltage("zDistance", &voltage);

void setup()
{
  nh.initNode();

  pinMode(led, OUTPUT);
  pinMode(directionPinEast1, OUTPUT);
  pinMode(directionPinEast2, OUTPUT);
  pinMode(pwmPinWest, OUTPUT);
  pinMode(directionPinWest2, OUTPUT);
  pinMode(pwmPinEast, OUTPUT);
  pinMode(directionPinWest1, OUTPUT);

  pinMode(directionPinSouthSway1, OUTPUT);
  pinMode(directionPinSouthSway2, OUTPUT);
  pinMode(pwmPinNorthSway, OUTPUT);
  pinMode(directionPinNorthSway2, OUTPUT);
  pinMode(pwmPinSouthSway, OUTPUT);
  pinMode(directionPinNorthSway1, OUTPUT);

  pinMode(directionPinSouthUp1, OUTPUT);
  pinMode(directionPinSouthUp2, OUTPUT);
  pinMode(pwmPinNorthUp, OUTPUT);
  pinMode(directionPinNorthUp2, OUTPUT);
  pinMode(pwmPinSouthUp, OUTPUT);
  pinMode(directionPinNorthUp1, OUTPUT);

  nh.subscribe(subPwmForward);
  nh.subscribe(subPwmSideward);
  nh.subscribe(subPwmUpward);
  nh.subscribe(subPwmTurn);
  nh.advertise(ps_voltage);
  Serial.begin(57600);
}

void loop()
{
  v = analogRead(analogPinPressureSensor);
  voltage.data = v;
  ps_voltage.publish(&voltage);
  nh.spinOnce();
  delay(1);
}
