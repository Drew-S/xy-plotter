/**
 *  Polygon.cpp
 *
 *  Draws lines between points of any length of lines.
 *
 *  @author Drew Sommer
 *  @version 1.0.1
 *  @license MIT (https://mit-license.org)
 */
#include "Polygon.h"

/**
 * Polygon with a list of points to draw
 * @param points List<POS> points to draw
 * @param drive  Drive controller
 * @param lcd    LCD screen controller
 */
Polygon::Polygon(LinkedList<POS> *points, Drive *drive, LiquidCrystal * lcd):
    Shape(drive, lcd),
    _points(points) {};

/**
 * Draw from point to point
 * @param  p Print details
 * @return   Updated position
 */
POS Polygon::draw(bool p=false) {

    if(p) print();

    // Move to our first point (point[0] is moveTo not line to).
    // To close a Polygon repeat the first point at the end.
    _drive->moveTo(_points->get(0).x, _points->get(0).y);

    // Loop through the points
    for(int i=1; i<_points->size(); i++) {
        POS point = _points->get(i);      // Get a point
        _drive->lineTo(point.x, point.y); // Draw to the point

    }

    // Remove points, cleaning up some memory
    for(int i=0; i<_points->size(); i++){
        _points->remove(0);
    }

    // Updated position
    return _drive->get();
};

/**
 * Print details to lcd and Serial
 */
void Polygon::print() {
    Serial.print("P(");
    _lcd->setCursor(0, 1);
    _lcd->print("P(");

    for(int i=0; i<_points->size(); i++){
        if(i == _points->size()-1){

            Serial.print("{");
            Serial.print(_points->get(i).x);
            Serial.print(",");
            Serial.print(_points->get(i).y);
            Serial.print("}");

            _lcd->print("{");
            _lcd->print(_points->get(i).x);
            _lcd->print(",");
            _lcd->print(_points->get(i).y);
            _lcd->print("}");

        } else {
            Serial.print("{");
            Serial.print(_points->get(i).x);
            Serial.print(",");
            Serial.print(_points->get(i).y);
            Serial.print("},");

            _lcd->print("{");
            _lcd->print(_points->get(i).x);
            _lcd->print(",");
            _lcd->print(_points->get(i).y);
            _lcd->print("},");
        }
    }
    Serial.println(")");
    _lcd->print(")");
};
