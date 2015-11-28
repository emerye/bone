#!/usr/bin/env node

var Gpio = require('onoff').Gpio;
var led = new Gpio(4, 'out'); 

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

function ledon() {
  console.log("LED ON"); 
  led.writeSync(1); 
}

led.writeSync(1); 
setTimeout(function() {
   console.log('Starting Timeout'); 
   ledcntl(0); 
   }, 2000);  

setTimeout(function() { ledcntl(1) }, 5000); 

 
console.log("Hello World"); 
console.log(process.argv); 
console.log(process.argv.slice(2)); 
var cmdargs = process.argv.slice(2); 



