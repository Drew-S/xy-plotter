/**
 *  Pen.h
 *
 *  Controller for manageing the pen (servo) up and down motion for drawing.
 *
 *  @author Drew Sommer
 *  @version 1.0.0
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
    int _del;
    int _cur;
    Servo _servo;     // Arduino servo controller

public:
    /**
     * Instantiate the pen with a Servo
     * @param pin  The servo pin
     * @param up   The max angle (0-180)
     * @param down The min angle (0-180)
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
};

#endif
