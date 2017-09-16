/**
 *  Drive.h
 *
 *  Drive controller, manages each stepper motor and servo.
 *  Maintains control over X and Y positions, movement along the x and y-axis.
 *  Maintains control over the pens up and down position
 *
 *  @author Drew Sommer
 *  @version 1.0.0
 *  @license MIT (https://mit-license.org)
 */
#ifndef DRIVE_H
#define DRIVE_H
#include "stepper/POS.h"
#include "stepper/Stepper.h"
#include "stepper/Pen.h"
#include "stepper/AnalogButtons.h"
#include <Arduino.h>

/**
 * Drive controller, used to control both steppers and servo, and reads
 *   interupts from the AnalogButtons
 */
class Drive {
private:
    int _del;           // delay (ms)
    POS _xy = { 0, 0 }; // current post
    Stepper _x;         // X direction stepper
    Stepper _y;         // Y direction stepper
    AnalogButtons _abx; // Interupts for X extremes
    AnalogButtons _aby; // Interupts for Y extremes
    Pen _pen;           // Servo controller

    /**
     * Private controller that does the actual moving
     * @param  x  New X position
     * @param  y  New Y position
     * @param  up Move pen up (true, dont draw) or down (false, draw)
     * @return    The updated POS
     */
    POS move(int x, int y, bool up);

public:
    /**
     * Driver constructor (singleton)
     * @param x     PinMap for X direction
     * @param y     PinMap for Y direction
     * @param del   Delay (ms)
     * @param servo Servo control pin
     * @param up    Max angle (0-180)
     * @param down  Min angle (0-180)
     */
    Drive(PinMap x, PinMap y, int del, int servo, int up, int down);

    /**
     * Used to setup Drive (call within setup())
     */
    void attach();

    /**
     * Draw a line from current POS to new POS
     * @param  x New X position
     * @param  y New Y position
     * @return   Updated POS
     */
    POS lineTo(int x, int y);

    /**
     * Move the pen from current POS to new POS
     * @param  x New X position
     * @param  y New Y position
     * @return   Updated POS
     */
    POS moveTo(int x, int y);

    /**
     * Return the pen to origin point (0,0)
     * @return Updated POS (0,0)
     */
    POS origin();

    POS get();
};

#endif
