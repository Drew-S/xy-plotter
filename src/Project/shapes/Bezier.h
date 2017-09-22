/**
 *  Bezier.h
 *
 *  Draws a bezier curve between two points p0 and p3, with control points
 *  p1 and p2.
 *
 *  @author Drew Sommer
 *  @version 1.0.0
 *  @license MIT (https://mit-license.org)
 */
#ifndef BEZIER_H
#define BEZIER_H
#include "./Shape.h"
#include "../stepper/POS.h"

class Bezier: public Shape {
private:
    POS _p0; // start point
    POS _p1; // first control point
    POS _p2; // second control point
    POS _p3; // end point

    // Print info to the client
    void print();

public:
    Bezier(){}; // default constructor
    Bezier(POS p0, POS p1, POS p2, POS p3, Drive *drive, LiquidCrystal *lcd); // primary constructor

    // draw function
    POS draw(bool p);
    POS draw(){ return draw(false); };
};

#endif
