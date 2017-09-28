/**
 *  Bezier.h
 *
 *  Draws a bezier curve between two points p0 and p3, with control points
 *  p1 and p2.
 *
 *  @author Drew Sommer
 *  @version 1.0.1
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

public:

    /**
     * Bezier()
     */
    Bezier(){};

    /**
     * Bezier curve
     * @param p0    Start point
     * @param p1    First control point
     * @param p2    Second control point
     * @param p3    End point
     * @param drive Drive controller
     * @param lcd   LCD screen controller
     */
    Bezier(POS p0, POS p1, POS p2, POS p3, Drive *drive, LiquidCrystal *lcd); // primary constructor

    /**
     * Draw the Bezier curve
     * @param  p Whether or not to print details
     * @return   Updated position
     Latex:
    $$
    x_t = x_0 + 3t(x_1 - x_0) + 3t^2(x_0 + x_2 - 2x_1) + t^3(x_3 - x_0 + 3x_1 - 3x_2) \\
    y_t = y_0 + 3t(y_1 - y_0) + 3t^2(y_0 + y_2 - 2y_1) + t^3(y_3 - y_0 + 3y_1 - 3y_2) \\
    \{0 \le t \le 1\}
    $$
     */
    POS draw(bool p);

    /**
     * Draw the Bezier curve without printing details
     * @return Updated position
     */
    POS draw(){ return draw(false); };

    /**
     * Print details to LCD and Serials
     */
    void print();
};

#endif
