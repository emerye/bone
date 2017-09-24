""" 
  Author: Surendra Kane

  Script to control individual Raspberry Pi GPIO's.
  Applicable ONLY for Raspberry PI 3, based on schematics.
  Please modify for other board versions to control correct GPIO's.
"""

import fauxmo
import logging
import time
import RPi.GPIO as GPIO

from debounce_handler import debounce_handler

logging.basicConfig(level=logging.DEBUG)

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)

gpio_ports = {'gpio1':1,'gpio2':2,'gpio3':3,'gpio4':4,'gpio5':5,'gpio6':6,'gpio7':7,'gpio8':8,'gpio9':9,'gpio10':10,'gpio11':11,'gpio12':12,'gpio13':13,'gpio14':14,'gpio15':15,'gpio16':16,'gpio17':17,'gpio18':18,'gpio19':19,'gpio20':20,'gpio21':21,'gpio22':22,'gpio23':23,'gpio24':24,'gpio25':25,'gpio26':26}

class device_handler(debounce_handler):
    """Triggers on/off based on GPIO 'device' selected.
       Publishes the IP address of the Echo making the request.
    """
    """
    TRIGGERS = {"gpio1":50001,
		"gpio2":50002,
		"gpio3":50003,
		"gpio4":50004,
		"gpio5":50005,
		"gpio6":50006,
		"gpio7":50007,
		"gpio8":50008,
		"gpio9":50009,
		"gpio10":50010,
		"gpio11":50011,
		"gpio12":50012,
		"gpio13":50013,
		"gpio14":50014,
    """
    TRIGGERS = {"gpio15":50015,
		"gpio16":50016,
		"gpio17":50017,
		"gpio18":50018,
		"gpio19":50019,
    		"gpio20":50020,
		"gpio21":50021,
		"gpio22":50022,
		"gpio23":50023,
		"gpio24":50024,
                "gpio25":50025,
        	"gpio26":50026}

    def trigger(self,port,state):
      print('port: %d , state: %s', port, state)
      if state == True:
        GPIO.setup(port, GPIO.OUT)
        GPIO.output(port,GPIO.HIGH)
      else:
        GPIO.setup(port, GPIO.OUT)
        GPIO.output(port,GPIO.LOW)

    def act(self, client_address, state, name):
        print "State", state, "on ", name, "from client @", client_address, "gpio port: ",gpio_ports[str(name)]
        self.trigger(gpio_ports[str(name)],state)
        return True

if __name__ == "__main__":
    # Startup the fauxmo server
    fauxmo.DEBUG = True
    print "name of program", __name__
    p = fauxmo.poller()
    u = fauxmo.upnp_broadcast_responder()
    u.init_socket()
    p.add(u)

    # Register the device callback as a fauxmo handler
    d = device_handler()
    for trig, port in d.TRIGGERS.items():
        fauxmo.fauxmo(trig, u, p, None, port, d)

    # Loop and poll for incoming Echo requests
    logging.debug("Entering fauxmo polling loop")
    while True:
        try:
            # Allow time for a ctrl-c to stop the process
            p.poll(100)
            time.sleep(0.1)
        except Exception, e:
            logging.critical("Critical exception: " + str(e))
            break
