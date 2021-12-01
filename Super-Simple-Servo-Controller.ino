//
// Servo Motor Controller -- 2021-NOV-05 Russ Williams rlw1138#hotmail#com
//
/*  Speed and Direction control of a 'continuous' servo-motor, using an
    Incremental (quadrature-coded) Rotary Encoder and Arduino MCU

    Rotate the encoder one 'detent' to start the servo turning slowly.
    Keep rotating (or resume rotating) to go to the next faster speed.
    Rotate opposite (or press the button, if equipped) to stop.

    Make sure to provide adequate power to the servo.

    See notes at the bottom of config.h: "Regarding Speed Values"
*/
//
//    Copyright (C) 2021  Russ Williams
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//
// Robust Rotary Encoder Reading [ read_rotary() function ]
// Copyright John Main - best-microcontroller-projects.com
//

// choose the specific controller (Controller_01 thru Controller_06;)
#define CONTROLLER Controller_02
#include "config.h" // speed settings for the different controller units

//#define DeBuG
#define CLK 7
#define DATA 4
#define CONTROLPIN 6
#define BUTTON 5
#define BAUD 9600
#define IDLE 0
#ifdef DIRECTION_SWAP
    // 'swapped' values
    #define CW -1
    #define CCW 1
#else // 'normal' values
    #define CW 1
    #define CCW -1
#endif

#include <Servo.h>
Servo myservo;  // create servo object to control a servo

// number of defined speed settings
int8_t speeds_CW  = ((sizeof servo_speed_CW)  / (sizeof servo_speed_CW[0])  );
int8_t speeds_CCW = ((sizeof servo_speed_CCW) / (sizeof servo_speed_CCW[0]) );
static int8_t servoCmd = 0; //value to send to servo
static int8_t n = 0; //index to speed arrays
static int8_t savedDirection = 0; //zero for Idle, 1 for CW, -1 for CCW


void setup() {
  myservo.attach(CONTROLPIN, 1000, 2000);  //servo control pin, range in mS

  full_stop(); // just in case

  pinMode(CLK, INPUT);
  pinMode(CLK, INPUT_PULLUP); //stabilizes the voltage of the pin
  pinMode(DATA, INPUT);
  pinMode(DATA, INPUT_PULLUP);
  pinMode(BUTTON, INPUT);
  pinMode(BUTTON, INPUT_PULLUP);

  //to help keep track of what's loaded on the arduino
  Serial.begin(BAUD);
  Serial.println("Servo-motor Speed/Direction Controller");
  #ifndef DeBuG
    Serial.end();
  #endif

  #ifdef DeBuG
  Serial.print(speeds_CW);
  Serial.println(" speeds CW");
  Serial.print(speeds_CCW);
  Serial.println(" speeds CCW");
  #endif
}


void loop() {
    int8_t val;
    int8_t limit;
    int8_t currentDirection = 0; // rotation direction, 0=idle 1=CW -1=CCW

    if ( val = read_rotary() ) {

        if ( val == CW ) {
            currentDirection = CW;
            limit = speeds_CW -2;
        }
        else {
            //( val == -1 )
            currentDirection = CCW;
            limit = speeds_CCW -2;
        }

        // same direction, or first time thru the loop
        if ( currentDirection == savedDirection || savedDirection == 0 ) {
            if ( currentDirection == CW ) {
              servoCmd = servo_speed_CW[n];
          }
            else {
              servoCmd = servo_speed_CCW[n];
            }

            /* OPTIONAL -- your motor MAY need this to function correctly (un-comment if needed) */
            //myservo.writeMicroseconds(STOP_MS); //set initial servo position (stopped)

            myservo.write(servoCmd); // send "rotation angle" in degrees, to set speed
            //myservo.writeMicroseconds(servoCmd); // send "rotation angle" in microSeconds, to set speed
            #ifdef DeBuG
              Serial.print(servoCmd);
              Serial.print(" # ");
              Serial.println(currentDirection);
            #endif
            savedDirection =  currentDirection;
            if ( n <= limit ) n += 1; // only allow the defined speeds
        }
        else {
            // user reversed direction so STOP!
            full_stop();
            #ifdef DeBuG
              Serial.println("reverse STOP!");
            #endif
        }
    }

    // button pressed so STOP!
    if (digitalRead(BUTTON)==LOW) {
      delay(10); //make sure button-press is intentional (crude de-bounce)
      if (digitalRead(BUTTON)==LOW) {
          full_stop();
          #ifdef DeBuG
            Serial.println("button STOP!");
          #endif
          while(digitalRead(BUTTON)==LOW);//waits for button to be released
      }
    }
} // END MAIN LOOP


void full_stop() {
    myservo.writeMicroseconds(STOP_MS);
    delay(60);
    myservo.writeMicroseconds(STOP_MS);
    servoCmd = 0; //value to send to servo
    n = 0;
    savedDirection = 0;
}


// BEWARE! This function uses tricksy mathemagical devilry!
// Best not to meddle with the sorcery that follows.
// A valid move CW returns 1, CCW returns -1, invalid returns 0.

int8_t read_rotary() {
  const int8_t rot_enc_table[] = {0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0};
  static int8_t prevNextCode = 0;
  static int16_t store = 0;

  prevNextCode <<= 2;
  if (digitalRead(DATA)) prevNextCode |= 0x02;
  if (digitalRead(CLK)) prevNextCode |= 0x01;
  prevNextCode &= 0x0f;

   // If valid then store as 16 bit data.
   if  (rot_enc_table[prevNextCode] ) {
      store <<= 4;
      store |= prevNextCode;
      if ((store&0xff)==0x2b) return -1;
      if ((store&0xff)==0x17) return 1;
   }
   return 0;
} // Copyright John Main - best-microcontroller-projects.com

/* end */
