#include <SoftwareSerial.h>
SoftwareSerial BT(11, 12);
const byte LEFT1 = 7;
const byte LEFT2 = 6;
const byte LEFT_PWM = 5;

const byte RIGHT1 = 9;
const byte RIGHT2 = 8;
const byte RIGHT_PWM = 10;

volatile byte LmotorSpeed = 59;
volatile byte RmotorSpeed = 55;

//const int LEFT_IR = A0;
//const int RIGHT_IR =A1;
const byte RIGHT_IR = 2;
const byte LEFT_IR = 3;

volatile boolean hasChanged_left = true;
volatile boolean Rt_Lf;
volatile unsigned long interval[3]={100000000,100000000,100000000};
volatile byte pointer = 0;
volatile byte count = 0;

unsigned long rtime;




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
  if(hasChanged_left)
  {
    hasChanged_left = false;
    analogWrite(LEFT_PWM, mts+14);
    delay(15);
  }
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
  if(hasChanged_left)
  {
    hasChanged_left = false;
    analogWrite(LEFT_PWM, mts+14);
    delay(15);
  }
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
  right_mt_Forward((RmotorSpeed));
}

void rightForward()
{
  left_mt_Forward((LmotorSpeed));
  analogWrite(RIGHT_PWM, 0);
}

void turnLeft()
{
  right_mt_Forward((RmotorSpeed));
  left_mt_Backward((LmotorSpeed-4));  
}

void turnRight()
{
  right_mt_Backward((RmotorSpeed-4));
  left_mt_Forward((LmotorSpeed));  
}
void leftBackward()
{
  analogWrite(LEFT_PWM, 0);
  right_mt_Backward((RmotorSpeed));
}

void rightBackward()
{
  analogWrite(RIGHT_PWM, 0);
  left_mt_Backward((LmotorSpeed));  
}


void stopMotor()
{
  analogWrite(LEFT_PWM, 0);
  analogWrite(RIGHT_PWM, 0);
}

void motorBreak()
{
  backward();
  delay(50);
  analogWrite(LEFT_PWM, 0);
  analogWrite(RIGHT_PWM, 0);
}



float avrg(volatile long unsigned arr[], int size)
{
  long unsigned tmp = 0;
  for(int i=0; i<size; i++)
  {
    tmp += arr[i];
  }
  return tmp/size;
}

//////////////////////// interrupt functions /////////////////////////////
void leftInterrupt()
{
  float newSpeed;
  if(!Rt_Lf)
  {
    Rt_Lf = true;
    unsigned long tmp = micros();
    pointer += 1;
    pointer %= 3;
    interval[pointer] = tmp - rtime;
    newSpeed=avrg(interval, 3);
    if(newSpeed < 18000000 && count<2)
    {
      BT.print(newSpeed);
      BT.print('L');
      BT.print(',');
      count += 1;
      LmotorSpeed -= 7;
      RmotorSpeed -= 7;

      leftForward();
    }
    else
    {
      count = 0;
      LmotorSpeed = 58;
      RmotorSpeed = 54;
    }
  }
  else
  {
    turnLeft();
    //leftForward();
  }
  
}

void rightInterrupt()
{
  float newSpeed;
  if(Rt_Lf)
  {
    Rt_Lf = false;
    unsigned long tmp = micros();
    pointer += 1;
    pointer %= 3;
    interval[pointer] = tmp - rtime;
    newSpeed=avrg(interval, 3);
    if(newSpeed < 180000000 && count<2)
    {
      BT.print(newSpeed);
      BT.print('R');
      BT.print(',');
      count += 1;
      RmotorSpeed -= 7; 
      LmotorSpeed -= 7; 
      rightForward();
    }
    else
    {
      count = 0;
      LmotorSpeed = 58;
      RmotorSpeed = 54;
    }
  }
  else
  {
    turnRight();
    //rightForward();
  }
}




void setup()
{
  pinMode(LEFT1, OUTPUT);
  pinMode(LEFT2, OUTPUT);
  pinMode(LEFT_PWM, OUTPUT);
  pinMode(RIGHT1, OUTPUT);
  pinMode(RIGHT2, OUTPUT);
  pinMode(RIGHT_PWM, OUTPUT);

  pinMode (LEFT_IR, INPUT);
  pinMode (RIGHT_IR, INPUT);
  digitalWrite(RIGHT_IR,HIGH);
  digitalWrite(LEFT_IR,HIGH);

  attachInterrupt(digitalPinToInterrupt(RIGHT_IR), rightInterrupt, LOW);
  attachInterrupt(digitalPinToInterrupt(LEFT_IR), leftInterrupt, LOW);
  Serial.begin(9600);
  BT.begin(19200);
}

void loop() 
{
  forward();
}
