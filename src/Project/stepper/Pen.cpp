/**
 *  Pen.cpp
 *
 *  Controller for manageing the pen (servo) up and down motion for drawing.
 *
 *  @author Drew Sommer
 *  @version 1.0.1
 *  @license MIT (https://mit-license.org)
 */
#include "Pen.h"
#include <Arduino.h>
#include <Servo.h>

/**
 * Instantiate the pen with a Servo
 * @param pin  The servo pin
 * @param up   The max angle (0-180)
 * @param down The min angle (0-180)
 * @param del  The delay time (ms)
 */
Pen::Pen(int pin, int up, int down, int del)
    : _pin(pin),
      _upPos(up),
      _downPos(down),
      _del(del),
      _cur(up){};

/**
 * Attaches the Pen (call within setup())
 */
void Pen::attach() {
    _servo.attach(_pin);
}

/**
 * Set the servo to max angle
 * @return true/false if completed
 */
bool Pen::up() {
    _target = _upPos; // Set target angle
    return move();    // Move to target
}

/**
 * Set the servo to min angle
 * @return true/false if completed
 */
bool Pen::down() {
    _target = _downPos; // Set target angle
    return move();      // Move to target
};

/**
 * Set the low position (using a potentiometer)
 * @param ro read-out
 */
int Pen::setDown(int ro) {
    _downPos = ro; // Set down position
    down();        // Move to down position
    return getDown();
};

/**
 * Move from the current angle to the target angle
 * @return completed (true)
 */
bool Pen::move(){

    // Our current angle is greater than our target angle. Decrement our angle
    // till we match our target angle.
    if(_cur > _target){
        for(int i=_cur; i>=_target; i--){
            _servo.write(i);
            _cur = i;
            delay(_del);
        }

    // Our current angle is less than our target angle. Increment our angle till
    // we match our target angle.
    } else {
        for(int i=_cur; i<=_target; i++){
            _servo.write(i);
            _cur = i;
            delay(_del);
        }
    }
    return true;
}
