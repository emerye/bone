#!/bin/bash

mosquitto_pub -d -u mqtt -P Mosq -t hello/world -m "Hello from Terminal window 2!"
