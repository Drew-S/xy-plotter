/**
 *  Drive.cpp
 *
 *  Drive controller, manages each stepper motor and servo.
 *  Maintains control over X and Y positions, movement along the x and y-axis.
 *  Maintains control over the pens up and down position
 *
 *  @author Drew Sommer
 *  @version 1.0.1
 *  @license MIT (https://mit-license.org)
 */
#include <Arduino.h>
#include "Drive.h"

/**
 * Driver constructor (singleton)
 * @param x     PinMap for X direction
 * @param y     PinMap for Y direction
 * @param del   Delay (ms)
 * @param servo Servo control pin
 * @param up    Max angle (0-180)
 * @param down  Min angle (0-180)
 * @param lcd   LCD controller
 */
Drive::Drive(
    PinMap x,
    PinMap y,
    int del,
    int servo,
    int up,
    int down,
    LiquidCrystal *lcd
):    _x(x, del),
      _y(y, del),
      _abx(x.btnPin, x.btn1, x.btn2, x.buff),
      _aby(y.btnPin, y.btn1, y.btn2, y.buff),
      _del(del),
      _pen(servo, up, down, del),
      _lcd(lcd){};

/**
 * Driver constructor (singleton)
 * @param x     PinMap for X direction
 * @param y     PinMap for Y direction
 * @param del   Delay (ms)
 * @param servo Servo control pin
 * @param up    Max angle (0-180)
 * @param down  Min angle (0-180)
 * @param lcd   LCD controller
 * @param p     Bool print info
 */
Drive::Drive(
    PinMap x,
    PinMap y,
    int del,
    int servo,
    int up,
    int down,
    LiquidCrystal *lcd,
    bool p
):    _x(x, del),
      _y(y, del),
      _abx(x.btnPin, x.btn1, x.btn2, x.buff),
      _aby(y.btnPin, y.btn1, y.btn2, y.buff),
      _del(del),
      _pen(servo, up, down, del),
      _lcd(lcd),
      _p(p) {};

/**
* Used to setup Drive (call within setup())
*/
void Drive::attach() {
    _pen.attach();
    _x.attach();
    _y.attach();
};

/**
 * Draw a line from current POS to new POS
 * @param  x New X position
 * @param  y New Y position
 * @return   Updated POS
 */
POS Drive::lineTo(int x, int y) {
    return move(x, y, false);
};

/**
 * Move the pen from current POS to new POS
 * @param  x New X position
 * @param  y New Y position
 * @return   Updated POS
 */
// BUG: moveTo takes weird path to get to spot. Possibly because of large space:
// (0,0) -> (400, 400), while shapes like Ellipse and Circle only increment
// with lineTo a single x value at a time.
//
// This does not persist with Polygons however, which should be suffering from
// same symptoms.
POS Drive::moveTo(int x, int y) {
    return move(x, y, true);
};

/**
 * Return the pen to origin point (0,0)
 * @return Updated POS (0,0)
 */
POS Drive::origin() {

    // Check if collided with extreme
    int x = _abx.check();
    int y = _aby.check();

    // Raise pen, stop drawing
    _pen.up();

    // Get to (0,0)
    while(x != 1 || y != 1) {

        // Print current position to LCD
        _lcd->setCursor(0,0);
        _lcd->print("(");
        _lcd->print(_xy.x);
        _lcd->print(",");
        _lcd->print(_xy.y);
        _lcd->print(")       ");

        if(_p) {
            // Print current position to Serial
            Serial.print("(");
            Serial.print(_xy.x);
            Serial.print(",");
            Serial.print(_xy.y);
            Serial.println(")");
        }

        // Step once to the left (x-)
        // if not already at x=0
        if(x != 1){
            _x.backward();
            _xy.x--;
        }

        // Step once up (y-)
        // if not already at y=0
        if(y != 1){
            _y.backward();
            _xy.y--;
        }
        delay(_del);

        // Check if collided
        x = _abx.check();
        y = _aby.check();

        delay(_del);
    }
    // Serial.println('origin done');
    return { 0, 0 };
};

/**
 * Get the current POS
 * @return POS, current position
 */
POS Drive::get() {
    return _xy;
};

/**
 * Private controller that does the actual moving
 * @param  x  New X position
 * @param  y  New Y position
 * @param  up Move pen up (true, dont draw) or down (false, draw)
 * @return    The updated POS
 */
POS Drive::move(int x, int y, bool up){

    // Get the number of steps needed in x and y.
    int diff_x = _xy.x - x;
    int diff_y = _xy.y - y;

    // If no movement to be had, skip unnecessary work.
    if(diff_x == 0 && diff_y == 0) return get();

    int x_dir; // 1=forward, -1=backward
    int y_dir; // 1=forward, -1=backward
    bool trip = false; // We hit an extreme

    // This helps to ensure that we move at proper angles. If our ratio is 2
    // (y=2x) we want to step x twice and y once. Increment ratio_cur++ till
    // ratio_cur=ratio, each of these steps we increase our x. once ratio_cur
    // matches ratio we increment y and set ratio_cur to 0.
    int ratio_cur = 0;
    int ratio = 0;

    // Tells us if we need to step more in the x direction or y. y = 1/2x we
    // want to step y twice and x once. So xFirst is false. IE. y first
    bool xFirst = true;

    // Get our direction for the x-axis and absolute our difference
    if(diff_x < 0) {
        // Our diff_x < 0 so we should be calling _x.backward()
        x_dir = -1;   // Move in x-
        diff_x *= -1; // Absolute our difference

    } else if(diff_x > 0) x_dir = 1; // Move in the x+
    else x_dir = 0; // No x movement

    // Get our direction for the y-axis and absolute our difference
    if(diff_y < 0) {
        // Our diff_y < 0 so we should be calling _y.backward()
        y_dir = -1;   // Move in y-
        diff_y *= -1; // Absolute our difference

    } else if(diff_y > 0) y_dir = 1; // move in y+
    else y_dir = 0; // No y movement

    // We want more y steps than x
    if(diff_y > diff_x) {
        // Get our y steps to x steps
        ratio = (int)round(diff_y/diff_x);
        xFirst = false; // set to do our steps for y first

    // Get our x steps to y steps
    } else ratio = (int)round(diff_x/diff_y);

    // Raise or lower our pen
    if(up) _pen.up(); // moveTo()
    else _pen.down(); // lineTo()

    // Move to our desired point
    while((diff_x > 0 || diff_y > 0) && !trip){

        // Print current position to LCD
        _lcd->setCursor(0,0);
        _lcd->print("(");
        _lcd->print(_xy.x);
        _lcd->print(",");
        _lcd->print(_xy.y);
        _lcd->print(")       ");

        if(_p){
            // Print current position to Serial
            Serial.print("(");
            Serial.print(_xy.x);
            Serial.print(",");
            Serial.print(_xy.y);
            Serial.println(")");

        }

        // Move more in the x direction first (ratio count)
        if(xFirst) {

            // If we still have steps to take in x and the ratio shows we still
            // have x steps to take before y. Or our y is already finished, move
            // in the x direction.
            if((diff_x > 0 && ratio_cur < ratio) || diff_y <= 0) {
                if(x_dir > 0) { // Move forward
                    _x.forward();
                    _xy.x--; // Decrement x position
                             // Steppers are flipped (see main.cpp PinMap's)

                } else if(x_dir < 0) { // Move backward
                    _x.backward();
                    _xy.x++; // Increment x position

                }
                diff_x--; // Remove a step needed
                ratio_cur++; // Increase the ratio count
            }

            // If we still have steps to take in y and the ratio shows we have
            // finished the x steps. Or our x is already finished, move
            // in the y direction.
            if((diff_y > 0 && ratio_cur >= ratio) || diff_x <= 0) {
                if(y_dir > 0) { // Move forward
                    _y.forward();
                    _xy.y--; // Decrement y position

                } else if(y_dir < 0) { // Move backward
                    _y.backward();
                    _xy.y++; // Increment y position

                }
                diff_y--; // Remove a step needed
                ratio_cur = 0; // Reset our ratio count
            }

        // Move in the y direction first (ratio count)
        } else {

            // If we still have steps to take in y and the ratio shows we still
            // have y steps to take before x. Or our x is already finished, move
            // in the y direction.
            if((diff_y > 0 && ratio_cur < ratio) || diff_x <= 0) {
                if(y_dir > 0) { // Move forward
                    _y.forward();
                    _xy.y--; // Decrement y position

                } else if(y_dir < 0) { // Move backward
                    _y.backward();
                    _xy.y++; // Increment y position
                }
                diff_y--; // Remove a step needed
                ratio_cur++; // Increment the ratio count
            }

            // If we still have steps to take in x and the ratio shows we have
            // finished the y steps. Or our y is already finished, move
            // in the x direction.
            if((diff_x > 0 && ratio_cur >= ratio) || diff_y <= 0) {
                if(x_dir > 0) { // Move forward
                    _x.forward();
                    _xy.x--; // Decrement x position

                } else if(x_dir < 0) { // Move backward
                    _x.backward();
                    _xy.x++; // Increment x position
                }
                diff_x--; // Remove a step needed
                ratio_cur = 0; // Reset the ratio count
            }

        }

        // Check if we collided with an extreme
        if(_abx.check() != 0 || _aby.check() != 0) {
            trip = true;
        }
    }

    // If we hit an extreme reset the XY-Plotter, inform user
    if(trip) {
        // Serial.println("origin");
        _xy = origin();
        _x.setPOS(0);
        _y.setPOS(0);
    }

    // Return updated position
    return get();
};

/**
 * Set the pen low point
 * @param ro read-out
 */
int Drive::setPen(int ro) {
    return _pen.setDown(ro);
};
