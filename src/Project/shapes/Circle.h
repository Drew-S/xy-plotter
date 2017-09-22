/**
 *  Circle.h
 *
 *  Draws a circle. Subclass of Ellipse since the formula for a circle is the
 *  formula for an ellipse with a=b.
 *
 *  @author Drew Sommer
 *  @version 1.0.0
 *  @license MIT (https://mit-license.org)
 */
#ifndef CIRCLE_H
#define CIRCLE_H
#include "./Ellipse.h"
#include "../stepper/POS.h"

/**
 * Draw a circle
 */
class Circle: public Ellipse {
private:
    int _cx; // centre x
    int _cy; // centre y
    int _r; // radius
    void print();


public:
    Circle(){}; // default constructor
    Circle(int cx, int cy, int r, Drive *drive, LiquidCrystal *lcd); // primary constructor

    POS draw(bool p);
    POS draw(){ return draw(false); };
};

#endif
