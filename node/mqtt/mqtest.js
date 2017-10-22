'use strict'

var mqtt = require('mqtt')
var client  = mqtt.connect('mqtt://mqtt:Mosq@localhost')
//var client  = mqtt.connect([{host: 'localhost', port: 1883, username: 'andy', password: new Buffer('1wanda1')}])
var count = 0; 
var myarray = ['one', 'two', 'three'];
var err, granted; 

console.log("%s\n", myarray[2]); 

client.on('connect', function () {
   client.subscribe('device/#'); 
   //  client.publish('presence', 'Hello mqtt')
   //  setInterval(sendPublish, 1000); 
   console.log("Subscribed\n"); 
});
 
client.on('message', function (topic, message) {
  console.log("Topic:%s  Message:%s\n\n", topic, message.toString());  
  processMessage(topic, message); 
//  client.end()
}); 

client.on('error', function (err) {
  // message is Buffer
 console.log("In Error\n");  
  console.log("ERR:%s \n", err);  
//  client.end()
}); 


 function processMessage(topic, message) {

  console.log("Process message topic:%s  Message:%s\n", topic, message); 
  
  switch(topic) {
	case 'device/socket1':
          pwrControl(1, message); 
 	break;
        case 'device/sockete2':
	 pwrControl(2, message);  
	break; 
        case 'device/socket3': 
	 pwrControl(3, message);  
        break; 
  }
}

function pwrControl(socketNum, message) {
	console.log("SocketNum:%d  Message: %s\n", socketNum, message); 
        if (message === 'on') {
	   
	   }
	else {
		
	}
}
