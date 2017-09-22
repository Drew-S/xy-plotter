/**
 *  Shape.h
 *
 *  Base shape class. Parent to all other shapes. Used to create a list of shapes
 *  to call children Shape::draw().
 *
 *  @author Drew Sommer
 *  @version 1.0.1
 *  @license MIT (https://mit-license.org)
 */
#ifndef SHAPE_H
#define SHAPE_H
#include "../stepper/POS.h"
#include "../Drive.h"
#include "../lib/ShiftedLCD.h"

class Drive;

/**
 * Shape parent class used for looping through many subclass shapes
 */
class Shape {
private:

    // Print function for informing the client of what is being drawn
    void print(){
        _lcd->setCursor(0,1);
        _lcd->print("Shape()");
        Serial.println("Shape()");
    };

public:

    Drive *_drive; // Drive controller, accessible by subclasses
    LiquidCrystal *_lcd;

    /**
     * Shape()
     */
    Shape(){};

    /**
     * Shape with passed parameters
     * @param drive Drive controller
     * @param lcd   LCD screen controller
     */
    Shape(Drive *drive, LiquidCrystal *lcd);

    /**
     * Destructor
     */
    ~Shape(){};

    /**
     * Draw our shape
     * @param  p Boolean, whether or not to print our details
     * @return   Updated position
     */
    virtual POS draw(bool p);

    /**
     * Draw our shape (without printing details)
     * @return Updated position
     */
    virtual POS draw(){ return draw(false); };

    // Virtual allows for polymorphism and creating an array of shapes (Ellipses,
    // Circles, Etc.) and call their respective draw functions.
};

#endif
