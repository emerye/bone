#!/usr/bin/env node
"use strict"; 

var I2CADDR = 0x27;

var i2c = require('i2c-bus');
var utils = require('./utils.js'); 
var Gpio = require('onoff').Gpio;
var led = new Gpio(4, 'out'); 
var ledState = 0; 
var i2c1 = null; 
var lcdchar = require('./LCDChar.js');

function OpenI2C() {
  try { 
  i2c1 = i2c.openSync(1); 
  } 
  catch (err) { //Does not work.  
    console.log("Opening I2C port failed.", err); 
  }

  try {
   i2c1.sendByteSync(I2CADDR, 0x08); 
  } 
    catch (err) {
      console.log("Exception sending byte.", err); 
  }
}

function ledcntl(state) {
  led.writeSync(state); 
  if (state == 1) {
    console.log("LED ON");
    }
    else {
    console.log("LED OFF"); 
  }
}

function exit() {
  button.unexport(); 
  led.unexport(); 
  process.exit();
}

var button = new Gpio(17, 'in', 'both'); // Export GPIO #17, pin 11, as an interrupt
                                      // generating input.

console.log('Please press the button on GPIO #17...');
// The callback passed to watch will be called when the button on GPIO #4 is
// pressed. 
button.watch(function (err, value) {
  if (err) {
    throw err;
  }
  led.writeSync(value);  
  console.log('Button pressed!, its value was ' + value);

});



OpenI2C(); 

var charObj = new lcdchar(i2c1, 0x27); 
charObj.LCDInit(); 

console.log(process.argv); 
console.log(process.argv.slice(2)); 

utils.WriteByte(i2c1,0x27,0x47, 1); 

charObj.LCDWriteString(0, 0, 'Today is Jan 10th');  
charObj.LCDWriteString(1, 0, 'Test tomorrow');  
charObj.LCDWriteString(2, 0, 'Go to Thunder Valley');  
charObj.LCDWriteString(3, 0, 'Today is Sunday');  

i2c1.closeSync(); 

led.writeSync(1); 
setTimeout(function() {
   console.log('Starting Timeout'); 
   ledcntl(0); 
   }, 2000);  

setTimeout(function() { ledcntl(1) }, 500); 

for (var i=0; i<5; i++)  {
  if (ledState == 1) {
    ledcntl(0); 
    ledState = 0; 
  } else {
    ledcntl(1); 
    ledState = 1; 
  }
    utils.sleep(1000, function() { 
    }); 
}

i2c1.closeSync(); 
//exit(); 
//process.exit(); 
 
