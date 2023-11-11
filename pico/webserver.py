import network
import socket
import time
from machine import Pin

led = Pin("LED", Pin.OUT)
ssid = 'MickeyNet'
password = 'andy-wanda'
led_one = Pin(14, Pin.OUT)
led_two = Pin(15, Pin.OUT)

def Website():
    value_one = led_one.value()
    value_two = led_two.value()
    website = """<!DOCTYPE html>
    <html>
        <head> <title>Lutz Embedded Tec! Raspberry Pico W</title> </head>
        <body>
            <h1>Lutz Embedded Tec! Raspberry Pico W</h1>
            
            <table style="width:400px" class="center">
                  <tr>
                    <th><center>LED Number </center></th>
                    <th><center>Button </center> </th>
                    <th><center>Pin State</center> </th>
                  </tr>
                  <tr>
                    <td><center>one </td>
                    <td><center><input type='button' value='toggle' onclick='toggleLed("one")'/> </center></td>
                    <td> <center>  <span id="value_one">""" + str(value_one) + """</span></center> </td>
                  </tr>
                  <tr>
                    <td><center>two</center> </td>
                    <td><center><input type='button' value='toggle' onclick='toggleLed("two")'/></center></td>
                    <td><center>  <span id="value_two">""" + str(value_two) + """</span></center></td>
                   </tr>
            </table>
            
            <input type='button' value='update' onclick='update()'/>
                        
            <script>
                function toggleLed(led){
                    var xhttp = new XMLHttpRequest();
                    xhttp.open('GET', '/led/'+led, true);
                    xhttp.send();
                }
                function update(){
                    location.reload(true);
                }
                    
            </script>
        </body>
    </html>
    """
    return website

wlan = network.WLAN(network.STA_IF)
wlan.active(True)
wlan.connect(ssid, password)


html = """<!DOCTYPE html>
<html>
    <head> <title>Pico W</title> </head>
    <body> <h1>Pico W</h1>
        <p>%s</p>
        <p>Andy's Pico</p>
        <p>One other line</p>
    </body>
</html>
"""

led.toggle()
max_wait = 10
while max_wait > 0:
    if wlan.status() < 0 or wlan.status() >= 3:
        break
    max_wait -= 1
    print('waiting for connection...')
    time.sleep(1)

if wlan.status() != 3:
    raise RuntimeError('network connection failed')
else:
    print('connected')
    status = wlan.ifconfig()
    print( 'ip = ' + status[0] )

addr = socket.getaddrinfo('0.0.0.0', 80)[0][-1]

s = socket.socket()
s.bind(addr)
s.listen(1)


print('listening on', addr)

# Listen for connections
while True:
    try:
        cl, addr = s.accept()
        print('client connected from', addr)
        request = cl.recv(1024)
        print(request)
        
        request = str(request)
        led_on = request.find('/light/on')
        led_off = request.find('/light/off')
        print( 'led on = ' + str(led_on))
        print( 'led off = ' + str(led_off))
        stateis = 'Undefined'

        if led_on == 6:
            print("led on")
            led.value(1)
            stateis = "LED is ON"

        if led_off == 6:
            print("led off")
            led.value(0)
            stateis = "LED is OFF"

        response = html % stateis

        cl.send('HTTP/1.0 200 OK\r\nContent-type: text/html\r\n\r\n')
        cl.send(response)
        print('led toggle')
        led.toggle()
        cl.close()
    

    except OSError as e:
        cl.close()
        print('connection closed')