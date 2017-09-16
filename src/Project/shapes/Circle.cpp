/**
*  Circle.cpp
*
*  Draws a circle. Subclass of Ellipse since the formula for a circle is the
*  formula for an ellipse with a=b.
*
*  @author Drew Sommer
*  @version 1.0.0
*  @license MIT (https://mit-license.org)
*/
#include "Circle.h"
#include "math.h"

/**
 * Create a new circle
 * @param cx    int   centre x
 * @param cy    int   centre y
 * @param r     int   radius
 * @param drive Drive drive controller
 */
Circle::Circle(int cx, int cy, int r, Drive *drive):
    Ellipse(cx, cy, r, r, drive),
    _cx(cx),
    _cy(cy),
    _r(r){}

/**
 * Draw the circle
 * @param  p bool print info
 * @return   POS  current position
 *
 Latex:
$$
y = \sqrt{r^2-x^2}
$$
 */
POS Circle::draw(bool p=false) {
    if(p) print();
    return Ellipse::draw(false);
};

/**
 * Print info to the client
 */
void Circle::print() {
    Serial.print("Circle(cx=");
    Serial.print(_cx);
    Serial.print(",cy=");
    Serial.print(_cy);
    Serial.print(",r=");
    Serial.print(_r);
    Serial.print(") M ");
};
