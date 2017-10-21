'use strict'


var mqtt = require('mqtt')
var client  = mqtt.connect('mqtt://mqtt:Mosq@localhost')
//var client  = mqtt.connect([{host: 'localhost', port: 1883, username: 'andy', password: new Buffer('1wanda1')}])
var count = 0; 
var myarray = ['one', 'two', 'three'];

console.log("%s\n", myarray[2]); 

client.on('connect', function () {
  client.subscribe('presence')
  client.publish('presence', 'Hello mqtt')
  setInterval(sendPublish, 1000); 
})
 
client.on('message', function (topic, message) {
  // message is Buffer
  console.log(message.toString())
//  client.end()
})

function sendPublish() {
  client.publish('presence', count.toString());
  count += 1;
}
  
