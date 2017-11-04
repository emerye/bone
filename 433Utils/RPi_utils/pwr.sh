#!/bin/bash

#set -x
set -eu

device=$1
operation=$2
length=160
protocol=1
channel=317

CODESEND='/home/andy/bone/433Utils/RPi_utils/codesend'; 

function xmit {
  $CODESEND $1 $protocol $length
  sleep 0.1
  $CODESEND $1 $protocol $length
} 

#Channel 317
case "$device" in 
"1") 
  if [ $operation = "on" ]; then 
    #317
    xmit 1381683  
#    $CODESEND 1381683 1 $length 
    echo "Channel 317 Socket One On" 
  else 
    xmit 1381692 
    echo "Channel 317 Socket One Off" 
  fi
;;
"2") 
  if [ $operation = "on" ]; then 
    xmit 1381827 
    echo "Channel 317 Socket Two On" 
  else 
    xmit 1381836
    echo "Channel 317 Socket Two Off" 
  fi
;;
"3") 
  if [ $operation = "on" ]; then 
    xmit 1382147 
    echo "Channel 317 Socket Three On" 
  else 
    xmit 1382156
    echo "Channel 317 Socket Three Off" 
  fi
;;
"4") 
  if [ $operation = "on" ]; then 
    xmit 1383683 
    echo "Channel 317 Socket Four On" 
  else 
    xmit 1383692 
    echo "Channel 317 Socket Four Off" 
  fi
;;
"5") 
  if [ $operation = "on" ]; then 
    xmit 1389827 
    echo "Channel 317 Socket All On" 
  else 
    xmit 1389836
    echo "Channel 317 Socket All Off" 
  fi
;;

#Channel 314
"6") 
  if [ $operation = "on" ]; then 
    #317
    xmit 4216115  
#    $CODESEND 1381683 1 $length 
    echo "Channel 314 Socket One On" 
  else 
    xmit 4216124 
    echo "Channel 314 Socket One Off" 
  fi
;;
"7") 
  if [ $operation = "on" ]; then 
    xmit 4216259 
    echo "Channel 314 Socket Two On" 
  else 
    xmit 4216268 
    echo "Channel 314 Socket Two Off" 
  fi
;;
"8") 
  if [ $operation = "on" ]; then 
    xmit 4216579 
    echo "Channel 314 Socket Three On" 
  else 
    xmit 4216588 
    echo "Channel 314 Socket Three Off" 
  fi
;;
"9") 
  if [ $operation = "on" ]; then 
    xmit 4218115 
    echo "Channel 314 Socket Four On" 
  else 
    xmit 4218124 
    echo "Channel 314 Socket Four Off" 
  fi
;;
"10") 
  if [ $operation = "on" ]; then 
    xmit 4224259 
    echo "Channel 314 Socket All On" 
  else 
    xmit 4224268 
    echo "Channel 314 Socket All Off" 
  fi
;;
esac    

