#include <Arduino.h>

#include "Wheels.h"


#define SET_MOVEMENT(side,f,b) digitalWrite( side[0], f);\
                               digitalWrite( side[1], b)


#define INTINPUT0 A0
#define INTINPUT1 A1
#define BEEPING_PIN 13

volatile int cnt0, cnt1;

long int beepingFreq = 250000;

void doBeep()
{
  digitalWrite(BEEPING_PIN, digitalRead(BEEPING_PIN) ^ 1);

}

Wheels::Wheels() 
{ 
 // pinMode(BEEPING_PIN, OUTPUT);

 // cnt0=0;
 // cnt1=0;
 // speedLeft = 255;
 // speedRight = 255;

 // PCICR = 0x02;
 // PCMSK1 = 0x03;
}

void Wheels::attachRight(int pF, int pB, int pS)
{
    pinMode(pF, OUTPUT);
    pinMode(pB, OUTPUT);
    pinMode(pS, OUTPUT);
    this->pinsRight[0] = pF;
    this->pinsRight[1] = pB;
    this->pinsRight[2] = pS;
}


void Wheels::attachLeft(int pF, int pB, int pS)
{
    pinMode(pF, OUTPUT);
    pinMode(pB, OUTPUT);
    pinMode(pS, OUTPUT);
    this->pinsLeft[0] = pF;
    this->pinsLeft[1] = pB;
    this->pinsLeft[2] = pS;
}

void Wheels::setSpeedRight(uint8_t s)
{
    analogWrite(this->pinsRight[2], s);
}


void Wheels::setSpeedLeft(uint8_t s)
{
    analogWrite(this->pinsLeft[2], s);
}

void Wheels::changeSpeedLeft(uint8_t s)
{
  analogWrite(this->pinsLeft[2], (analogRead(this->pinsLeft[2]) + s) % 255);
}

void Wheels::changeSpeedRight(uint8_t s)
{
  analogWrite(this->pinsRight[2], (analogRead(this->pinsRight[2]) + s) % 255);
}

void Wheels::setSpeed(uint8_t s)
{
    setSpeedLeft(s);
    setSpeedRight(s);
}

void Wheels::attach(int pRF, int pRB, int pRS, int pLF, int pLB, int pLS)
{
    this->attachRight(pRF, pRB, pRS);
    this->attachLeft(pLF, pLB, pLS);
}

void Wheels::forwardLeft() 
{
    SET_MOVEMENT(pinsLeft, HIGH, LOW);
}

void Wheels::forwardRight() 
{
    SET_MOVEMENT(pinsRight, HIGH, LOW);
}

void Wheels::backLeft()
{
    SET_MOVEMENT(pinsLeft, LOW, HIGH);
}

void Wheels::backRight()
{
    SET_MOVEMENT(pinsRight, LOW, HIGH);
}

void Wheels::forward()
{
    //digitalWrite(BEEPING_PIN, LOW);
    //Timer1.detachInterrupt();
    this->forwardLeft();
    this->forwardRight();
}



void Wheels::back()
{
    //Timer1.detachInterrupt();
    //Timer1.attachInterrupt(doBeep, beepingFreq);
    this->backLeft();
    this->backRight();
}


void Wheels::stopLeft()
{
    SET_MOVEMENT(pinsLeft, LOW, LOW);
}

void Wheels::stopRight()
{
    SET_MOVEMENT(pinsRight, LOW, LOW);
}

void Wheels::stop()
{
    //digitalWrite(BEEPING_PIN, LOW);
    //Timer1.detachInterrupt();
    this->stopLeft();
    this->stopRight();
}

void delayCnt(int cnt)
{
  while (cnt0 < cnt && cnt1 < cnt) {
    
  }
  cnt0 = cnt1 = 0;
}

void Wheels::moveForwardCnt(int cnt)
{
  this->forward();
  delayCnt(cnt);
}

void Wheels::moveBackCnt(int cnt)
{
  this->back();
  delayCnt(cnt);
}

void Wheels::moveForward(int d)
{
  this->forward();
  delay(d);
}

void Wheels::moveBack(int d)
{
  this->back();
  delay(d);
}

void Wheels::turnRightCnt(int cnt)
{
  if (digitalRead(pinsRight[1]) == HIGH && digitalRead(pinsLeft[1]) == HIGH)
  {
    this->forwardLeft();
  }
  if (digitalRead(pinsRight[0]) == HIGH && digitalRead(pinsLeft[0]) == HIGH) 
  {
    this->backLeft();
  }
  delayCnt(cnt);
}

void Wheels::turnLeftCnt(int cnt)
{
  if (digitalRead(pinsRight[1]) == HIGH && digitalRead(pinsLeft[1]) == HIGH)
  {
    this->forwardRight();
  }
  if (digitalRead(pinsRight[0]) == HIGH && digitalRead(pinsLeft[0]) == HIGH) 
  {
    this->backRight();
  }
  delayCnt(cnt);
}

void Wheels::test()
{
  Serial.println(cnt0);
  Serial.println(cnt1);

  int c = 75;
  this->forward();
  delay(1000);
  if (cnt0 > c)
  {
    cnt0 = 0;
    this->setSpeedLeft(this->speedLeft - 10);
    this->speedLeft -= 10;
  }

  else if (cnt0 < c)
  {
    cnt0 = 0;
    this->setSpeedLeft(this->speedLeft + 10);
    this->speedLeft += 10;  
  }
    if (cnt1 > c)
  {
    cnt1 = 0;
    this->setSpeedRight(this->speedRight - 10);
    this->speedRight -= 10;
  }

  else if (cnt1 < c)
  {
    cnt1 = 0;
    this->setSpeedRight(this->speedRight + 10);
    this->speedRight += 10;  
  }

  if ( this->speedRight > 255) 
  this->speedRight = 255;
  
  if ( this->speedLeft > 255) 
  this->speedLeft = 255;
  
}

//ISR(PCINT1_vect)
//{
// if( (PINC & (1 << PC0)) ) 
// cnt0++;

// if( (PINC & (1 << PC1)) )
// cnt1++;
//}


