#! /bin/bash

mosquitto_pub -h localhost -d -t /test -m "Hello" --keepalive 600 -q 1
