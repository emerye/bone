#!/usr/bin/env python3

import serial
import sys
from datetime import datetime

# Configuration
SERIAL_PORT = "/dev/ttyUSB0"
BAUD_RATE = 115200
OUTPUT_FILE = "serial_log.txt"


def timestamp():
    return datetime.now().strftime("%Y-%m-%d %H:%M:%S")

def unixtime():
    now = datetime.now()
    return int(now.timestamp())

def main():
    try:
        # Open serial port
        ser = serial.Serial(
            port=SERIAL_PORT,
            baudrate=BAUD_RATE,
            timeout=1,
            dsrdtr=False,   # Don't use hardware DSR/DTR flow control
            rtscts=False
        )

        # Force DTR true
        ser.dtr = True

        print(f"Opened {SERIAL_PORT} at {BAUD_RATE} baud")
        print(f"DTR state: {ser.dtr}")
        print(f"Logging to: {OUTPUT_FILE}")

        with open(OUTPUT_FILE, "a", buffering=1) as logfile:
            logfile.write(
                f"\n--- Logging started {datetime.now()} ---\n"
            )

            while True:
                try:
                    line = ser.readline()

                    if line:
                        text = line.decode("utf-8", errors="replace").rstrip()
                        ts_line = f"{timestamp()} {unixtime()} {text}" 

                        print (ts_line )
                        logfile.write(ts_line + "\n")

                except KeyboardInterrupt:
                    print("\nStopping.")
                    break

    except serial.SerialException as e:
        print(f"Serial error: {e}")
        sys.exit(1)

    finally:
        try:
            ser.close()
        except:
            pass

if __name__ == "__main__":
    main()
