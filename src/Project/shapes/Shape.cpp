/**
 *  Shape.cpp
 *
 *  Base shape class. Parent to all other shapes. Used to create a list of shapes
 *  to call children Shape::draw().
 *
 *  @author Drew Sommer
 *  @version 1.0.1
 *  @license MIT (https://mit-license.org)
 */
#include "Shape.h"

/**
 * Shape with passed parameters
 * @param drive Drive controller
 * @param lcd   LCD screen controller
 */
Shape::Shape(Drive *drive, LiquidCrystal *lcd): _drive(drive), _lcd(lcd){};

/**
 * Draw our shape
 * @param  p Boolean, whether or not to print our details
 * @return   Updated position
 */
POS Shape::draw(bool p=false){

    if(p) print();

    return _drive->get();
};
