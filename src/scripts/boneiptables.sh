#!/bin/bash

SERVER_IP='99.22.55.32'

iptables -F
iptables -X

#iptables -P INPUT DROP
#iptables -P OUTPUT DROP
iptables -P FORWARD DROP

iptables -A INPUT -i lo -j ACCEPT
iptables -A OUTPUT -o lo -j ACCEPT

iptables -A INPUT -p tcp -s 0/0 -d $SERVER_IP --sport 513:65535 --dport 22 -m state --state NEW,ESTABLISHED -j ACCEPT
iptables -A OUTPUT -p tcp -s $SERVER_IP -d 0/0 --sport 22 --dport 513:65535 -m state --state ESTABLISHED -j ACCEPT 

iptables -A INPUT -p tcp --dport 3000 -j DROP
iptables -A INPUT -p tcp --dport 80 -j DROP
#iptables -A OUTPUT -j DROP
