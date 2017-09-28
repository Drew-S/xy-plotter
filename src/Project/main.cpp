/**
 *  main.cpp
 *
 *  Main controller for drawing to XY-Plotter with Arduino
 *
 *  @author Drew Sommer
 *  @version 1.0.1
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
PinMap Y = { 7, 5, 6, 1, 340, 510, 50, 0 };

// LCD controller
LiquidCrystal lcd(9);
LiquidCrystal *lcd_pointer = &lcd;

//                           del servo up down (up / down angle, max, 71)
Drive *drive = new Drive(X, Y, 5, 10, 0, 71, lcd_pointer);

// Start drawing button, for after setting pen lower point
const int startButton = 2;

// Analog input for potentiometer for adjusting pen height
const int dial = 3;

// List of shapes to draw
Shape *shapes[50];

/*
    Serial interface control values

    Various values for controlling the XY-Plotter's flow, adding shapes, and
    doing work.

    Flow:
    -   Setup
    -   Pen
    -   Draw

    Setup. Get a list of shapes from the client.

        Shape syntax is in a form structured like:

        npC500,500,200,qp...qu

        n        : Command recognizing a connection is made
        p        : Shape data incoming
        C,E,B,P  : Shape type (C=Circle, E=Ellipse, B=Bezier, P=Polygon)
        0-99999, : integer value, depends on shape as to what it determines (see client code)
        q        : Shape data is done
        u        : list of shapes is completed

    Pen. Adjust the pen and press start.

    Draw. Draw the series of shapes.
 */
// Handshake is completed, and a connection is established
bool shook = false;

// Used by pen, helps to update LCD of pen low position
int temp = 0;

// Current index of shape being created or used
int ind = 0;

// Flow control values
bool set = true;   // Get shapes
bool draw = false; // Draw shapes
bool pen = false;  // Set pen

// Inchar to parse from client
char inChar;

// List of char to parse from client (numbers 0-99999)
char data[5];

// Index control for adding values to data
int dataInd = 0;

// Toggle for incoming shape data is ready, used to get shape type to set
bool incomingShapeDataReady = false;

// Shape data is incoming toggle, for numbers
bool incomingShapeData = false;

// toggle for completing entire drawing
bool completedEntireDrawing = false;

// Shape type 1=Cirlce, 2=Ellipse, 3=Bezier, 4=Polygon
int shapeType = 0;

// List of integer values to parse and pass into shapes
LinkedList<int> *values = new LinkedList<int>();


/**
 * Establish a connection with the client
 */
void handshake() {
    while(Serial.available() <= 0) {
        Serial.println(";Ready;");
        delay(300);
    }
    shook = true;
}

/**
 * Clean out all the old values from values to use in another shape
 */
void cleanValues(){
    while(values->size() > 0){
        values->remove(0);
    }
}

/**
 * Standard arduino setup
 */
void setup() {

    // Setup LCD screen
    lcd_pointer->begin(16, 2);
    lcd_pointer->noCursor();

    // Print a startup to LCD
    lcd_pointer->clear();
    lcd_pointer->print("Starting XY");

    // Start Serial
    Serial.begin(9600);

    // Setup drive (servo, pins, steppers, etc.)
    drive->attach();
}

/**
 * Standard arduino loop
 */
void loop() {

    // =========================================================================
    /*
     ██████ ██      ██ ███████ ███    ██ ████████
    ██      ██      ██ ██      ████   ██    ██
    ██      ██      ██ █████   ██ ██  ██    ██
    ██      ██      ██ ██      ██  ██ ██    ██
     ██████ ███████ ██ ███████ ██   ████    ██
    */
    // =========================================================================
    /**
     * Setup the shapes to be drawn. Connect to client and get a list of shapes
     * to draw.
     */
    while(set){

        // Serial.println(free_ram());

        // Setup connection if not already made
        if(!shook) handshake();

        // Data exists to be read
        if(Serial.available() > 0) {

            // We have incoming shape data
            if(incomingShapeData) {

                // Parse single character
                char v = (char)Serial.read();

                // End of number data
                if(v == ';') {
                    dataInd = 0; // Reset data index
                    // Serial.available() > 0 will end here and the data will be
                    // parsed in Serial.available() <= 0

                // End of shape data
                } else if(v == 'q'){
                    inChar = 'q';              // Toggle check for next section
                    incomingShapeData = false; // Toogle end of shape data

                // Add new data to (char)data array
                } else {
                    data[dataInd] = v; // Assign char
                    dataInd++;         // Increment index

                }

                // TODO: Clean LCD info
                lcd_pointer->clear();
                lcd_pointer->setCursor(0, 1);
                lcd_pointer->print("incomingShapeData");
                lcd_pointer->setCursor(0, 0);

                int val = 0;

                for(int i=0; i<5; i++){
                    if(data[i] != NULL){
                        val = (val*10) + (data[i] - '0');
                    }
                }
                lcd_pointer->print(val);
                lcd_pointer->print("      ");
                delay(50);
            // Command for what to do next data
            } else {
                inChar = (char)Serial.read(); // Read data command

                // TODO: clean/remove LCD info
                lcd_pointer->clear();
                lcd_pointer->print(inChar);
                delay(50);

            }
        }

        // Input from client is empty, parse chunk of data
        if(Serial.available() <= 0){

            // Connection was established, client sent 'n' confirmation
            if(inChar == 'n') {
                Serial.println(";next;"); // Ask for next chunk

            // Shape data is going to be sent next, prep for shape dat
            } else if(inChar == 'p') {
                incomingShapeDataReady = true; // Shape data will be coming
                Serial.println(";next;");      // Ask for next chunk

            // End of shape data, parse values into a shape
            } else if(inChar == 'q') {
                incomingShapeData = false; // Reset flag for incoming shape data

                // TODO: Remove Serial info (used for debugging and testing)
                // for(int i=0; i<values->size(); i++){
                //     Serial.print(values->get(i));
                //     Serial.print(",");
                // }

                // Parse data for a Circle
                if(shapeType == 1) {
                    int cx = values->get(0); // Get centre x
                    int cy = values->get(1); // Get centre y
                    int r = values->get(2);  // Get radius

                    // Assign circle to list
                    shapes[ind] = new Circle(cx, cy, r, drive, lcd_pointer);

                    ind++;         // Increment assignment index
                    cleanValues(); // Clean out values list

                    shapeType = 0;

                // Parse data for an Ellipse
                } else if(shapeType == 2) {

                    // Parse data for an Ellipse with rotation applied
                    Serial.println(values->size());
                    if(values->size() > 4) {
                        int cx = values->get(0); // Get centre x
                        int cy = values->get(1); // Get centre y
                        int a = values->get(2);  // Get a length
                        int b = values->get(3);  // Get b length

                        //        Get origin.x   Get origin.y
                        POS o = {values->get(4), values->get(5)};
                        double ang = values->get(6)*PI/180; // Get angle
                        // Values should be sent as degrees in integer form and
                        // will be converted to a double in radians. This
                        // simplifies the conversion as a decimal value would be
                        // far more difficult to convert. (This may yet change)

                        // Assign ellipse to list
                        shapes[ind] = new Ellipse(cx, cy, a, b, o, ang, drive, lcd_pointer);

                        ind++;         // Increment assignment index
                        cleanValues(); // Clean out values list

                        shapeType = 0;

                    } else {
                        int cx = values->get(0); // Get centre x
                        int cy = values->get(1); // Get centre y
                        int a = values->get(2);  // Get a length
                        int b = values->get(3);  // Get b length

                        // Assign ellipse to list
                        shapes[ind] = new Ellipse(cx, cy, a, b, drive, lcd_pointer);

                        ind++;         // Increment assignment index
                        cleanValues(); // Clean out values list

                        shapeType = 0;
                    }

                // Parse data for a Bezier curve
                } else if(shapeType == 3) {

                    // Get start point
                    //             p0.x           p0.y
                    POS p0 = {values->get(0), values->get(1)};

                    // Get first control point
                    //             p1.x           p1.y
                    POS p1 = {values->get(2), values->get(3)};

                    // Get second control point
                    //             p2.x           p2.y
                    POS p2 = {values->get(4), values->get(5)};

                    // Get end point
                    //             p3.x           p3.y
                    POS p3 = {values->get(6), values->get(7)};

                    // Assign bezier curve to list
                    shapes[ind] = new Bezier(p0, p1, p2, p3, drive, lcd_pointer);

                    ind++;         // Increment assignment index
                    cleanValues(); // Clean out values list

                    shapeType = 0;

                // Parse data for a Polygon
                } else if(shapeType == 4) {
                    // Create a new list for points
                    LinkedList<POS> *points = new LinkedList<POS>();

                    // Assign values to POS points in the points list
                    for(int i=0; i<values->size(); i+=2){
                        points->add({values->get(i), values->get(i+1)});
                    }

                    // Assign polygon to list
                    shapes[ind] = new Polygon(points, drive, lcd_pointer);

                    ind++;         // Increment assignment index
                    cleanValues(); // Clean out values list

                }

                // We hit our max array size, draw the first 50 shapes and ask
                // for more latter
                if(ind == 49) {
                    Serial.println(";wait;"); // Send wait command
                    set = false;              // stop asking for shapes
                    pen = true;               // Go to pen setup
                    ind = 0;                  // reset index for adding new shapes later

                } else {
                    Serial.println(";next;"); // Ask for next chunk

                }

            // End of shapes data, go to next step
            } else if(inChar == 'u') {
                set = false;                   // Toggle done getting shapes
                pen = true;                    // Toggle setup pen
                completedEntireDrawing = true; // Toggle so arduino does not ask
                                               // for more shapes later

                for(int i=0; i<50; i++){
                    if(shapes[i] != NULL){
                        Serial.print("List: ");
                        shapes[i]->print();
                    }
                    delay(50);
                }

            // Deal with other data characters
            } else {

                // Get ready for incoming shape data
                if(incomingShapeDataReady) {

                    // Set toggle for getting data
                    incomingShapeData = true;

                    // Set toggle for setup-for-shape-data
                    incomingShapeDataReady = false;

                    // Assign type of data to receive
                    if(inChar == 'C') shapeType = 1;
                    if(inChar == 'E') shapeType = 2;
                    if(inChar == 'B') shapeType = 3;
                    if(inChar == 'P') shapeType = 4;

                    // TODO: cleanup lcd info
                    lcd_pointer->print(shapeType);
                    Serial.println(";next;"); // Ask for next chunk

                // Parse incoming shape data, positional integers
                } else if(incomingShapeData) {

                    // Set initial integer
                    int val = 0;

                    // Loop through char data array
                    for(int i=0; i<5; i++){
                        if(data[i] != NULL){ // if index is not empty, add to val

                            // Multiply val by 10 and add single digit integer
                            // Parse data[i] char to proper integer
                            val = (val*10) + (data[i] - '0');

                            // EG: data = ['5', '0', '0'];
                            //
                            // val = 0 + 5 - '0'; (- '0' fixes int casting of char )
                            // val = 5*10 + 0 - '0'
                            // val = 50*10 + 0 -'0'
                            // val = 500
                        }
                    }
                    for(int i=0; i<5; i++){
                        data[i] = NULL;
                    }

                    // Add value to list
                    values->add(val);
                    Serial.println(";next;"); // Ask for next chunk

                }
            }
        }
        delay(50);

    }

    //==========================================================================
    /*
    ██████  ███████ ███    ██
    ██   ██ ██      ████   ██
    ██████  █████   ██ ██  ██
    ██      ██      ██  ██ ██
    ██      ███████ ██   ████
    */
    //==========================================================================
    while(pen){

        // Get pen lowering point, pot resistance 0-1023 to servo angle 0-71 degrees
        temp = drive->setPen(map(analogRead(dial), 0, 1023, 0, 71));

        // Inform the user through LCD
        lcd_pointer->clear();
        lcd_pointer->print("Pen: ");

        // Maps low point from inverted angle to percent 0 at lowest, 100 at highest
        lcd_pointer->print(map(temp, 0, 71, 100, 0));

        // Inform client as well
        Serial.print("Pen: ");
        Serial.println(map(temp, 0, 71, 100, 0));
        delay(300);

        // Start button pressed
        if(analogRead(startButton) > 1000) {
            pen = false; // Toggle pen setup
            draw = true; // Toggle draw section
            Serial.println("Start drawing");
            lcd_pointer->clear();
            lcd_pointer->print("Start drawing");
            delay(300);

        }

    }

    //==========================================================================
    /*
    ██████  ██████   █████  ██     ██
    ██   ██ ██   ██ ██   ██ ██     ██
    ██   ██ ██████  ███████ ██  █  ██
    ██   ██ ██   ██ ██   ██ ██ ███ ██
    ██████  ██   ██ ██   ██  ███ ███
    */
    //==========================================================================
    while(draw){

        // Loop through shapes and draw the shapes
        //
        // BUG: In between each shape plotter wants to reset to (0,0)
        for(int i=0; i<50; i++){
            if(shapes[i] != NULL) {
                shapes[i]->draw(true);
            }
        }

        // Done drawing
        draw = false;

        // We completed Entire Drawing stop doing thing
        if(completedEntireDrawing) {
            drive->moveTo(0,0); // Return to (0,0)

            // Inform client/user that we are done
            Serial.println("Done!");
            lcd_pointer->clear();
            lcd_pointer->print("Done!");

        // Get set setup so we can get more shapes!
        } else {
            set = true;
        }

        delay(300);
    }
}
