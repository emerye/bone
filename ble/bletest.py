from adafruit_ble import BLERadio
from adafruit_ble.advertising.standard import ProvideServicesAdvertisement
from adafruit_ble.services.standard.device_info import DeviceInfoService
from adafruit_ble_cycling_speed_and_cadence import CyclingSpeedAndCadenceService
import time

radio = BLERadio()
print("scanning")
found = set()
for entry in radio.start_scan(timeout=10, minimum_rssi=-70):
    addr = entry.address
    if addr not in found:
        print(entry)
    found.add(addr)

print("scan done")

advs = {}
for adv in radio.start_scan(ProvideServicesAdvertisement, timeout=5):
    if CyclingSpeedAndCadenceService in adv.services:
        print("found a CyclingSpeedAndCadenceService advertisement")
        # Save advertisement. Overwrite duplicates from same address (device).
        advs[adv.address] = adv

radio.stop_scan()
print("Done")


# Connect to all available CSC sensors.
cyc_connections = []
for adv in advs.values():
    cyc_connections.append(radio.connect(adv))
    print("Connected", len(cyc_connections))

# Print out info about each sensors.
for conn in cyc_connections:
    if conn.connected:
        if DeviceInfoService in conn:
            dis = conn[DeviceInfoService]
            try:
                manufacturer = dis.manufacturer
            except AttributeError:
                manufacturer = "(Manufacturer Not specified)"
            print("Device:", manufacturer)
        else:
            print("No device information")

print("Waiting for data... (could be 10-20 seconds or more)")
# Get CSC Service from each sensor.
cyc_services = []
for conn in cyc_connections:
    cyc_services.append(conn[CyclingSpeedAndCadenceService])
    # Read data from each sensor once a second.
    # Stop if we lose connection to all sensors.

while True:
    still_connected = False
    wheel_revs = None
    crank_revs = None
    crank_event_time = None
    for conn, svc in zip(cyc_connections, cyc_services):
        if conn.connected:
            still_connected = True
            values = svc.measurement_values
            if values is not None:  # add this
                if values.cumulative_wheel_revolutions:
                    wheel_revs = values.cumulative_wheel_revolutions
                if values.cumulative_crank_revolutions:
                    crank_revs = values.cumulative_crank_revolutions
                if values.last_crank_event_time:
                    crank_event_time = values.last_crank_event_time


    if not still_connected:
        break
    if wheel_revs:   # might still be None
        print(wheel_revs)
#        clue_data[0].text = "Wheel: {0:d}".format(wheel_revs)
#        clue_data.show()
    if crank_revs:
        print(crank_revs)
#        clue_data[2].text = "Crank: {0:d}".format(crank_revs)
#       clue_data.show()
    if crank_event_time:
        print("Crank event time:", crank_event_time)
    time.sleep(0.1)


