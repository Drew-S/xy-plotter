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
    int _r;  // radius

    /**
     * Print details to LCD and Serial
     */
    void print();


public:

    /**
     * Circle()
     */
    Circle(){};

    /**
     * Create a Circle (Ellipse where a=b)
     * @param cx    Centre x
     * @param cy    Centre y
     * @param r     Radius
     * @param drive Drive controller
     * @param lcd   LCD screen controller
     */
    Circle(int cx, int cy, int r, Drive *drive, LiquidCrystal *lcd); // primary constructor

    /**
     * Draw the Circle
     * @param  p Whether or not to print details
     * @return   Updated position
     Latex:
    $$
    y = \pm\sqrt{r^2-x^2}
    $$
     */
    POS draw(bool p);

    /**
     * Draw Circle without printing details
     * @return Updated position
     */
    POS draw(){ return draw(false); };
};

#endif
