/**
 *  Pen.cpp
 *
 *  Controller for manageing the pen (servo) up and down motion for drawing.
 *
 *  @author Drew Sommer
 *  @version 1.0.0
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
 * Set the servo to max
 * @return true/false if completed
 */
bool Pen::up() {
    if(_upPos > _downPos) {
        for(int i=_cur; i<_upPos; i++){
            _servo.write(i);
            _cur = i;
            delay(_del);
        }
    } else {
        for(int i=_cur; i>_upPos; i--){
            _servo.write(i);
            _cur = i;
            delay(_del);
        }
    }
    _up = true;
    return _up;
}

/**
 * Set the servo to min
 * @return true/false if completed
 */
bool Pen::down() {
    if(_downPos > _upPos) {
        for(int i=_cur; i<_downPos; i++){
            _servo.write(i);
            _cur = i;
            delay(_del);
        }
    } else {
        for(int i=_cur; i>_downPos; i--){
            _servo.write(i);
            _cur = i;
            delay(_del);
        }
    }
    _up = false;
    return _up;
}
