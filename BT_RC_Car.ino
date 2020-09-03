#include <SoftwareSerial.h>
SoftwareSerial BT(11, 12);
char val;
const byte LEFT1 = 7;
const byte LEFT2 = 6;
const byte LEFT_PWM = 5;

const byte RIGHT1 = 9;
const byte RIGHT2 = 8;
const byte RIGHT_PWM = 10;

byte LmotorSpeed = 50;
byte RmotorSpeed = 45;

const byte LEFT_IR = 3;
const byte RIGHT_IR =2;

volatile boolean hasInterrupted =false;

////////////////// Motor Controlling Functions ///////////////////////
void right_mt_Forward(byte mts)
{
  digitalWrite(RIGHT1, HIGH);
  digitalWrite(RIGHT2, LOW);
  analogWrite(RIGHT_PWM, mts);
}

void left_mt_Forward(byte mts)
{
  digitalWrite(LEFT1, HIGH);
  digitalWrite(LEFT2, LOW);
  analogWrite(LEFT_PWM, mts);
}

void right_mt_Backward(byte mts)
{
  digitalWrite(RIGHT1, LOW);
  digitalWrite(RIGHT2, HIGH);
  analogWrite(RIGHT_PWM, mts);
}

void left_mt_Backward(byte mts)
{
  digitalWrite(LEFT1, LOW);
  digitalWrite(LEFT2, HIGH);
  analogWrite(LEFT_PWM, mts);
}

void forward()
{
  left_mt_Forward(LmotorSpeed);
  right_mt_Forward(RmotorSpeed);
}

void backward()
{
  left_mt_Backward(LmotorSpeed);
  right_mt_Backward(RmotorSpeed);
}

void leftForward()
{
  analogWrite(LEFT_PWM, 0);
  right_mt_Forward(RmotorSpeed - 5);
}

void rightForward()
{
  analogWrite(RIGHT_PWM, 0);
  left_mt_Forward(LmotorSpeed - 5);
}

void turnLeft()
{
  right_mt_Forward(RmotorSpeed - 5);
  left_mt_Backward(LmotorSpeed - 5);  
}

void turnRight()
{
  right_mt_Backward(RmotorSpeed - 5);
  left_mt_Forward(LmotorSpeed - 5);  
}
void leftBackward()
{
  analogWrite(LEFT_PWM, 0);
  right_mt_Backward(RmotorSpeed - 5);
}

void rightBackward()
{
  analogWrite(RIGHT_PWM, 0);
  left_mt_Backward(LmotorSpeed - 5);  
}


void stopMotor()
{
  analogWrite(LEFT_PWM, 0);
  analogWrite(RIGHT_PWM, 0);
}

void motorBreak()
{
  backward();
  delay(80);
  analogWrite(LEFT_PWM, 0);
  analogWrite(RIGHT_PWM, 0);
}


////////////////// Interrupt Funtions /////////////////////////
void leftInterrupt()
{
  hasInterrupted = true;
  rightForward();
}

void rightInterrupt()
{
  hasInterrupted = true;
  leftForward();
}

void setup() 
{
  pinMode(LEFT1, OUTPUT);
  pinMode(LEFT2, OUTPUT);
  pinMode(LEFT_PWM, OUTPUT);
  pinMode(RIGHT1, OUTPUT);
  pinMode(RIGHT2, OUTPUT);
  pinMode(RIGHT_PWM, OUTPUT);
  
  digitalWrite(RIGHT_IR,HIGH);
  digitalWrite(LEFT_IR,HIGH);

  //attachInterrupt(digitalPinToInterrupt(RIGHT_IR), rightInterrupt, LOW);
  //attachInterrupt(digitalPinToInterrupt(LEFT_IR), leftInterrupt, LOW);
  Serial.begin(9600);
  BT.begin(19200);
}

void loop() 
{
  if(hasInterrupted)
  {
    hasInterrupted = false;
    stopMotor();
  }
  if(Serial.available())
  {
    val = Serial.read();
    Serial.print(val);
    BT.print(val);
  }
  if(BT.available())
  {
    val = BT.read();
    Serial.print(val);
    if(val == '0')
    {
      stopMotor();
    }
    else if(val == '1')
    {
      forward();
    }
    else if(val == '2')
    {
      leftForward();
    }
    else if(val == '3')
    {
      rightForward();
    }
    else if(val == '4')
    {
      turnLeft();
    }
    else if(val == '5')
    {
      turnRight();
    }
    else if(val == '6')
    {
      leftBackward();
    }
    else if(val == '7')
    {
      rightBackward();
    }
    else if(val == '8')
    {
      backward();
    }
    else if(val == '9')
    {
      motorBreak();
      val = 0;
    }
    else if(val == 'a')
    {
      if(LmotorSpeed < 255)
      {
        
        LmotorSpeed += 3;
      }
    }
    else if(val == 'b')
    {
      if(LmotorSpeed > 5)
      {
        
        LmotorSpeed -= 3;
      }
    }
    else if(val == 'c')
    {
      if(LmotorSpeed < 255)
      {
        LmotorSpeed += 3;
      }
      if(RmotorSpeed < 255)
      {
        RmotorSpeed += 3;
      }
    }
    else if(val == 'd')
    {
      if(LmotorSpeed > 5)
      {
        LmotorSpeed -= 3;
      }
      if(RmotorSpeed > 5)
      {
        RmotorSpeed -= 3;
      }
    }
    else if(val == 'e')
    {
      if(RmotorSpeed < 255)
      {
        RmotorSpeed += 3;
      }
    }
    else if(val == 'f')
    {
      if(RmotorSpeed > 5)
      {
        RmotorSpeed -= 3;
      }
    }
  }
}
