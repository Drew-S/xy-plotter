/**
 *  Polygon.h
 *
 *  Draws lines between points of any length of lines.
 *
 *  @author Drew Sommer
 *  @version 1.0.1
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
    LinkedList<POS> *_points; // Points to draw between, first point is moveTo

public:
    /**
     * Polygon()
     */
    Polygon(){};

    /**
     * Polygon with a list of points to draw
     * @param points List<POS> points to draw
     * @param drive  Drive controller
     * @param lcd    LCD screen controller
     */
    Polygon(LinkedList<POS> *points, Drive *drive, LiquidCrystal *lcd);

    /**
     * Draw from point to point
     * @param  p Print details
     * @return   Updated position
     */
    POS draw(bool p);

    /**
     * Draw without printing details
     * @return Updated position
     */
    POS draw(){ return draw(false); };

    /**
     * Print details to lcd and Serial
     */
    void print();

};

#endif
