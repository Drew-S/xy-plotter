/**
 *  Pen.h
 *
 *  Controller for manageing the pen (servo) up and down motion for drawing.
 *
 *  @author Drew Sommer
 *  @version 1.0.1
 *  @license MIT (https://mit-license.org)
 */
#ifndef PEN_H
#define PEN_H
#include <Servo.h>

/**
 * Pen is used to control the Arduino Servo
 * @param pin  The servo pin
 * @param up   Max angle (0-180)
 * @param down Min angle (0-180)
 */
class Pen {
private:
    bool _up = false; // Pen up or down (default down)
    int _pin;         // Control pin
    int _downPos;     // Min angle (0-180)
    int _upPos;       // Max angle (0-180)
    int _del;         // Delay time (ms)
    int _cur;         // Current angle
    int _target;      // Target angle
    Servo _servo;     // Arduino servo controller

    /**
     * Move from the current angle to the target angle
     * @return completed (true)
     */
    bool move();

public:
    /**
     * Instantiate the pen with a Servo
     * @param pin  The servo pin
     * @param up   The max angle (0-180)
     * @param down The min angle (0-180)
     * @param del  The delay time (ms)
     */
    Pen(int pin, int up, int down, int del);

    /**
     * Attaches the Pen (call within setup())
     */
    void attach();

    /**
     * Set the servo to max
     * @return true/false if completed
     */
    bool up();

    /**
     * Set the servo to min
     * @return true/false if completed
     */
    bool down();

    /**
     * Set the low position (using a potentiometer)
     * @param ro read-out
     */
    int setDown(int ro);

    /**
     * Get current down pos
     * @return int down pos
     */
    int getDown(){ return _downPos; };
};

#endif
