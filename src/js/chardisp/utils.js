#!/usr/bin/env node

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

exports.sleep = function (time, callback) {
  var stop = new Date().getTime(); 
  while(new Date().getTime() < stop + time) { 
    ;
  }
  callback(); 
}


exports.WriteNibble = function(i2cHandle, tgtAddress, msbtoWrite, cmd) {
  var bytetoWrite = 0;

  bytetoWrite = bytetoWrite | (msbtoWrite & 0xF0) | HDWENABLELINE | LCDBACKLIGHT | cmd; 
  try {
    i2cHandle.sendByteSync(tgtAddress, bytetoWrite); 
  } catch (err) {
    console.log('Error in Utils.WriteNibble :', err); 
  }

  //Enable low
  bytetoWrite &= ~HDWENABLELINE; 
  bytetoWrite = bytetoWrite | (msbtoWrite & 0xF0) | LCDBACKLIGHT | cmd; 
  try {
    i2cHandle.sendByteSync(tgtAddress, bytetoWrite); 
  } catch (err) {
    console.log('Error in Utils.WriteNibble :', err); 
  }
 
  bytetoWrite |=  HDWENABLELINE; 
  bytetoWrite = bytetoWrite | (msbtoWrite & 0xF0) | LCDBACKLIGHT | cmd; 
  try {
    i2cHandle.sendByteSync(tgtAddress, bytetoWrite); 
  } catch (err) {
    console.log('Error in Utils.WriteNibble :', err); 
  }
}


exports.WriteByte = function (i2cHandle, tgtAddress, bytetoWrite, cmd) {
  var lsb; 
 
  lsb = (bytetoWrite << 4) & 0xF0;  
  this.WriteNibble(i2cHandle, tgtAddress, (bytetoWrite & 0xF0), cmd);   
  this.WriteNibble(i2cHandle, tgtAddress, lsb , cmd);   
}

exports.LcdInit = function (i2cHandle, tgtAddress) {
  this.sleep(200, function(){});
  this.WriteNibble(i2cHandle, tgtAddress, 0x30, 0); 
  this.sleep(5, function(){}); 
  this.WriteNibble(i2cHandle, tgtAddress, 0x30, 0); 
  this.sleep(10, function(){});
  this.WriteNibble(i2cHandle, tgtAddress, 0x30, 0); 
  this.sleep(1, function(){});

  this.WriteNibble(i2cHandle, tgtAddress, 0x20, 0); 
  this.sleep(1, function(){});
  this.WriteByte(i2cHandle, tgtAddress, 0x28, 0); 
  this.sleep(1, function(){});

  this.WriteByte(i2cHandle, tgtAddress, LCD_CLEAR_DISPLAY, 0); 
  this.sleep(2, function(){});
  this.WriteByte(i2cHandle, tgtAddress, LCD_RETURN_HOME, 0); 
  this.sleep(1, function(){});
  this.WriteByte(i2cHandle, tgtAddress, LCD_CURSOR_DISPLAY, 0); 
  this.sleep(1, function(){});
  this.WriteByte(i2cHandle, tgtAddress, LCD_DISPLAY_ON_OFF | DISPLAY_ENTIRE, 0); 
  this.sleep(1, function(){});
  this.WriteByte(i2cHandle, tgtAddress, LCD_RETURN_HOME, 0); 
  this.sleep(2, function(){});
} 

