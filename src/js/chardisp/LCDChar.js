#!/usr/bin/env node

var i2c = require('i2c-bus'); 
var utils = require('./utils.js'); 

const LCD_CLEAR_DISPLAY = 0x01; 
const LCD_RETURN_HOME = 0x02;  
const LCD_ENTRY_MODE_SET = 0x04; 
const ENTRY_MODE_LEFT = 0x02;  
const ENTRY_MODE_S = 0x01; 
const LCD_DISPLAY_ON_OFF = 0x08;  
const DISPLAY_ENTIRE = 0x04;  
const DISPLAY_CURSOR = 0x02;  
const DISPLAY_CURSOR_POS = 0x01; 
const LCD_CURSOR_DISPLAY = 0x10;  
const CURSOR_DISPLAY_SC = 0x08;
const CURSOR_DISPLAY_RL = 0x04;  
const LCD_FUNCTION_SET = 0x20;  
const LCD_CGRAM_ADDR = 0x40;  
const LCD_DDRAM_ADDR = 0x80;  
const HDWENABLELINE = 0x04; 
const LCDBACKLIGHT = 0x08;  


//Constructor
function LCDChar(devHandle, tgtAddress) {
	this.devHandle = devHandle; 
	this.tgtAddress = tgtAddress;  
}; 


// Methods 
LCDChar.prototype.LCDinit = function() {
	console.log('LCDinit Method called. Target address is ', this.tgtAddress );   
}; 


LCDChar.prototype.sleep = function (time, callback) {
  var stop = new Date().getTime(); 
  while(new Date().getTime() < stop + time) { 
    ;
  }
  callback(); 
}


LCDChar.prototype.WriteNibble = function(msbtoWrite, cmd) {
  var bytetoWrite = 0;

  bytetoWrite = bytetoWrite | (msbtoWrite & 0xF0) | HDWENABLELINE | LCDBACKLIGHT | cmd; 
  try {
    this.devHandle.sendByteSync(this.tgtAddress, bytetoWrite); 
  } catch (err) {
    console.log('Error in Utils.WriteNibble :', err); 
  }

  //Enable low
  bytetoWrite &= ~HDWENABLELINE; 
  bytetoWrite = bytetoWrite | (msbtoWrite & 0xF0) | LCDBACKLIGHT | cmd; 
  try {
    this.devHandle.sendByteSync(this.tgtAddress, bytetoWrite); 
  } catch (err) {
    console.log('Error in Utils.WriteNibble :', err); 
  }
 
  bytetoWrite |=  HDWENABLELINE; 
  bytetoWrite = bytetoWrite | (msbtoWrite & 0xF0) | LCDBACKLIGHT | cmd; 
  try {
    this.devHandle.sendByteSync(this.tgtAddress, bytetoWrite); 
  } catch (err) {
    console.log('Error in Utils.WriteNibble :', err); 
  }
}


LCDChar.prototype.WriteByte = function (bytetoWrite, cmd) {
  var lsb; 

  lsb = (bytetoWrite << 4) & 0xF0;  
  this.WriteNibble((bytetoWrite & 0xF0), cmd);   
  this.WriteNibble(lsb , cmd);   
}


LCDChar.prototype.LcdInit = function () {
  
  this.sleep(200, function(){});
  this.WriteNibble(0x30, 0); 
  this.sleep(5, function(){}); 
  this.WriteNibble(0x30, 0); 
  this.sleep(10, function(){});
  this.WriteNibble(0x30, 0); 
  this.sleep(1, function(){});
  this.WriteNibble(0x20, 0); 
  this.sleep(1, function(){});
  this.WriteByte(0x28, 0); 
  this.sleep(1, function(){});

  this.WriteByte(LCD_CLEAR_DISPLAY, 0); 
  this.sleep(2, function(){});
  this.WriteByte(LCD_RETURN_HOME, 0); 
  this.sleep(1, function(){});
  this.WriteByte(LCD_CURSOR_DISPLAY, 0); 
  this.sleep(1, function(){});
  this.WriteByte(LCD_DISPLAY_ON_OFF | DISPLAY_ENTIRE, 0); 
  this.sleep(1, function(){});
  this.WriteByte(LCD_RETURN_HOME, 0); 
  this.sleep(2, function(){});
} 

//Export this class
module.exports = LCDChar; 
