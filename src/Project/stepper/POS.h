/**
 *  POS.h
 *
 *  Holds struct's that are used throughout the code. Primarilly the POS struct.
 *
 *  POS is short for position. Same structure as a point or 2D vector.
 *
 *  PinMap is used to setup an EasyDriver stepper motor. Two of these are passed
 *  to the Drive, which get passed to the stepper motors. The stepper uses most
 *  of PinMap, the Drive also uses some of the values.
 *
 *  @author Drew Sommer
 *  @version 1.0.0
 *  @license MIT (https://mit-license.org)
 */
#ifndef POS_H
#define POS_H

/**
 * POS is a tuple for a 2D position (x, y)
 */
struct POS {
    int x; // X position
    int y; // Y position

};

/**
 * PinMap for the Stepper and analog buttons
 */
struct PinMap {
    // Stepper
    int step;   // Step pin   :: EasyDriver
    int dir;    // Dir pin    :: EasyDriver
    int enable; // Enable pin :: EasyDriver
    // Analog Buttons (used by Drive)
    int btnPin; // Analog read pin
    int btn1;   // Resistance value for btn1 (0-1024)
    int btn2;   // Resistance value for btn2 (0-1024)
    int buff;   // Buffer value (default 50) (for button resistance)
    int flip;
};

#endif
