
SERVER_IP='99.22.55.32'
WEMO_IP='192.168.1.142'

/sbin/iptables -F

iptables -P INPUT DROP 
iptables -P OUTPUT ACCEPT
#iptables -P FORWARD ACCEPT

iptables -A INPUT -i lo -j ACCEPT
iptables -A OUTPUT -o lo -j ACCEPT

#Git
iptables -A OUTPUT -p tcp --dport 9418 -j ACCEPT
iptables -A OUTPUT -p tcp --dport 9418 -j ACCEPT
iptables -A INPUT -p tcp --sport 443 -j ACCEPT
iptables -A INPUT -p tcp --sport 443 -j ACCEPT
iptables -A INPUT -p tcp --sport 80 -j ACCEPT
iptables -A INPUT -p tcp --sport 80 -j ACCEPT

#Web
iptables -A INPUT -p tcp --dport 2000 -m state --state NEW,ESTABLISHED -j ACCEPT
iptables -A INPUT -i eth0 -p tcp --dport 2000 -m state --state NEW,ESTABLISHED -j ACCEPT
iptables -A OUTPUT -o eth0 -p tcp --sport 2000 -m state --state ESTABLISHED -j ACCEPT

#Time
iptables -A OUTPUT -p udp --dport 123 -j ACCEPT
iptables -A INPUT -p udp --sport 123 -j ACCEPT

#IP Forwarding
echo 1 >| /proc/sys/net/ipv4/ip_forward
#iptables -A INPUT -p tcp --sport 1883 -j ACCEPT
iptables -t nat -A PREROUTING -p tcp --dport 1883 -j DNAT --to-destination  $WEMO_IP
iptables -t nat -A POSTROUTING -j MASQUERADE 
#iptables -t nat -A POSTROUTING -p tcp -d $WEMO_IP --dport 1883 -j SNAT --to-source $SERVER_IP:1883 

#DNS
iptables -A INPUT -p udp --sport 53 -j ACCEPT
iptables -A INPUT -p tcp --sport 53 -j ACCEPT
iptables -A OUTPUT -p udp --dport 53 -j ACCEPT
iptables -A OUTPUT -p tcp --dport 53 -j ACCEPT

#  "Allowing DNS lookups (tcp, udp port 53) to server '$ip'"
iptables -A OUTPUT -p udp -d $SERVER_IP --dport 53 -m state --state NEW,ESTABLISHED -j ACCEPT
iptables -A INPUT  -p udp -s $SERVER_IP --sport 53 -m state --state ESTABLISHED     -j ACCEPT
iptables -A OUTPUT -p tcp -d $SERVER_IP --dport 53 -m state --state NEW,ESTABLISHED -j ACCEPT
iptables -A INPUT  -p tcp -s $SERVER_IP --sport 53 -m state --state ESTABLISHED     -j ACCEPT

#SSH
iptables -A INPUT -p tcp -s 0/0 -d $SERVER_IP --sport 513:65535 --dport 22 -m state --state NEW,ESTABLISHED -j ACCEPT
iptables -A OUTPUT -p tcp -s $SERVER_IP -d 0/0 --sport 22 --dport 513:65535 -m state --state ESTABLISHED -j ACCEPT 


iptables -P INPUT DROP
#iptables -P FORWARD DROP
