#!/usr/bin/env python3

from adafruit_ble import BLERadio
from adafruit_ble.advertising.standard import ProvideServicesAdvertisement
from adafruit_ble.services.standard.device_info import DeviceInfoService
from adafruit_ble_cycling_speed_and_cadence import CyclingSpeedAndCadenceService
import time


radio = BLERadio()
advs = {}
print("scanning")
for adv in radio.start_scan(ProvideServicesAdvertisement, timeout=10, minimum_rssi=-65):
    if CyclingSpeedAndCadenceService in adv.services:
        print("found a CyclingSpeedAndCadenceService advertisement")
        # Save advertisement. Overwrite duplicates from same address (device).
        advs[adv.address] = adv
        print("Scan address: ", adv.address)
        if adv.address == "c5:6a:43:c8:99:6b":
            break
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
    cuurentCrankRevs = None
    currentCrankEventTime = None

lastCrankTime = 0
lastCrankRevs = 0
diffCrankTime = 0
diffCrankRevs = 0

while True:
    still_connected = False
    wheel_revs = None
    crank_revs = None
    crank_event_time = None
    crankRPM = None
  
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
    if crank_revs and crank_event_time:
        diffCrankRevs = crank_revs - lastCrankRevs
        if diffCrankRevs < 0:
            diffCrankRevs = crank_revs + (65535 - lastCrankRevs)
        lastCrankRevs = crank_revs

        diffCrankTime = crank_event_time - lastCrankTime
        if diffCrankTime < 0:
            diffCrankTime = crank_event_time + (65535 - lastCrankTime)
        lastCrankTime = crank_event_time
        if diffCrankTime == 0:
            crankRPM = 0
        else:
            crankRPM = (((diffCrankRevs / diffCrankTime) * 1024.0) * 60)
    

       # print("DiffTime %d  DiifRevs %d" % (diffCrankTime, diffCrankRevs))
        if crankRPM != 0:
            print("RPM %d  EventTime %d  CrankRev %d" % (crankRPM, crank_event_time, crank_revs))
        
    time.sleep(2)
   


