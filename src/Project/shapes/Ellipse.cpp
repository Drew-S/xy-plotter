/**
 *  Ellipse.cpp
 *
 *  Used to draw an ellipse. Supports rotation.
 *
 *  @author Drew Sommer
 *  @version 1.0.1
 *  @license MIT (https://mit-license.org)
 */
#include "Ellipse.h"
#include "math.h"

/**
 * Create an Ellipse without a rotation
 * @param cx    Centre x position
 * @param cy    Centre y position
 * @param a     width
 * @param b     height
 * @param drive Drive controller
 * @param lcd   LCD screen controller
 */
Ellipse::Ellipse(int cx, int cy, int a, int b, Drive *drive, LiquidCrystal *lcd):
    Shape(drive, lcd),
    _cx(cx),
    _cy(cy),
    _a(a),
    _b(b){
        _origin.x = cx;
        _origin.y = cy;
        _angle = 0;
    };

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
Ellipse::Ellipse(
    int cx, int cy, int a, int b,
    POS origin, double angle, Drive *drive,
    LiquidCrystal *lcd
):
    Shape(drive, lcd),
    _cx(cx),
    _cy(cy),
    _a(a),
    _b(b),
    _origin(origin),
    _angle(angle){};

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
POS Ellipse::draw(bool p) {

    if(p) print();

    // moveTo start point
    if(_angle != 0) { // If rotation neede
        POS xy = rotate(_cx - _a, _cy);
        _drive->moveTo(xy.x, xy.y);

    } else { // Do not rotate
        _drive->moveTo(_cx - _a, _cy);
    }

    // Draw the upper 1/2 of the ellipse
    for(int x=-_a; x<=_a; x++){

        // Get our y
        /*
        $$
        y = \frac{b}{a}\sqrt{a^2 - x^2} \\
        (x + cx, y+ cy)
        $$
         */
        int y = (int)(((double)_b/(double)_a)*sqrt(pow(_a, 2) - pow(x, 2)));

        if(_angle != 0) { // If rotation wanted
            POS xy = rotate(x, y);
            _drive->lineTo(xy.x+_cx, xy.y+_cy);

        } else { // Do not rotate
            _drive->lineTo(x+_cx, y+_cy);
        }
    }

    // Draw the lower 1/2 of the ellipse
    for(int x=_a; x>=-_a; x--){

        // Get our y
        /*
        $$
        y = -\frac{b}{a}\sqrt{a^2 - x^2} \\
        (x + cx, y+ cy)
        $$
         */
        int y = (int)(((double)_b/(double)_a)*sqrt(pow(_a, 2) - pow(x, 2)));

        if(_angle != 0) { // If rotation wanted
            POS xy = rotate(x, -y);
            _drive->lineTo(xy.x+_cx, xy.y+_cy);

        } else { // Do not rotate
            _drive->lineTo(x+_cx, -y+_cy);
        }
    }

    // Updated position
    return _drive->get();
};

/**
 * Rotate the ellipse by the origin point, point by point
 * @param  x int x point
 * @param  y int y point
 * @return   new position
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
POS Ellipse::rotate(int x, int y){

    // Our shift point
    POS shift = {_origin.x - _cx, _origin.y - _cy};

    // The new position
    POS newShift = {0, 0};

    // If we have not already calculated our cos and sin, do so and save
    if(!_rotatedValues) {
        _rotatedValues = true;
        _cosA = cos(_angle);
        _sinA = sin(_angle);
    }

    // Since rotation is done by the centre of the Ellipse we rotate the
    // shift point to know where to move the Ellipse before re-centering
    // the Ellipse. Only really used when the origin is not the centre of
    // the Ellipse.
    newShift.x = (int)(_cosA*shift.x - _sinA*shift.y);
    newShift.y = (int)(_sinA*shift.x + _cosA*shift.y);

    // Rotate our point
    int xx = (int)(_cosA*x - _sinA*y);
    int yy = (int)(_sinA*x + _cosA*y);

    // Return new point shifted
    return {xx - newShift.x, yy - newShift.y};
};

/**
 * Print details to LCD and Serail
 */
void Ellipse::print(){

    Serial.print("E(");
    Serial.print(_cx);
    Serial.print(",");
    Serial.print(_cy);
    Serial.print(",");
    Serial.print(_a);
    Serial.print(",");
    Serial.print(_b);
    Serial.print(",{");
    Serial.print(_origin.x);
    Serial.print(",");
    Serial.print(_origin.y);
    Serial.print("},");
    Serial.print(_angle);
    Serial.println(")");

    _lcd->setCursor(0, 1);
    _lcd->print("E(");
    _lcd->print(_cx);
    _lcd->print(",");
    _lcd->print(_cy);
    _lcd->print(",");
    _lcd->print(_a);
    _lcd->print(",");
    _lcd->print(_b);
    _lcd->print(",{");
    _lcd->print(_origin.x);
    _lcd->print(",");
    _lcd->print(_origin.y);
    _lcd->print("},");
    _lcd->print(_angle);
    _lcd->print(")");
};
