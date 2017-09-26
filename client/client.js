/**
 *  client.js
 *
 *  Controller for connecting and command XY-Plotter
 *
 *  @author Drew Sommer
 *  @version 1.0.0
 *  @license MIT (https://mit-license.org)
 */
const SerialPort = require('serialport'),
      os         = require('os'),
      fs         = require('fs'),
      https      = require('https'),
      SVG_parser = require('./SVG_Parser');

// Get command array
var list = SVG_parser('../TEST.svg'),
    ind  = 0;

// console.log(list);

// Portname for arduino
var portname = null;

// Get the portname for Linux '/dev/ttyUSB#'
if(os.platform() == 'linux') {
    var files = fs.readdirSync('/dev/');
    for(var file of files) {
        if(/^ttyUSB\d+$/.test(file)){
            portname = '/dev/'+file;
            break;
        }
    }
}
// TODO: add checks and get portname for other OS's

if(portname != null) {

    console.log("Controlling XY-Plotter on: " + portname);

    // Create serial port connection
    serialPort = new SerialPort(portname, {
        baudrate: 9600,
        dataBits: 8,
        parity: 'none',
        stopBits: 1,
        flowControl: false

    }, (err) => { if(err) throw err; });

    // Data from arduino
    var dataString = '';

    // When serial port opens
    serialPort.on('open', () => {

        // On getting data (Serial.print(ln));
        serialPort.on('data', (data) => {
            dataString+=data; // Add data chunk to string

            if(/\r|\n/.test(dataString)){ // When we get a newline (Serial.println)

                // Remove newline/carriage return from string
                dataString = dataString.replace(/\r|\n/g, '');

                // The arduino is trying to establish a connection
                if(dataString == ';Ready;'){
                    console.log('Send: n');
                    if(ind != 0){
                        console.log('done!');
                    }
                    serialPort.write('n'); // Send 'n', informing we have a
                                           // and we are ready to send data
                }

                // The arduino wants the next chunk of data
                if(dataString == ';next;'){
                    console.log('Send: '+list[ind]);
                    serialPort.write(list[ind]); // Send the next chunk of data
                    ind++;
                }

                // If the string is not ';Ready;' or ';next;' print data
                if(!/;next;|;Ready;/g.test(dataString)) console.log(dataString);
                dataString = ''; // Reset data string

            }
        });

    });

}

// Create a server so the application does not close imediatly. Without this
// the script would run once and not actually talk with the arduino.

// TODO: add a check for a completed list of commands string from arduino, and
//       use that to close server.
var port = 8000;

https.createServer({}, (req, res) => {

}).listen(port, (err, data) => {
    console.log("Server running on port: " + port);

});
