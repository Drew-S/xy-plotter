/**
 *  Shape.h
 *
 *  Base shape class. Parent to all other shapes. Used to create a list of shapes
 *  to call children Shape::draw().
 *
 *  @author Drew Sommer
 *  @version 1.0.0
 *  @license MIT (https://mit-license.org)
 */
#ifndef SHAPE_H
#define SHAPE_H
#include "../stepper/POS.h"
#include "../Drive.h"

class Drive;

/**
 * Shape parent class used for looping through many subclass shapes
 */
class Shape {
private:

    // Print function for informing the client of what is being drawn
    void print(){
        Serial.println("Shape()");
    };

public:

    Drive *_drive; // Drive controller, accessible by subclasses

    Shape(){}; // Default constructor
    Shape(Drive *drive); // Normal constructor
    ~Shape(); // Destructor

    POS draw(bool p); // Default draw function
    POS draw(){ return draw(false); };
};

#endif