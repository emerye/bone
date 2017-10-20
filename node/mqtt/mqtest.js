var mqtt = require('mqtt')
//var client  = mqtt.connect('mqtt://localhost')
var client  = mqtt.connect([{host: 'localhost', port: 1883 }])
 
var myarray = ['one', 'two', 'three'];

console.log("%s\n", myarray[2]); 

client.on('connect', function () {
  client.subscribe('presence')
  client.publish('presence', 'Hello mqtt')
})
 
client.on('message', function (topic, message) {
  // message is Buffer
  console.log(message.toString())
  client.end()
})
