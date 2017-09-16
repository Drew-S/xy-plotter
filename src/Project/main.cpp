/**
 *  main.cpp
 *
 *  Main controller for drawing to XY-Plotter with Arduino
 *
 *  @author Drew Sommer
 *  @version 1.0.0
 *  @license MIT (https://mit-license.org)
 */

#include <Arduino.h>
#include "Drive.h"

#include <LinkedList.h>

#include "stepper/POS.h"
#include "shapes/Circle.h"
#include "shapes/Ellipse.h"
#include "shapes/Polygon.h"
#include "shapes/Bezier.h"
#include "shapes/Shape.h"

#include "math.h"

class Drive;

//         stp dir en x  x-   x+
PinMap X = { 4, 2, 3, 0, 340, 510, 50, 1 };

//         stp dir en y  y-   y+
PinMap Y = { 7, 5, 6, 1, 340, 510, 50, 1 };
Drive *drive = new Drive(X, Y, 5, 11, 0, 180);
bool start = true;

LinkedList<POS> *polyPoints1 = new LinkedList<POS>();
LinkedList<POS> *polyPoints2 = new LinkedList<POS>();

LinkedList<Shape> shapes = LinkedList<Shape>();

Ellipse E1;
Ellipse E2;
Ellipse E3;
Circle C;
Polygon P1;
Polygon P2;

POS shift = {1050, 1700};

void setup() {
    Serial.begin(9600);
    drive->attach();

    polyPoints1->add({400, 400});
    polyPoints1->add({400, 1700});
    polyPoints1->add({1700, 1700});
    polyPoints1->add({1700, 400});
    polyPoints1->add({400, 400});

    polyPoints2->add({1050, 3454});
    polyPoints2->add({1969, 2534});
    polyPoints2->add({1050, 1615});
    polyPoints2->add({130, 2534});
    polyPoints2->add({1050, 3454});

    // shapes.add(Ellipse(shift.x, shift.y, 650, 325, drive));
    // shapes.add(Ellipse(shift.x, shift.y, 650, 325, {shift.x, shift.y}, PI/3, drive));
    // shapes.add(Ellipse(shift.x, shift.y, 650, 325, {shift.x, shift.y}, -PI/3, drive));
    // shapes.add(Circle(shift.x, shift.y, 650, drive));
    // shapes.add(Polygon(polyPoints, drive));
    E1 = Ellipse(shift.x, shift.y, 650, 325, drive);
    E2 = Ellipse(shift.x, shift.y, 650, 325, {shift.x, shift.y}, PI/3, drive);
    E3 = Ellipse(shift.x, shift.y, 650, 325, {shift.x, shift.y}, -PI/3, drive);
    C = Circle(shift.x, shift.y, 650, drive);
    P1 = Polygon(polyPoints1, drive);
    P2 = Polygon(polyPoints2, drive);
}

void loop() {

    if(start) {
        delay(1000);
        Serial.println(shapes.size());
        // while(shapes.size() > 0) {
        //     Serial.println("Drawing");
        //     shapes.shift().draw();
        // }
        E1.draw(true);
        E2.draw(true);
        E3.draw(true);
        C.draw(true);
        P1.draw(true);
        P2.draw(true);
        start = false;
    }
}
