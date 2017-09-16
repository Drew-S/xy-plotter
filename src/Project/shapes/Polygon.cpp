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
Polygon::Polygon(LinkedList<POS> *points, Drive *drive):
    Shape(drive),
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
    while(_points->size() > 0) {
        POS point = _points->shift();
        Serial.print("Polygon::point(");
        Serial.print(point.x);
        Serial.print(",");
        Serial.print(point.y);
        Serial.println(")");
        _drive->moveTo(point.x, point.y);
    }

    return _drive->get();
};

void Polygon::print() {

};
