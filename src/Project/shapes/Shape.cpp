/**
 *  Shape.cpp
 *
 *  Base shape class. Parent to all other shapes. Used to create a list of shapes
 *  to call children Shape::draw().
 *
 *  @author Drew Sommer
 *  @version 1.0.0
 *  @license MIT (https://mit-license.org)
 */
#include "Shape.h"

/**
 * Main constructor
 * @param drive Drive
 *
 * Sub-shapes call the super Shape() passing the drive pointer
 */
Shape::Shape(Drive *drive): _drive(drive){};

/**
 * Destructor
 */
Shape::~Shape() {
    delete _drive;
};

/**
 * Draw function.
 * @return POS
 *
 * Shape only returns drives current POS
 *
 * Sub-shape draws its specific shape
 */
POS Shape::draw(bool p=false){

    if(p) print();

    return _drive->get();
};
