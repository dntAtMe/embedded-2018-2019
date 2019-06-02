/* 
 * prosta implementacja klasy obsługującej 
 * silniki pojazdu za pośrednictwem modułu L298
 *
 * Sterowanie odbywa się przez:
 * 1)  powiązanie odpowiednich pinów I/O Arduino metodą attach() 
 * 2)  ustalenie prędkości setSpeed*()
 * 3)  wywołanie funkcji ruchu
 *
 * TODO:
 *  - zabezpieczenie przed ruchem bez attach()
 *  - ustawienie domyślnej prędkości != 0
 */


#include <Arduino.h>
#include "TimerOne.h"
#include <PinChangeInterruptSettings.h>
#include <PinChangeInterruptBoards.h>
#include <PinChangeInterruptPins.h>
#include <PinChangeInterrupt.h>

#ifndef Wheels_h
#define Wheels_h



class Wheels {
    public: 
        int speedLeft;
        int speedRight;
        Wheels();
        /*
         *  pinForward - wejście "naprzód" L298
         *  pinBack    - wejście "wstecz" L298
         *  pinSpeed   - wejście "enable/PWM" L298
         */
        void attachRight(int pinForward, int pinBack, int pinSpeed);
        void attachLeft(int pinForward, int pinBack, int pinSpeed);
        void attach(int pinRightForward, int pinRightBack, int pinRightSpeed,
                    int pinLeftForward, int pinLeftBack, int pinLeftSpeed);
        /*
         *  funkcje ruchu
         */
        void forward();
        void moveForward(int);
        void moveForwardCnt(int);
        void forwardLeft();
        void forwardRight();
        void back();
        void moveBack(int);
        void moveBackCnt(int);
        void backLeft();
        void backRight();
        void stop();
        void stopLeft();
        void stopRight();
        void turnRightCnt(int);
        void turnLeftCnt(int);
        /*
         *  ustawienie prędkości obrotowej (przez PWM)
         *   - minimalna efektywna wartość 60
         *      może zależeć od stanu naładowania baterii
         */
        void setSpeed(uint8_t);
        void setSpeedRight(uint8_t);
        void setSpeedLeft(uint8_t);
        void changeSpeedLeft(uint8_t);
        void changeSpeedRight(uint8_t);
        void test();

    private: 
        int pinsRight[3];
        int pinsLeft[3];
};



#endif
