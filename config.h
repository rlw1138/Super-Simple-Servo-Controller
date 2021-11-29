/*
Configuration of servo-controller SPEED SETTINGS


The fact that I mostly use my servo-controllers for my telescopes is
reflected in the various "//comments" below

If six variations are too few, you can certainly add as many as needed.
*/

#define CONTROLLER_FIRST 1
#define Controller_01    1    // Maksutov_127
#define Controller_02    2    // Solar_152
#define Controller_03    3    // Lunt_50
#define Controller_04    4    // AT10D_254
#define Controller_05    5    // OrionST_120
#define Controller_06    6    // BresserAR_80
#define CONTROLLER_LAST  6

#ifndef CONTROLLER
  #error "Configuration Error(1): A CONTROLLER has not been specified!"
#elif CONTROLLER < CONTROLLER_FIRST || CONTROLLER > CONTROLLER_LAST
  #error "Configuration Error(2): You must choose a Valid CONTROLLER!"
#endif

//
// TEN servo speed values (in Degrees) for each Direction, from Slow to Fast
//

#if CONTROLLER == 1        // for 5" Maksutov
// un-comment next line, if you need to reverse functionality
//  #define DIRECTION_SWAP
  #define STOP_MS 1500
  #define STOP_DEGREES 90
  const int8_t servo_speed_CW[] = {92, 93, 94, 95, 96, 98, 99, 100, 100, 100};
  const int8_t servo_speed_CCW[] = {86, 85, 84, 83, 82, 81, 80, 79, 79, 79};
  #warning "INFO: Compiling for MAK_5"

#elif CONTROLLER == 2      // for Solar Newtonian
// un-comment next line, if you need to reverse functionality
//  #define DIRECTION_SWAP
  #define STOP_MS 1490
  #define STOP_DEGREES 89
  const int8_t servo_speed_CW[] = {91, 92, 93, 94, 95, 96, 98, 100, 110, 120};
  const int8_t servo_speed_CCW[] = {86, 85, 84, 83, 82, 76, 70, 60, 50, 40};
  #warning "INFO: Compiling for SOLAR_6"

#elif CONTROLLER == 3      // for Lunt LS50tHa
//  #define DIRECTION_SWAP
  #define STOP_MS 1500
  #define STOP_DEGREES 90
  const int8_t servo_speed_CW[] =  {93, 94, 95, 96, 97, 98, 100, 104, 110, 120};
  const int8_t servo_speed_CCW[] = {87, 86, 85, 84, 83, 76, 70, 60, 50, 40};
  #warning "INFO: Compiling for LUNT_50"

// #elif CONTROLLER == 4     // for Astro-Tech 10" Dob
// #elif CONTROLLER == 5     // for Orion short-tube 5"
 #elif CONTROLLER == 6     // for Bresser 80mm achro
//  #define DIRECTION_SWAP
  #define STOP_MS 1500
  #define STOP_DEGREES 90
  const int8_t servo_speed_CW[] = {91, 92, 93, 94, 95, 96, 98, 100, 110, 120};
  const int8_t servo_speed_CCW[] = {89, 88, 87, 86, 85, 84, 80, 76, 70, 60};
  #warning "INFO: Compiling CONTROLLER_06"

#else
  #error "Configuration Error(3): The selected CONTROLLER's settings are not yet defined!"
#endif

/*

REGARDING SPEED VALUES

Values are given in "degrees from Stopped Position" (which is 90-ish degrees). 180 degrees is "fastest clockwise" and 0 degrees is "fastest counterclockwise" (although you may not see any difference between 0 and 40, or between 120 and 180).

The farther the "command value" is from 'stopped position' the faster the servo will turn.

ASCENDING values tell the motor to turn Clockwise, and DESCENDING values tell it Counter-clockwise.

Depending on your particular 'continuous' motor, you MAY have to command the motor to STOP between speed changes (and some speeds, especially fastest or slowest, may not work).

A 'real' continuous motor will have a potentiometer that sets the "stop" position. You can modify a normal servo to allow continuous rotation -- if you do, try to use a pot to do it (rather than resistors).


REGARDING DEGREES & MICROSECONDS

The "stop" command uses 'writeMicroseconds()' which gives a little bit better control.

The 'attach()' command tells the motor to use a 'range' of 1000 to 2000 microseconds, with 1500 (stop) being right in the middle.

Some motors can respond to a range as wide as 550 to 2500, so experiment if you like.

One of my continuous motors works best with a 'stop' value of 1490. You could also use 90 (or 89, or 91, depending) degrees as the value for "myservo.write(STOP_DEGREES)"

BE SURE TO TEST YOUR SETTINGS "UNDER LOAD"! The motor may start up and run fine with no load attached. Once you put everything together......well...

You might also have to tweak my speed values (in config.h) to suit your motor. Have Fun!

*/
