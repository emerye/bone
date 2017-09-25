'use strict';
 
const FauxMo = require('fauxmojs');
 
let fauxMo = new FauxMo(
	  {
		      ipAddress: '192.168.1.112',
		      devices: [
			            {
					            name: 'office light',
					            port: 11000,
					            handler: (action) => {
							              console.log('office light action:', action);
							            }
					          },
			            {
					            name: 'office fan',
					            port: 11001,
					            handler: (action) => {
							              console.log('office fan action:', action);
							            }
					          }
			          ]
		    });
 
console.log('started..');
