/**
 *  Stepper.cpp
 *
 *  Controller for a single stepper motor. Controls, forward and backward motion
 *  and its integer position. Uses http://www.schmalzhaus.com/EasyDriver/index.html
 *  EasyDriver to control the stepper.
 *
 *  @author Drew Sommer
 *  @version 1.0.1
 *  @license MIT (https://mit-license.org)
 */
#include <Arduino.h>
#include "Stepper.h"

/**
 * Instantiate a new stepper with the PinMap
 * @param map the pins for the stepper to manage
 */
Stepper::Stepper(PinMap map, int del) // TODO: remove AB
    : _step(map.step), _dir(map.dir), _enable(map.enable), _flip(map.flip), _del(del) {}

/**
 * Instantiate a new stepper with the PinMap
 * @param map the pins for the stepper to manage
 * @param ms1 the microstepping pin 1
 * @param ms2 the microstepping pin 2
 */
Stepper::Stepper(PinMap map, int del, int ms1, int ms2) // TODO: remove AB
    : _ms(true), _step(map.step), _dir(map.dir), _enable(map.enable), _flip(map.flip), _del(del),
    _ms1(ms1), _ms2(ms2){}

/**
 * Used to setup the Stepper (called within steup())
 */
void Stepper::attach() {

    pinMode(_step, OUTPUT);
    pinMode(_dir, OUTPUT);
    pinMode(_enable, OUTPUT);
    if(_ms) {
        pinMode(_ms1, OUTPUT);
        pinMode(_ms2, OUTPUT);
    }
}

/**
 * Move's the stepper forward a step
 * @return the new currentPos
 */
int Stepper::forward() {
    digitalWrite(_enable, LOW);
    _enableMode = false;
    if(_flip == 1) digitalWrite(_dir, HIGH);
    else digitalWrite(_dir, LOW);
    _dirMode = false;

    digitalWrite(_step, HIGH);
    delay(_del);
    digitalWrite(_step, LOW);
    delay(_del);
    _currentPos++;

    digitalWrite(_enable, HIGH);
    _enableMode = true;

    return _currentPos;
}

/**
 * Move's the stepper backwards a step
 * @return the new currentPos
 */
int Stepper::backward() {
    digitalWrite(_enable, LOW);
    _enableMode = false;
    if(_flip == 1) digitalWrite(_dir, LOW);
    else digitalWrite(_dir, HIGH);
    _dirMode = true;

    digitalWrite(_step, HIGH);
    delay(_del);
    digitalWrite(_step, LOW);
    delay(_del);
    _currentPos--;

    digitalWrite(_enable, HIGH);
    _enableMode = true;
    return _currentPos;
}

/**
 * Sets the microstepping value
 * @param  num 1 = full step, 2 = 1/2 step, 4 = 1/4 step, 8 = 1/8 step
 * @return     the new microstepping balue, 1,2,4,8 or 0, 0 if no change
 */
int Stepper::setMS(int num) {
    if (!_ms) return 0;
    switch (num) {
        case 1:
            digitalWrite(_ms1, LOW);
            digitalWrite(_ms2, LOW);
            _ms1Mode = false;
            _ms2Mode = false;
        return 1;

        case 2:
            digitalWrite(_ms1, HIGH);
            digitalWrite(_ms2, LOW);
            _ms1Mode = true;
            _ms2Mode = false;
        return 2;

        case 4:
            digitalWrite(_ms1, LOW);
            digitalWrite(_ms2, HIGH);
            _ms1Mode = false;
            _ms2Mode = true;
        return 4;

        case 8:
            digitalWrite(_ms1, HIGH);
            digitalWrite(_ms2, HIGH);
            _ms1Mode = true;
            _ms2Mode = true;
        return 8;

        default:
        return 0;
    }
}

/**
 * Overwrites the steppers currentPos
 * @param  num the overwrite value
 * @return     true/false when complete
 */
bool Stepper::setPOS(int num) {
    _currentPos = num;
    return true;
}
