/**
 *  Ellipse.cpp
 *
 *  Used to draw an ellipse. Supports rotation.
 *
 *  @author Drew Sommer
 *  @version 1.0.0
 *  @license MIT (https://mit-license.org)
 */
#include "Ellipse.h"
#include "math.h"

/**
 * Ellipse constructor
 * @param cx    int   Centre x
 * @param cy    int   Centre y
 * @param a     int   a, width
 * @param b     int   b, height
 * @param drive Drive controller
 */
Ellipse::Ellipse(int cx, int cy, int a, int b, Drive *drive):
    Shape(drive),
    _cx(cx),
    _cy(cy),
    _a(a),
    _b(b){
        _origin.x = cx;
        _origin.y = cy;
        _angle = 0;
    };

/**
 * Ellipse constructor
 * @param cx     int    Centre x
 * @param cy     int    Centre y
 * @param a      int    a, width
 * @param b      int    b, height
 * @param origin POS    origin point
 * @param angle  double Angle of rotation (radians)
 * @param drive  Drive  controller
 */
Ellipse::Ellipse(
    int cx, int cy, int a, int b,
    POS origin, double angle, Drive *drive
):
    Shape(drive),
    _cx(cx),
    _cy(cy),
    _a(a),
    _b(b),
    _origin(origin),
    _angle(angle){};

/**
 * draw the ellipse
 * @return POS drive current position
 *
 Latex:
$$
y = \frac{b}{a}\sqrt{a^2 - x^2} \\
(x + cx, y+ cy)
$$
 */
POS Ellipse::draw(bool p) {

    if(p) print();

    double a2 = pow(_a, 2);

    // moveTo start point
    if(_angle != 0) { // if rotated
        POS xy = rotate(_cx - _a, _cy);
        _drive->moveTo(xy.x, xy.y);
    } else { // not rotated
        _drive->moveTo(_cx - _a, _cy);
    }

    if(p) Serial.println("L");

    // Draw the upper 1/2 of the ellipse
    for(int x=-_a; x<=_a; x++){

        int y = (int)(((double)_b/(double)_a)*sqrt(a2 - pow(x, 2)));

        if(_angle != 0) { // if rotated
            POS xy = rotate(x, y);
            _drive->lineTo(xy.x+_cx, xy.y+_cy);
        } else { // not rotated
            _drive->lineTo(x+_cx, y+_cy);
        }
    }

    // Draw the lower 1/2 of the ellipse
    for(int x=_a; x>=-_a; x--){

        int y = (int)(((double)_b/(double)_a)*sqrt(a2 - pow(x, 2)));

        if(_angle != 0) { // if rotated
            POS xy = rotate(x, -y);
            _drive->lineTo(xy.x+_cx, xy.y+_cy);
        } else { // not rotated
            _drive->lineTo(x+_cx, -y+_cy);
        }
    }

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

    POS shift = {_origin.x - _cx, _origin.y - _cy};
    POS newShift = {0, 0};

    if(!_rotatedValues) {
        _rotatedValues = true;
        _cosA = cos(_angle);
        _sinA = sin(_angle);
    }

    newShift.x = (int)(_cosA*shift.x - _sinA*shift.y);
    newShift.y = (int)(_sinA*shift.x + _cosA*shift.y);

    int xx = (int)(_cosA*x - _sinA*y);
    int yy = (int)(_sinA*x + _cosA*y);

    return {xx - newShift.x, yy - newShift.y};
};

/**
 * Print ellipse info to the client
 */
void Ellipse::print(){
    Serial.print("Ellipse(cx=");
    Serial.print(_cx);
    Serial.print(",cy=");
    Serial.print(_cy);
    Serial.print(",a=");
    Serial.print(_a);
    Serial.print(",b=");
    Serial.print(_b);
    Serial.print(",origin={");
    Serial.print(_origin.x);
    Serial.print(",");
    Serial.print(_origin.y);
    Serial.print("},angle=");
    Serial.print(_angle);
    Serial.print(") M ");
};
