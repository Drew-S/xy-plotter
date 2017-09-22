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
 * Create a Circle (Ellipse where a=b)
 * @param cx    Centre x
 * @param cy    Centre y
 * @param r     Radius
 * @param drive Drive controller
 * @param lcd   LCD screen controller
 */
Circle::Circle(int cx, int cy, int r, Drive *drive, LiquidCrystal *lcd):
    Ellipse(cx, cy, r, r, drive, lcd),
    _cx(cx),
    _cy(cy),
    _r(r){}

/**
 * Draw the Circle
 * @param  p Whether or not to print details
 * @return   Updated position
 Latex:
$$
y = \pm\sqrt{r^2-x^2}
$$
 */
POS Circle::draw(bool p=false) {
    if(p) print();
    return Ellipse::draw(false);
};

/**
 * Print details to LCD and Serial
 */
void Circle::print() {
    Serial.print("C(");
    Serial.print(_cx);
    Serial.print(",");
    Serial.print(_cy);
    Serial.print(",");
    Serial.print(_r);
    Serial.println(")");

    _lcd->setCursor(0,1);
    _lcd->print("C(");
    _lcd->print(_cx);
    _lcd->print(",");
    _lcd->print(_cy);
    _lcd->print(",");
    _lcd->print(_r);
    _lcd->print(")");

};
