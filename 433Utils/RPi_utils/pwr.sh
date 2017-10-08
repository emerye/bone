#!/bin/bash

device=$1
operation=$2

case "$device" in 
"1") 
  if [ $operation = "on" ]; then 
    ./codesend 1381683 1 150
    echo "Channel one on" 
  else 
    ./codesend 1381692 1 150
    echo "Channel one off" 
  fi
;;
"2") 
  if [ $operation = "on" ]; then 
    ./codesend 1381827 1 150
    echo "Channel two on" 
  else 
    ./codesend 1381836 1 150
    echo "Channel two off" 
  fi
;;
"3") 
  if [ $operation = "on" ]; then 
    ./codesend 1382147 1 150
    echo "Channel three  on" 
  else 
    ./codesend 1382156 1 150
    echo "Channel three off" 
  fi
;;
"4") 
  if [ $operation = "on" ]; then 
    ./codesend 1383683 1 150
    echo "Channel four on" 
  else 
    ./codesend 1383692 1 150
    echo "Channel four off" 
  fi
;;
esac    

#Channel 
#./codesend 1381683 1 150

#Channel 1 off
#./codesend 1381692 1 150


#Channel 3 On
#./codesend 1382147 1 150 
#Channel 3 Off
#./codesend 1382156 1 150 


