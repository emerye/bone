var i2c = require('i2c-bus'); 
var utils = require('./utils.js'); 


//Constructor
function LCDChar(devHandle, tgtAddress) {
	this.devHandle = devHandle; 
	this.tgtAddress = tgtAddress;  
}; 

// Methods 
LCDChar.prototype.LCDinit = function() {
	console.log('LCDinit Method called. Target address is ', this.tgtAddress );   
}; 

//Export this class
module.exports = LCDChar; 




