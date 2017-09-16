/**
 *  Polygon.h
 *
 *  Draws lines between points of any length of lines.
 *
 *  @author Drew Sommer
 *  @version 1.0.0
 *  @license MIT (https://mit-license.org)
 */
#ifndef POLYGON_H
#define POLYGON_H
#include <LinkedList.h>
#include "./Shape.h"
#include "../stepper/POS.h"

/**
 * Polygon draws a shape with straight lines between 2 points. With N points.
 */
class Polygon: public Shape {
private:
    LinkedList<POS> *_points; // Points to draw between

    void print();

public:
    Polygon(){}; // default constructor
    Polygon(LinkedList<POS> *points, Drive *drive); // primary constructor

    POS draw(bool p); // draw function
    POS draw(){ return draw(false); };
};

#endif
