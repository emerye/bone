#!/usr/bin/env node

var Gpio = require('onoff').Gpio;
var led = new Gpio(4, 'out'); 
var ledState = 0; 


function sleep(time, callback) {
  var stop = new Date().getTime(); 
  while(new Date().getTime() < stop + time) { 
    ;
  }
  callback(); 
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


console.log("Hello World"); 
console.log(process.argv); 
console.log(process.argv.slice(2)); 
var cmdargs = process.argv.slice(2); 

led.writeSync(1); 
setTimeout(function() {
   console.log('Starting Timeout'); 
   ledcntl(0); 
   }, 2000);  


setTimeout(function() { ledcntl(1) }, 5000); 

while(1) {
  if (ledState == 1) {
    ledcntl(0); 
    ledState = 0; 
  } else {
    ledcntl(1); 
    ledState = 1; 
  }
    sleep(100, function() { 
    }); 
}

// process.exit(); 
 
