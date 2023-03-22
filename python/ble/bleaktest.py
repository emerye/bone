import asyncio
from bleak import BleakScanner
from bleak import BleakClient
import time 
import binascii


address = "C5:6A:43:C8:99:6B"

MODEL_NBR_UUID = "00002a24-0000-1000-8000-00805f9b34fb"
HARDWARE_REVISION_UUID = "00002a27-0000-1000-8000-00805f9b34fb"
NOTIFICATION_UUID = "00002902-0000-1000-8000-00805f9b34fb"
CSC_MEASUREMENT_UUID = "00002a5b-0000-1000-8000-00805f9b34fb"
CSC_UUID = "00001816-0000-1000-8000-00805f9b34fb"
BATTERY_SERVICE_UUID = "0000180f-0000-1000-8000-00805f9b34fb"
BATTERY_LEVEL_UUID = "00002a19-0000-1000-8000-00805f9b34fb"

last_time = 0
last_crank = 1

async def callback_handler(_, data):
    global last_time
    global last_crank

    #print((time.time(), data))
    #print(binascii.hexlify(data))
    current_crank = data[0]*256 + data[1]
    timecnt = data[4]*256 + data[3]
    delta_time = timecnt - last_time
    cranks = current_crank - last_crank
    print("crank_delta", cranks, "delta_t", delta_time)
    if (delta_time > 1) and (cranks > 0):
        cranks_sec = (delta_time / 1024) / cranks 
        if cranks_sec > 0:
            cadence = 60.0 / cranks_sec
            print("Crank per sec",cranks_sec,"Cadence", cadence)

    last_time = timecnt 
    last_crank = current_crank


async def xoss():
    async with BleakClient(address) as client:
        model_number = await client.read_gatt_char(MODEL_NBR_UUID)
        hardware_rev = await client.read_gatt_char(HARDWARE_REVISION_UUID)
        my_hardware = (hardware_rev.decode('utf-8'))
        print(my_hardware)
        print("Model Number: {0}".format("".join(map(chr, model_number))))
        print("Hardware Revision: {0}".format(my_hardware))

        services = await client.get_services()

        for service in services:
            print('\nservice', service.handle, service.uuid, service.description)
            characteristics = service.characteristics
            for char in characteristics:
                print('  characteristic', char.handle, char.uuid, char.description, char.properties)
                descriptors = char.descriptors

                for desc in descriptors:
                    print('    descriptor', desc)


        battery_level = await client.read_gatt_char(20)
        print("Battery Level", battery_level)

        csc_notify = await client.read_gatt_descriptor(14)
        print("CSC Measurement Register", csc_notify) 

        await client.start_notify(12, callback_handler)
        csc_notify = await client.read_gatt_descriptor(14)
        print("CSC Measurement Register", csc_notify) 
        await asyncio.sleep(30.0)
        await client.stop_notify(12)
        # Send an "exit command to the consumer"
        print("Finished")


async def main():
    #devices = await BleakScanner.discover()
    #for d in devices:
    #    print(d)
    print('Started')



asyncio.run(main())
asyncio.run(xoss())
  
