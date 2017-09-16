/**
 *  AnalogButtons.h
 *
 *  Manages input on a single pin for multiple buttons (2)
 *
 *  @author Drew Sommer
 *  @version 1.0.0
 *  @license MIT (https://mit-license.org)
 */
#ifndef ANALOGBUTTONS_H
#define ANALOGBUTTONS_H

/**
 * Library to interpret analog input for multiple buttons on a single pin
 * @param pin  The pin monitor
 * @param btn1 Button 1 voltage value to check for
 * @param btn2 Button 2 voltage value to check for
 */
class AnalogButtons {
private:
    int _pin;
    int _btn1;
    int _btn2;
    int _buff = 50;

public:
    /**
     * Library to interpret analog input for multiple buttons on a single pin
     * @param pin  The pin monitor
     * @param btn1 Button 1 voltage value to check for
     * @param btn2 Button 2 voltage value to check for
     * @param buff Buffer value to pad check with
     *             (btn value of 500 will trip between 450-550 when set to 50)
     */
    AnalogButtons(int pin, int btn1, int btn2, int buff);

    /**
     * Checks the current analog pins voltage
     * @return Returns the -1 for button 1, 1 for button 2, or 0 for neither
     */
    int check();
};

#endif
