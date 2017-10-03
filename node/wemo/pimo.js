#!/usr/bin/env node

'use strict';

const FauxMo = require('fauxmojs');
var Gpio = require('onoff').Gpio,
    led = new Gpio(17, "out"),
    cb;


let fauxMo = new FauxMo({
    ipAddress: '192.168.1.112',
    devices: [{
            name: 'office light',
            port: 11000,
            handler: (action) => {
                console.log('office light action:', action);
            }
        },
        {
            name: 'pie light',
            port: 11001,
            /*      handler: (action) => {
                console.log('office fan action:', action);
		
            } */
            handler: (action) => write(action)
        },
        {
            name: 'Wandas light',
            port: 11002,
            handler: (action) => write(action)
        },
    ]
});

console.log('started..');

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
    console.log('State written to pin', action);
}
