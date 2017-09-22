/**
 *  Ellipse.h
 *
 *  Used to draw an ellipse. Supports rotation.
 *
 *  @author Drew Sommer
 *  @version 1.0.0
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
    int _cx; // Centre x
    int _cy; // Centre y
    int _a; // a width
    int _b; // b height
    double _cosA = 0; // Rotation value cos
    double _sinA = 0; // Rotation value sin
    bool _rotatedValues = false; // If already rotated

    POS _origin; // Origin to rotate by
    double _angle = 0; // Angle to rotate (radians)

    void print(); // Print info to client

public:
    Ellipse(){}; // Default constructor
    Ellipse(int cx, int cy, int a, int b, Drive *drive, LiquidCrystal *lcd); // REgular ellipse constructor

    // Rotated ellipse constructor
    Ellipse(int cx, int cy, int a, int b, POS origin, double angle, Drive *drive, LiquidCrystal *lcd);

    POS draw(bool p);
    POS draw(){ return draw(false); };

    // rotate function point by point
    POS rotate(int x, int y);
};

#endif
