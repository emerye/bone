#! /bin/bash

mosquitto_pub -h rbackup -p 1883 -d -u andy -P andy -t /test -m "Hello" --keepalive 600 -q 1
