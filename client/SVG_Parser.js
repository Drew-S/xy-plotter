/**
 *  SVG_Parser.js
 *
 *  Parses a SVG file into a list of commands for the XY-Plotter.
 *
 *  Returned list has the following format:
 *
 * ['u', 'p', ('C'|'P'|'B'|'E'), ..., 'q', 'n']
 *
 *     'u'          Informs the Plotter that a new drawing is going to be send
 *                  (multiple shapes).
 *
 *     'p'          The begining of new Shape data will be sent.
 *
 *     'C|P|B|E'    The type of shape data to be sent.
 *
 *         'C'      Circle data
 *                  ['p', 'C', cx, cy, r, 'q']
 *                  cx = centre x position
 *                  cy = centre y position
 *                  r  = radius
 *
 *                  eg:
 *                      Circle(100, 100, 50)
 *                      ['p', 'C', '100;', '100;', '50;', 'q']
 *
 *         'P'      Polygon data
 *                  ['p', 'P', p0.x, p0.y, p1.x, p1.y, ..., pn.x, pn.y 'q']
 *                  pn.x = x position
 *                  pn.y = y position
 *
 *                  eg:
 *                      Polygon({100, 100}, {100, 200}, {200, 100})
 *                      ['p', 'P', '100;', '100;', '100;', '200;', '200;', '100;', 'q']
 *
 *         'B'      Bezier curve data
 *                  ['p', 'B', p0.x, p0.y, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, 'q']
 *                  p0.x = Start point x
 *                  p0.y = Start point y
 *                  p1.x = First control point x
 *                  p1.y = First control point y
 *                  p2.x = Second control point x
 *                  p2.y = Second control point y
 *                  p3.x = End point x
 *                  p3.y = End point y
 *
 *                  eg:
 *                      Bezier({100, 100}, {100, 200}, {200, 200}, {200, 100})
 *                      ['p', 'B', '100;', '100;', '100;', '200;', '200;', '200;', '200;', '100;']
 *
 *         'E'      Ellipse data
 *                  ['p', 'E', cx, cy, a, b, (origin.x, origin.y, angle), 'q']
 *                  cx       = Centre x position
 *                  cy       = Centre y position
 *                  a        = width
 *                  b        = height
 *                (optional)
 *                  origin.x = Rotation x position
 *                  origin.y = Rotation y position
 *                  angle    = Rotation angle (integer)(degrees)
 *
 *                  eg:
 *                      Ellipse(100, 100, 50, 10)
 *                      ['p', 'E', '100;', '100;', '50;', '10;', 'q']
 *
 *                      Ellipse(100, 100, 50, 10, {100, 100}, 45)
 *                      ['p', 'E', '100;', '100;', '50;', '10;', '100;', '100;', '45;', 'q']
 *
 *     '...;'       The integer values for the shape data, 0-99999; the ';' is
 *                  to inform the Plotter the number is done and to go to next
 *                  number
 *
 *  @author Drew Sommer
 *  @version 1.0.0
 *  @license MIT (https://mit-license.org)
 */
const fs         = require('fs'),
      parse      = require('xml-parser');

var comma = ';';

parse_transform = (string) => {
    var match = string.match(/([a-zA-Z]+)|([0-9\.]+)/g);

    var val = [];


    for(var i=1; i<match.length; i++){
        val.push(match[i]);
    }

    return object = {
        type: match[0],
        values: val
    };
}

/**
 * Convert mm length to steps`
 * @param  {Number} x Length
 * @return {Number}   Steps
 *
 Latex:
$$
\frac{x}{\frac{1.8\pi}{180}\times\frac{13}{2}}
$$
 */
mm = (x) => {
    if(typeof x === 'string') x = Number(x);
    var rat = ((1.8*Math.PI)/180)*(13/2);
    return Math.round(x/rat);
}

/**
 * Convert object data into a Circle command array
 * @param  {Object} obj Circle object to parse
 * @return {Array}      Command list ['p', 'C', ..., 'q']
 */
Circle = (obj)  => {
    return ['p', 'C',
        mm(obj.cx)+comma,
        mm(obj.cy)+comma,
        mm(obj.r)+comma, 'q'];
}

/**
 * Convert object data into an Ellipse command array
 * @param  {Object} obj Ellipse object to parse
 * @return {Array}      Command list ['p', 'E', ..., 'q']
 */
Ellipse = (obj) => {
    var arr = ['p', 'E',
        mm(obj.cx)+comma,
        mm(obj.cy)+comma,
        mm(obj.rx)+comma,
        mm(obj.ry)+comma];

    if(obj.transform){
        var transform = parse_transform(obj.transform);
        if(transform.type = 'rotate') {
            arr.push(mm(obj.cx)+comma);
            arr.push(mm(obj.cy)+comma);
            arr.push(Math.round(transform.values[0])+comma);
        }
    }
    arr.push('q');
    return arr;
}

/**
 * Convert object data into a Bezier curve command array.
 *
 * The path may hold far more than a single bezier curve, so it does parse
 * multiple Bezier curves from a single path.
 *
 * @param  {Object} object Path object to parse
 * @return {Array}         Command list ['p', 'B', ..., 'q', ('p', 'B', ..., 'q')]
 */
Path = (object) => {

    // Get a list of numbers for path data
    var data = object.d.match(/([0-9\.]+)/g);

    // Parse the 8 numbers (4 control points) into a Bezier curve data
    function Bezier(data) {
        for(var i=0; i<data.length; i++){
            data[i] = mm(data[i])+comma;

        }
        var list = ['p', 'B'];
        list = list.concat(data);
        list.push('q');

        return list;

    }

    var list = [];

    var Bs = ((data.length - 6 ) / 8) + 1;

    // Loop through the numbers and create Bezier command array(s)
    for(var i=0; i<Bs; i++){
        var mult = (i - 1) * 8;
        if(i == 0) {
            list = list.concat(Bezier(data.slice(0, 8)));
        } else {
            list = list.concat(Bezier(data.slice(6+mult, 14+mult)));
        }
    }

    return list;

}

/**
 * Convert array of points into a Polygon command array
 * @param  {Array} points Array of points
 * @return {Array}        Polygon command array ['p', 'P', ..., 'q']
 */
Polygon = (points) => {
    var arr = ['p', 'P'];
    for(var i=0; i<points.length; i++){
        arr.push(mm(points[i].x)+comma);
        arr.push(mm(points[i].y)+comma);
    }
    arr.push('q');
    return arr;
}

/**
 * Convert a Rect object into a Polygon command array
 * @param  {Object} obj Rect object
 * @return {Array}      Polygon command array ['p', 'P', ..., 'q']
 */
Rect = (obj) => {
    var x, y, width, height,
    points = [];

    x  = obj.x;
    y  = obj.y;
    width = obj.width;
    height = obj.height;

    points.push({ x: x, y: y });
    points.push({ x: width, y: y });
    points.push({ x: width, y: height });
    points.push({ x: x, y: height });
    points.push({ x: x, y: y });

    return Polygon(points);
}

/**
 * Exports parser for parsing a SVG file into a command list
 * @param  {string} file Path of the file to read
 * @return {Array}      Command list ['n', 'p', ..., 'q', 'u']
 */
module.exports = function(file){
    // Command list to build on
    var list     = ['n'], // Start drawing data command

        // Read and parse the file
        SVG      = parse(fs.readFileSync(file, 'utf8')).root,
        shapes;

    // Loop through all of the children of SVG and find 'g' (where the shapes
    // are held)
    for(var i of SVG.children){
        if(/g/g.test(i.name)){
            shapes = i.children; // Save shapes
            break;
        }
    }

    // Used for logging data to console (debugging)
    var l = 0;

    // Loop through and parse the shapes
    for(var i of shapes){

        // Parse Rectangle data
        if(i.name == 'rect') list = list.concat(Rect(i.attributes));

        // Parse Ellipse data
        else if(i.name == 'ellipse') list = list.concat(Ellipse(i.attributes));

        // Parse Circle data
        else if(i.name == 'circle') list = list.concat(Circle(i.attributes));

        // Parse Path data
        else if(i.name == 'path') list = list.concat(Path(i.attributes));

        // Log shape command array
        console.log(list.slice(l, list.length));
        l = list.length;

    }
    list.push('u'); // Add "finish drawing data command"

    return list;
};
