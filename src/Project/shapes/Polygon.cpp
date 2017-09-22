/**
 *  Polygon.cpp
 *
 *  Draws lines between points of any length of lines.
 *
 *  @author Drew Sommer
 *  @version 1.0.0
 *  @license MIT (https://mit-license.org)
 */
#include "Polygon.h"

/**
 * Create a new polygon drawing between N number of points
 * @param points List<POS>
 * @param drive  Drive
 */
Polygon::Polygon(LinkedList<POS> *points, Drive *drive, LiquidCrystal * lcd):
    Shape(drive, lcd),
    _points(points) {};

/**
 * Draws the shape
 * @return POS
 *
 * The first point is a moveTo point, does not draw to point.
 *
 * To close the polygon the last point should repeat the first point.
 *
 * Draws a line bettwen P_n and P_{n+1}
 */
POS Polygon::draw(bool p=false) {

    if(p) print();

    _drive->moveTo(_points->get(0).x, _points->get(0).y);

    // Loop through the points, removing the front most point and drawing to it
    for(int i=1; i<_points->size(); i++) {
        POS point = _points->get(i);
        _drive->lineTo(point.x, point.y);
    }
    for(int i=0; i<_points->size(); i++){
        _points->remove(0);
    }

    return _drive->get();
};

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
