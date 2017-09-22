# XY-Plotter

### 1.1.1

This project includes code, drawings, and schematics for building a XY-Plotter. The code is all made in c++ using [Platform.io](http://platformio.org) to compile and run on an Arduino.

## Code

### NOTE

**The current version of `main.cpp`, where the server code runs from is being used for testing. This version of the code is not ready for full use.**

This code uses:
-   `LinkedList` from [PaulMurrayCbr](http://github.com/PaulMurrayCbr/LinkedList)
-   `ShiftedLCD` from [omersiar](https://github.com/omersiar/ShiftedLCD)

The code is operating well enough to draw basic shapes:
-   Circle (working)
-   Ellipse (with rotations) (working)
-   Polygon (Square, Diamond, Rectangle, etc) (working)
-   Bezier curve (working)

The Drive functionallity has some quirks with `moveTo()`, taking odd paths to get to next shape. The shapes themselves are unaffected. Without fixing this the system still works well. However, without fixing them it runs slower as moving from one shape to another takes uneeded steps wasting time.

These four basic types will allow the drawing of, I believe any shape.

The client code, which has yet to started, will convert shapes into operable types for the server code (Arduino code).

I plan to make the client code convert SVG files into server code for easy drawing.

## CAD and Drawings

I have removed the old CAD and drawing files as they're outdated.

I will be drawing up new ones soon.

## Circuit

Look in the `docs/circuit` folder for diagrams.

The `Master.svg` file shows the entire diagram for the circuit. The diagrams are divided into smaller diagrams for easier reading.

![Master Circuit Diagram](https://raw.githubusercontent.com/Drew-S/xy-plotter/master/docs/circuit/Master.png)

## Author

Drew Sommer

## License

The MIT License (MIT)

Copyright (c) 2017 Drew Sommer

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
