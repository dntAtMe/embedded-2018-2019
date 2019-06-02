#include "Wheels.h"
#include <IRremote.h>

#include <Servo.h>

#define RECV_PIN 2
#define TRIG A4
#define ECHO A5
#define SERVO 3

Wheels w;
Servo servo;
IRrecv irrecv(RECV_PIN);
volatile char cmd;
decode_results results;
int passed = 0;
int stopped = 0;
int a = 0;
int mov = 0;


void setup() {
  pinMode(TRIG, OUTPUT);    
  pinMode(ECHO, INPUT);     
  pinMode(LED_BUILTIN, OUTPUT);

  // put your setup code here, to run once:
  Serial.begin(9600);

  w.attach(4,5,6,8,10,11);
  servo.attach(SERVO);
  //Timer1.initialize();

  Serial.println("Enabling IRin");
  irrecv.enableIRIn(); // Start the receiver
  Serial.println("Enabled IRin");

  w.setSpeed(0);

  while (passed < 3) {
    if (irrecv.decode(&results)) {
      Serial.println(results.value, HEX);
      if (passed == 0 && results.value == 0xFFA25D)
        passed++;
      if (passed == 1 && results.value == 0xFFE01F)
        passed++;
      if (passed == 2 && results.value == 0xFF38C7)
        passed++;
      irrecv.resume();
    }
  }
}

void read()
{
  
  while(Serial.available())
  {
    
    cmd = Serial.read();
    switch(cmd)
    {
      case 'w': w.forward(); break;
      case 's': w.back(); break;
      case 'a': w.forwardLeft(); break;
      case 'd': w.forwardRight(); break;
      case 'z': w.backLeft(); break;
      case 'c': w.backRight(); break;
      case 'p': w.stop(); break;
      case '1': w.setSpeedLeft(w.speedLeft - 10); w.speedLeft -= 10;break;
      case '2': w.setSpeedLeft(w.speedLeft + 10); w.speedLeft += 10;break;
      case '3': w.setSpeedRightint(w.speedRight - 10); w.speedRight -= 10;break;
      case '4': w.setSpeedRight(w.speedRight + 10); w.speedRight += 10;break;
      case '5': w.setSpeed(255); w.speedLeft = 255; w.speedRight = 255;break;
      
    }
  }
}

int check(int angle)
{
  unsigned long tot;
  unsigned long dist;
  //Serial.print("Angle ");
  //Serial.print(angle);
  servo.write(angle);

  digitalWrite(TRIG, HIGH);
  delay(10);
  digitalWrite(TRIG, LOW);
  tot = pulseIn(ECHO, HIGH);

  dist = tot/58;

  //Serial.print(", Distance ");
  //Serial.println(dist);

  delay(100);

  return dist;
}

void loop() {
  if (irrecv.decode(&results)) 
  {
    Serial.println(results.value, HEX);
    if (passed == 0 && results.value == 0xFFA25D)
      passed++;
    if (passed == 1 && results.value == 0xFFE01F)
      passed++;
    if (passed == 2 && results.value == 0xFF38C7)
      passed++;
    if (mov != 0 && results.value == 0xFF38C7)
    {
      mov = 0;
      w.stop();
    }
    if(!stopped && (results.value == 0xFF10EF || (results.value == 0xFFFFFFFF && mov == 3)))
    {
      mov = 3;
      w.backLeft();
      w.forwardRight();
    }
    if(!stopped && (results.value == 0xFF5AA5 || (results.value == 0xFFFFFFFF && mov == 2)))
    {
      mov = 2;
      w.backRight();
      w.forwardLeft();
    }
    if(!stopped && (results.value == 0xFF18E7 || (results.value == 0xFFFFFFFF && mov == 1)))
    {
      if (mov == 4)
      {
        w.stop();
        mov = 0;
      }
      else
      {
      mov = 1;
      w.forward();
      }
    }
    if (!stopped && results.value == 0xFF22DD)
    {
      w.setSpeedLeft(w.speedLeft = 10); 
      w.speedLeft += 10;
    }
    if (!stopped && results.value == 0xFFB04F)
    {
      w.setSpeedLeft(w.speedLeft - 10); 
      w.speedLeft -= 10;
    }
    if (!stopped && results.value == 0xFF4AB5)
    {
      if(mov == 1)
      {
        w.stop();
        mov = 0; 
      }
      else
      {
        w.back();
        mov = 4;
      }
    }
    if (results.value == 0xFF6897)
    {
      a = 1 - a;
      digitalWrite(LED_BUILTIN, a ? HIGH : LOW);
      w.stop();
    }
    irrecv.resume();
  }
  
  if (passed == 3)
  {
    w.setSpeed(stopped ? 255 : 0);
    w.stop();
    stopped = 1 - stopped;
    passed = 0;
  }

  delay(100);
  if (a)
  {
    
    
    int rot = 63;
    int step = 100;
  
    int d = 70;
    int maxD = 0;
    int maxS = 0;
    read();
    if (check(90) < d )
    {
      w.stop();
      int found = 0;
      for(int angle = 110; angle < 180; angle += 20)
      {
        int curD = check(angle);
        if (curD > maxD)
        {
          maxS = 0;
          maxD = curD;  
        }
  
      }
  
        for(int angle = 70; angle > 0; angle -= 20)
      {
         int curD = check(angle);
          if (curD > maxD)
          {
            maxS = 1;
            maxD = curD;  
          }
  
      }
        if(maxD >= d && maxS == 1)
        {
          w.backRight();
          w.forwardLeft();
          delay(100);
        }      
        if(maxD >= d && maxS == 0)
        {
          w.backLeft();
          w.forwardRight();
          delay(100);
        }
      
      
    }
    else 
    {
      w.forward();
  
    }
  }

}
