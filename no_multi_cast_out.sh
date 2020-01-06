ifconfig
echo START config
ifconfig lo multicast
ifconfig wlp1s0 -multicast
echo END config
ifconfig
