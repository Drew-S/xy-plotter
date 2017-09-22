/**
 *  Bezier.cpp
 *
 *  Draws a bezier curve between two points p0 and p3, with control points
 *  p1 and p2.
 *
 *  @author Drew Sommer
 *  @version 1.0.0
 *  @license MIT (https://mit-license.org)
 */
// TODO: Test Bezier curve
#include "Bezier.h"
#include "math.h"

/**
 * Bezier curve
 * @param p0    Start point
 * @param p1    First control point
 * @param p2    Second control point
 * @param p3    End point
 * @param drive Drive controller
 * @param lcd   LCD screen controller
 */
Bezier::Bezier(POS p0, POS p1, POS p2, POS p3, Drive *drive, LiquidCrystal *lcd):
    Shape(drive, lcd),
    _p0(p0),
    _p1(p1),
    _p2(p2),
    _p3(p3){};

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
POS Bezier::draw(bool p) {

    if(p) print();

    // Move to start point
    _drive->moveTo(_p0.x, _p0.y);

    // TODO: Update resolution to get a proper total number of x steps

    // Get our smallest x
    int min = _p0.x;
    if(_p1.x < min) min = _p1.x;
    if(_p2.x < min) min = _p2.x;
    if(_p3.x < min) min = _p3.x;

    // Get our largest x
    int max = _p0.x;
    if(_p1.x > max) max = _p1.x;
    if(_p2.x > max) max = _p2.x;
    if(_p3.x > max) max = _p3.x;

    // (max - min) = t steps (resolution)
    for(int i=0; i<=(max - min); i++){
        double t = i/(max - min); // 0 <= t <= 1
        double t2 = pow(t, 2);    // t^2
        double t3 = pow(t, 3);    // t^3

        // Get our x value
        int x = (int)(
            _p0.x +
            3 * t * (_p1.x - _p0.x) +
            3 * t2 * (_p0.x + _p2.x - 2 * _p1.x) +
            t3 * (_p3.x - _p0.x + 3 * _p1.x - 3 * _p2.x)
        );

        // Get our y value
        int y = (int)(
            _p0.y +
            3 * t * (_p1.y - _p0.y) +
            3 * t2 * (_p0.y + _p2.y - 2 * _p1.y) +
            t3 * (_p3.y - _p0.y + 3 * _p1.y - 3 * _p2.y)
        );

        // Draw to our next value
        _drive->lineTo(x, y);
    }

    // Return updated position
    return _drive->get();
};

/**
 * Print details to LCD and Serials
 */
void Bezier::print() {
    // TODO: write up print function for Bezier curve
};
