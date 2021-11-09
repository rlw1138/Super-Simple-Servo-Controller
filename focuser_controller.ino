//
// Servo Motor Controller -- 2021-NOV-05 Russ Williams rlw1138#hotmail#com
//
/*  Speed and Direction control of a 'continuous' servo-motor, using an 
    Incremental (quadrature-coded) Rotary Encoder and Arduino MCU

    Rotate the encoder one 'detent' to start the servo turning slowly.
    Keep rotating (or resume rotating) to go to the next faster speed.
    Rotate opposite (or press the button, if equipped) to stop.

    Make sure to provide adequate power to the servo.

    See note at the bottom, "Regarding Speed Values"
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

#include <Servo.h> 

Servo myservo;  // create servo object to control a servo 

#define CLK 7
#define DATA 4 
#define CONTROLPIN 6
#define BUTTON 5
#define BAUD 9600
#define STOP_MS 1500
#define STOP_DEGREES 90
#define IDLE 0
#define CW 1
#define CCW -1

void setup() {
  myservo.attach(CONTROLPIN, 500, 2500);  //pin for servo control, OPT: range in mS
  full_stop(); // just in case

  pinMode(CLK, INPUT);
  pinMode(CLK, INPUT_PULLUP); //stabilizes the voltage of the pin
  pinMode(DATA, INPUT);
  pinMode(DATA, INPUT_PULLUP);
  pinMode(BUTTON, INPUT);
  pinMode(BUTTON, INPUT_PULLUP);

  //to help keep track of what's loaded on the arduino
  Serial.begin(BAUD);
  Serial.println("Servo-motor Telescope Focuser Controller");
  Serial.end();
}

// globals
static uint8_t prevNextCode = 0;
static uint16_t store = 0;

void loop() {
    static int8_t val;
    static int8_t savedDirection = 0; //zero for Idle, 1 for CW, -1 for CCW
    static int8_t currentDirection = 0; // rotation direction, 0=idle 1=CW -1=CCW
    static int8_t n = 0; //index to speed arrays
    static int8_t servoCmd = 0; //value to send to servo

// For Solar Newt
//    #define STOP_MS 1490
//    #define STOP_DEGREES 89
//    // TEN servo speed values (in Degrees) for each Direction, from Slow to Fast
//    const int8_t servo_speed_CW[] = {91, 92, 93, 94, 95, 96, 98, 100, 110, 120};
//    const int8_t servo_speed_CCW[] = {86, 85, 84, 83, 82, 76, 70, 60, 50, 40};

    // For 5" Maksutov
    //    #define STOP_MS 1500
    //    #define STOP_DEGREES 90
    //TEN servo speed values (in Degrees) for each Direction, from Slow to Fast
    const int8_t servo_speed_CW[] = {92, 93, 94, 95, 96, 98, 99, 100, 100, 100};
    const int8_t servo_speed_CCW[] = {86, 85, 84, 83, 82, 81, 80, 79, 79, 79};

        
    if( val=read_rotary() ) {
        if ( prevNextCode==0x0b) {
            currentDirection = CW;
        }
        else {
            //(prevNextCode==0x07)
            currentDirection = CCW;
        }
        // same direction, or first time thru the loop
        if ( currentDirection ==  savedDirection || savedDirection == 0 ) {
            if ( currentDirection == CW) {
              servoCmd = servo_speed_CW[n];
            }
            else {
              servoCmd = servo_speed_CCW[n];
            }
        
            //OPTIONAL -- your motor MAY need this to function correctly (un-comment if needed)
            //myservo.writeMicroseconds(STOP_MS); //set initial servo position (stopped)
            
            myservo.write(servoCmd); // send "rotation angle" in degrees, to set speed
            savedDirection =  currentDirection;
            if (n <= 8) n += 1; // there are ten speeds, 0 <-> 9
        } 
        else {
            // user reversed direction so STOP!
            full_stop();
            n = 0;
            savedDirection = 0;
        }
    }
    
    // button pressed so STOP!
    if (digitalRead(BUTTON)==LOW) {
      delay(10); //make sure button-press is intentional (crude de-bounce)
      if (digitalRead(BUTTON)==LOW) {
          full_stop();
          while(digitalRead(BUTTON)==LOW);//waits for button to be released
      }
    }
} // END MAIN LOOP


void full_stop() {
    myservo.writeMicroseconds(STOP_MS); //set initial servo position (stopped)
    delay(60);
    myservo.writeMicroseconds(STOP_MS); //set initial servo position (stopped)
}


// BEWARE! This function uses tricksy mathemagical devilry! 
// Best not to meddle with the sorcery that follows.
// A valid CW or CCW move returns 1, invalid returns 0.

int8_t read_rotary() {
  const int8_t rot_enc_table[] = {0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0};

  prevNextCode <<= 2;
  if (digitalRead(DATA)) prevNextCode |= 0x02;
  if (digitalRead(CLK)) prevNextCode |= 0x01;
  prevNextCode &= 0x0f;

   // If valid then store as 16 bit data.
   if  (rot_enc_table[prevNextCode] ) {
      store <<= 4;
      store |= prevNextCode;
      //if (store==0xd42b) return 1;
      //if (store==0xe817) return -1;
      if ((store&0xff)==0x2b) return -1;
      if ((store&0xff)==0x17) return 1;
   }
   return 0;
}

/*

REGARDING DEGREES & MICROSECONDS

The "stop" command uses 'writeMicroseconds()' which gives a little bit better control. The 'attach()'
command tells the motor to use a 'range' of 500 to 2500 microseconds, with 1500 (stop) being right in 
the middle. My continuous motor works best with a 'stop' value of 1490. You could also use 90 (or 89, 
or 91, depending) degrees as the value for "myservo.write(STOP_DEGREES)"

REGARDING SPEED VALUES

Values are given in "degrees from Stopped Position" (which is 90-ish degrees).
180 degrees is "fastest clockwise" and 0 degrees is "slowest counterclockwise" (although you may
not see any difference between 0 and 40, or between 120 and 180).

The farther the "command value" is from 'stopped position' the faster the servo will turn. 

ASCENDING values tell the motor to turn Clockwise, and DESCENDING values tell it Counter-clockwise.  

Depending on your particular 'continuous' motor, you MAY have to command the motor to STOP between 
speed changes (and some speeds, especially fastest or slowest, may not work). 

A 'real' continuous motor will have a potentiometer that sets the "stop" position. You can modify a 
normal servo to allow continuous rotation -- if you do, try to use a pot to do it.

You might also have to tweak my speed values to suit your motor. Have Fun!

*/
