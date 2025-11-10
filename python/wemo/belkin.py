import pywemo
import time

#devices = pywemo.discover_devices()
#print(devices)
#devices[0].toggle()
#time.sleep(4)

url = pywemo.setup_url_for_address("192.168.1.129")
print(url)
device = pywemo.discovery.device_from_description(url)
print(device)
device.toggle()
