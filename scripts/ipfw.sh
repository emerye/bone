#!/bin/bash

iptables -t nat -A PREROUTING -p tcp --dport 8000 -j DNAT --to-destination 192.168.1.112:8000

iptables -t nat -A POSTROUTING -p tcp -d 192.168.1.112 --dport 8000 -j SNAT --to-source 99.22.55.32  

iptables -t nat -L -n
