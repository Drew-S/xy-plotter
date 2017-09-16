/**
 *  Stepper.h
 *
 *  Controller for a single stepper motor. Controls, forward and backward motion
 *  and its integer position. Uses http://www.schmalzhaus.com/EasyDriver/index.html
 *  EasyDriver to control the stepper.
 *
 *  @author Drew Sommer
 *  @version 1.0.0
 *  @license MIT (https://mit-license.org)
 */
#ifndef STEPPER_H
#define STEPPER_H
#include "POS.h"

/**
 * Stepper controller for the easy driver
 * @param map a PinMap of the pins for the stepper, check POS.h for structure
 */
class Stepper {
private:
    int _del;                 // delay (ms)
    int _currentPos  = 0;     // current pos

    int _step;                // step pin
    bool _stepMode   = false; // step's current mode (LOW/HIGH)

    int _dir;                 // dir pin
    bool _dirMode    = false; // dir's current mode (LOW/HIGH)

    int _enable;              // enable pin
    bool _enableMode = false; // enable's current mode (LOW/HIGH)

    int _ms1;                 // micro step 1 pin
    bool _ms1Mode    = false; // micro step 2's current mode (LOW/HIGH)

    int _ms2;                 // micro step 2 pin
    bool _ms2Mode    = false; // micro step 2's current mode (LOW/HIGH)

    bool _ms         = false; // for hardwired micro stepping 1/8 mode

    bool _flip       = false;

public:
    /**
     * Instantiate a new stepper with the PinMap
     * @param map the pins for the stepper to manage
     */
    Stepper(PinMap map, int del);

    /**
     * Instantiate a new stepper with the PinMap
     * @param map the pins for the stepper to manage
     * @param ms1 the microstepping pin 1
     * @param ms2 the microstepping pin 2
     */
    Stepper(PinMap map, int del, int ms1, int ms2);

    /**
     * Move's the stepper forwardx x number of steps
     * @return the new currentPos
     */
    int forward();

    /**
     * Move's the stepper backwards x number of steps
     * @return the new currentPos
     */
    int backward();

    /**
     * Sets the microstepping value
     * @param  num 1 = full step, 2 = 1/2 step, 4 = 1/4 step, 8 = 1/8 step
     * @return     the new microstepping balue, 1,2,4,8 or 0, 0 if no change
     */
    int setMS(int num);

    /**
     * Overwrites the steppers currentPos
     * @param  num the overwrite value
     * @return     true/false when complete
     */
    bool setPOS(int num);

    /**
     * Used to setup the Stepper (called within steup())
     */
    void attach();
};

#endif
