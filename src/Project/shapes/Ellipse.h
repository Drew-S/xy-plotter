/**
 *  Ellipse.h
 *
 *  Used to draw an ellipse. Supports rotation.
 *
 *  @author Drew Sommer
 *  @version 1.0.3
 *  @license MIT (https://mit-license.org)
 */
#ifndef ELLIPSE_H
#define ELLIPSE_H
#include "Shape.h"
#include "../stepper/POS.h"

/**
 * Draws an ellipse
 */
class Ellipse: public Shape {
private:
    int _cx;                     // Centre x
    int _cy;                     // Centre y
    int _a;                      // a width
    int _b;                      // b height
    double _cosA = 0;            // Rotation value cos
    double _sinA = 0;            // Rotation value sin
    bool _rotatedValues = false; // If already rotated

    POS _origin;       // Origin to rotate by
    double _angle = 0; // Angle to rotate (radians)

    /**
     * Rotate the ellipse by the origin point, point by point
     * @param  x
     * @param  y
     * @return   Updated position
     *
     Latex:
    $$
    \left[
    \begin{array}{cc}
    \cos\theta & -\sin\theta \\
    \sin\theta & \cos\theta
    \end{array}
    \right]
    \left[
    \begin{array}{c}
    x \\ y
    \end{array}
    \right]
    $$
     */
    POS rotate(int x, int y);

public:

    /**
     * Ellipse()
     */
    Ellipse(){};

    /**
     * Create an Ellipse without a rotation
     * @param cx    Centre x position
     * @param cy    Centre y position
     * @param a     width
     * @param b     height
     * @param drive Drive controller
     * @param lcd   LCD screen controller
     */
    Ellipse(int cx, int cy, int a, int b, Drive *drive, LiquidCrystal *lcd);

    /**
     * Create an Ellipse with a rotation
     * @param cx     Centre x position
     * @param cy     Centre y position
     * @param a      width
     * @param b      height
     * @param origin (x,y) to rotate by
     * @param angle  angle of rotation (radians)
     * @param drive  Drive controller
     * @param lcd    LCD screen controller
     */
    Ellipse(int cx, int cy, int a, int b, POS origin, double angle, Drive *drive, LiquidCrystal *lcd);

    /**
     * Draw the Ellipse
     * @param  p Whether or not to print details
     * @return   Updated position
     *
     * Latex:
     $$
     y = \pm\frac{b}{a}\sqrt{a^2 - x^2} \\
     (x + cx, y+ cy)
     $$
     */
    POS draw(bool p);

    /**
     * Draw the Ellipse without details
     * @return Updated position
     */
    POS draw(){ return draw(false); };

    /**
     * Print details to LCD and Serail
     */
    void print();
};

#endif
