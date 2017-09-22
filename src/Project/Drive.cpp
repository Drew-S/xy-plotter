/**
 *  Drive.cpp
 *
 *  Drive controller, manages each stepper motor and servo.
 *  Maintains control over X and Y positions, movement along the x and y-axis.
 *  Maintains control over the pens up and down position
 *
 *  @author Drew Sommer
 *  @version 1.0.0
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
 */
Drive::Drive(PinMap x, PinMap y, int del, int servo, int up, int down, LiquidCrystal *lcd)
    : _x(x, del),
      _y(y, del),
      _abx(x.btnPin, x.btn1, x.btn2, x.buff),
      _aby(y.btnPin, y.btn1, y.btn2, y.buff),
      _del(del),
      _pen(servo, up, down, del),
      _lcd(lcd) {};

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
POS Drive::moveTo(int x, int y) {
    return move(x, y, true);
};

/**
 * Return the pen to origin point (0,0)
 * @return Updated POS (0,0)
 */
POS Drive::origin() {
    int x = _abx.check();
    int y = _aby.check();
    _pen.up();
    while(x != 1 || y != 1) {

        _lcd->setCursor(0,0);
        _lcd->print("(");
        _lcd->print(_xy.x);
        _lcd->print(",");
        _lcd->print(_xy.y);
        _lcd->print(")       ");

        Serial.print("(");
        Serial.print(_xy.x);
        Serial.print(",");
        Serial.print(_xy.y);
        Serial.println(")");

        if(x != 1){
            _x.backward();
        }
        if(y != 1){
            _y.backward();
        }
        delay(_del);
        x = _abx.check();
        y = _aby.check();

        delay(_del);
    }
    // Serial.println('origin done');
    return { 0, 0 };
};

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

    int diff_x = _xy.x - x;
    int diff_y = _xy.y - y;

    int x_dir;
    int y_dir;
    bool trip = false;

    int ratio_cur = 0;

    bool xFirst = true;
    int ratio = 0;

    if(diff_x < 0) {
        x_dir = -1;
        diff_x *= -1;
    } else if(diff_x > 0) x_dir = 1;
    else x_dir = 0;

    if(diff_y < 0) {
        y_dir = -1;
        diff_y *= -1;
    } else if(diff_y > 0) y_dir = 1;
    else y_dir = 0;

    if(diff_y > diff_x) {
        ratio = (int)round(diff_y/diff_x);
        xFirst = false;
    } else ratio = (int)round(diff_x/diff_y);

    if(up) _pen.up();
    else _pen.down();

    while((diff_x > 0 || diff_y > 0) && !trip){

        _lcd->setCursor(0,0);
        _lcd->print("(");
        _lcd->print(_xy.x);
        _lcd->print(",");
        _lcd->print(_xy.y);
        _lcd->print(")       ");

        Serial.print("(");
        Serial.print(_xy.x);
        Serial.print(",");
        Serial.print(_xy.y);
        Serial.println(")");

        if(xFirst == 1) {
            if((diff_x > 0 && ratio_cur < ratio) || diff_y <= 0) {
                if(x_dir > 0) {
                    _x.forward();
                    _xy.x--;
                } else if(x_dir < 0) {
                    _x.backward();
                    _xy.x++;
                }
                diff_x--;
                ratio_cur++;
            }
            if((diff_y > 0 && ratio_cur >= ratio) || diff_x <= 0) {
                if(y_dir > 0) {
                    _y.forward();
                    _xy.y--;
                } else if(y_dir < 0) {
                    _y.backward();
                    _xy.y++;
                }
                diff_y--;
                ratio_cur = 0;
            }

        } else {
            if((diff_y > 0 && ratio_cur < ratio) || diff_x <= 0) {
                if(y_dir > 0) {
                    _y.forward();
                    _xy.y--;
                } else if(y_dir < 0) {
                    _y.backward();
                    _xy.y++;
                }
                diff_y--;
                ratio_cur++;
            }
            if((diff_x > 0 && ratio_cur >= ratio) || diff_y <= 0) {
                if(x_dir > 0) {
                    _x.forward();
                    _xy.x--;
                } else if(x_dir < 0) {
                    _x.backward();
                    _xy.x++;
                }
                diff_x--;
                ratio_cur = 0;
            }

        }
        if(_abx.check() != 0 || _aby.check() != 0) {
            trip = true;
        }
    }
    if(trip) {
        // Serial.println("origin");
        _xy = origin();
        _x.setPOS(0);
        _y.setPOS(0);
    }
    return get();
};

int Drive::setPen(int ro) {
    return _pen.setDown(ro);
};
