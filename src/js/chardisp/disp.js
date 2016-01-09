#!/usr/bin/env node

var I2CADDR = 0x27;

var i2c = require('i2c-bus');
var utils = require('./utils.js'); 
var Gpio = require('onoff').Gpio;
var led = new Gpio(4, 'out'); 
var ledState = 0; 
var i2c1 = null; 
var lcdCls = require('./lcdcls.js'); 

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


function exit() {
  led.unexport(); 
  process.exit();
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


OpenI2C(); 
utils.LcdInit(i2c1, 0x27); 

console.log(process.argv); 
console.log(process.argv.slice(2)); 

var cmdargs = process.argv.slice(2); 

utils.WriteByte(i2c1,0x27,0x41, 1); 
utils.WriteByte(i2c1,0x27,0x42, 1); 
utils.WriteByte(i2c1,0x27,0x43, 1); 
utils.WriteByte(i2c1,0x27,0x44, 1); 

var lcdDisp = new lcdCls(i2c1, 0x27); 
lcdDisp.LCDinit();  

var lcd2 = new lcdCls(i2c1, 1); 
lcd2.LCDinit(); 

var lcd3 = new lcdCls(i2c1, 2); 
lcd3.LCDinit(); 

led.writeSync(1); 
setTimeout(function() {
   console.log('Starting Timeout'); 
   ledcntl(0); 
   }, 2000);  


setTimeout(function() { ledcntl(1) }, 500); 
/*
while(1) {
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
*/

i2c1.closeSync(); 
process.exit(); 
 
