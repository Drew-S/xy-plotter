/**
 *  AnalogButtons.cpp
 *
 *  Manages input on a single pin for multiple buttons (2)
 *
 *  @author Drew Sommer
 *  @version 1.0.1
 *  @license MIT (https://mit-license.org)
 */
#include <Arduino.h>
#include "AnalogButtons.h"

/**
 * Library to interpret analog input for multiple buttons on a single pin
 * @param pin  The pin monitor
 * @param btn1 Button 1 voltage value to check for
 * @param btn2 Button 2 voltage value to check for
 * @param buff Buffer value to pad check with
 *             (btn value of 500 will trip between 450-550 when set to 50)
 */
AnalogButtons::AnalogButtons(int pin, int btn1, int btn2, int buff)
    : _pin(pin),
      _btn1(btn1),
      _btn2(btn2),
      _buff(buff){}

/**
 * Checks the current analog pins voltage
 * @return Returns the -1 for button 1, 1 for button 2, or 0 for neither
 */
int AnalogButtons::check() {

    // Read our pin
    int ch = analogRead(_pin);

    // Check if our first button is pressed
    if(_btn1 - _buff <= ch && ch < _btn1 + _buff) {
        return -1;

    // Check if our second button is pressed
    } else if(_btn2 - _buff <= ch && ch < _btn2 + _buff) {
        return 1;
    }

    // No button is pressed
    return 0;
}
