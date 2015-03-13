var http = require("http");
fs = require('fs');
url = require('url'); 
var htmlbreak = "<br />";
var h1 = "<h1>"; 
var endh1 = "</h1>";

http.createServer(function(request, response) {
  var b = require('bonescript');
  var d = new Date(); 
  var dateString; 
  
  var img = fs.readFileSync('/root/bone/cooper.jpg'); 
  imagedata = new Buffer(img).toString('base64'); 
 
  response.writeHead(200, {"Content-Type": "text/html"});
  response.write('<img src="data:image/jpeg;base64,'+imagedata+'">');//send image
  response.write(h1);
  response.write("\n\nLocal time: " + d.toString() + "<br />"); 
  response.write(htmlbreak); 

  b.pinMode('P8_9', b.INPUT); 
 
  var state = b.LOW;
  b.digitalRead('P8_9', check); 

  setTimeout(check, 500); 
 // setInterval(toggle, 1000); 
    

function toggle() {
   if(state == b.LOW) 
	state = b.HIGH;
   else 
        state = b.LOW; 
   b.digitalWrite(ledPin, state); 
}

function check(){
  b.digitalRead('P8_9', checkSwitch);
}

function checkSwitch(x) {
  if(x.value == 1) 
    response.write(h1 + "Garage Door is Closed." + endh1); 
  else 
    response.write(h1 + "Garage Door is Open.\n" + endh1); 
  response.end(); 
}


}).listen(2000);
