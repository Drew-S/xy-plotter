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
#include "lib/ShiftedLCD.h"

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

//         stp dir en x  x-   x+  buff flip(bool)
PinMap X = { 4, 2, 3, 0, 340, 510, 50, 1 };

//         stp dir en y  y-   y+  buff flip(bool)
PinMap Y = { 7, 5, 6, 1, 340, 510, 50, 1 };

// LCD controller
LiquidCrystal lcd(9);
LiquidCrystal *lcd_pointer = &lcd;

//                           del servo up down (up / down angle, max, 71)
Drive *drive = new Drive(X, Y, 5, 10, 0, 71, lcd_pointer);

const int startButton = 2;
const int dial = 3;

LinkedList<POS> *polyPoints1 = new LinkedList<POS>();
LinkedList<POS> *polyPoints2 = new LinkedList<POS>();

Shape *shapes[10];

POS shift = {1050, 1700};

bool all = true;
bool start = true;

void setup() {

    lcd_pointer->begin(16, 2);
    lcd_pointer->noCursor();

    lcd_pointer->clear();
    lcd_pointer->print("Starting XY");

    Serial.begin(9600);
    drive->attach();

    // polyPoints1->add({400, 400});
    // polyPoints1->add({400, 1700});
    // polyPoints1->add({1700, 1700});
    // polyPoints1->add({1700, 400});
    // polyPoints1->add({400, 400});
    //
    // polyPoints2->add({1050, 3454});
    // polyPoints2->add({1969, 2534});
    // polyPoints2->add({1050, 1615});
    // polyPoints2->add({130, 2534});
    // polyPoints2->add({1050, 3454});

    shapes[0] = new Bezier({400, 1200}, {800, 2400}, {1200, 400}, {2400, 800}, drive, lcd_pointer);
    // shapes[0] = new Ellipse(shift.x, shift.y, 650, 325, drive, lcd_pointer);
    // shapes[1] = new Ellipse(shift.x, shift.y, 650, 325, {shift.x, shift.y}, PI/3, drive, lcd_pointer);
    // shapes[2] = new Ellipse(shift.x, shift.y, 650, 325, {shift.x, shift.y}, -PI/3, drive, lcd_pointer);
    // shapes[3] = new Circle(shift.x, shift.y, 650, drive, lcd_pointer);
    // shapes[4] = new Polygon(polyPoints1, drive, lcd_pointer);
    // shapes[5] = new Polygon(polyPoints2, drive, lcd_pointer);

    Serial.println("Starting XY");
}

int temp = 0;

void loop() {
    while(all){
        temp = drive->setPen(map(analogRead(dial), 0, 1023, 0, 71));
        lcd_pointer->clear();
        lcd_pointer->print("Pen: ");
        lcd_pointer->print(map(temp, 0, 71, 100, 0));
        Serial.print("Pen: ");
        Serial.println(map(temp, 0, 71, 100, 0));
        delay(20);
        if(analogRead(startButton) > 1000) {
            all = false;
            Serial.println("Start drawing");
            lcd_pointer->clear();
            lcd_pointer->print("Start drawing");
            delay(500);
        }
    }
    while(start){
        for(int i=0; i<10; i++){
            shapes[i]->draw(true);
        }
        start = false;
    }
}
