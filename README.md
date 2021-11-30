# Super-Simple-Servo-Controller
Speed and Direction control for a Servo-Motor with Arduino Nano and Rotary Encoder

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
// Robust Rotary Encoder Reading [ read_rotary() function ]
// Copyright John Main - best-microcontroller-projects.com
//

Other details and discussion in my blog article:
https://s4rc4sm.wordpress.com/2021/11/10/electric-servo-motor-controller/
