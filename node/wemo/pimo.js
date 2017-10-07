#!/usr/bin/env node
'use strict';

const FauxMo = require('fauxmojs');
var Gpio = require('onoff').Gpio,
    led = new Gpio(17, "out"),
    cb;
//var process = require('process'); 

process.argv.forEach((val, index) => {
  console.log(`${index}: ${val}`);
});

var tgtIpAddress = process.argv[2];
var portNumber = 11000;

let fauxMo = new FauxMo({
    ipAddress: tgtIpAddress,
    devices: [{
            name: 'office light',
            port: portNumber++,
            handler: (action) => {
                console.log('office light action:', action);
            }
        },
        {
            name: 'pie light',
            port: portNumber++,
            /*      handler: (action) => {
                console.log('office fan action:', action);
		
            } */
            handler: (action) => write(action)
        },
        {
            name: 'Wandas light',
            port: portNumber++,
            handler: (action) => write(action)
        },
        {
            name: 'pie light two',
            port: portNumber++,
            handler: (action) => write(action)
        },
        {
            name: 'andy',
            port: portNumber++,
            handler: (action) => write(action)
        },
        {
            name: 'andy one',
            port: portNumber++,
            handler: (action) => write(action)
        },

        {
            name: 'andy two',
            port: portNumber++,
            handler: (action) => write(action)
        },


        {
            name: 'andy three',
            port: portNumber++,
            handler: (action) => write(action)
        },

        {
            name: 'andy four',
            port: portNumber++,
            handler: (action) => write(action)
        },

        {
            name: 'andy five',
            port: portNumber++,
            handler: (action) => write(action)
        },

        {
            name: 'andy six',
            port: portNumber++,
            handler: (action) => write(action)
        },

        {
            name: 'andy seven',
            port: portNumber++,
            handler: (action) => write(action)
        },


        {
            name: 'andy eight',
            port: portNumber++,
            handler: (action) => write(action)
        },
    ]
});

console.log('Started on IP', tgtIpAddress);


// functions

function message(action) {
    console.log("Office log", action);
}


function write(action) {
    if (action == 'off') {
        led.writeSync(0);
    } else {
        led.writeSync(1);
    }
    console.log('State written to pin: ', action);
}

setTimeout(function() {
  console.log('hello there'); 
}, 7000); 
