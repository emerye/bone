#!/bin/bash

device=$1
operation=$2
length=160
protocol=1

CODESEND='/home/andy/bone/433Utils/RPi_utils/codesend'; 

function xmit {
  echo $1 $protocol $length  
  $CODESEND $1 $protocol $length
  sleep 0.1
  $CODESEND $1 $protocol $length
} 


case "$device" in 
"1") 
  if [ $operation = "on" ]; then 
    #317
    xmit 1381683  
#    $CODESEND 1381683 1 $length 
    echo "Channel One On" 
  else 
    xmit 1381692 
    echo "Channel One Off" 
  fi
;;
"2") 
  if [ $operation = "on" ]; then 
    xmit 1381827 
    echo "Channel Two On" 
  else 
    xmit 1381836
    echo "Channel Two Off" 
  fi
;;
"3") 
  if [ $operation = "on" ]; then 
    xmit 1382147 
    echo "Channel Three On" 
  else 
    xmit 1382156
    $CODESEND 1382156 1 $length
    echo "Channel Three Off" 
  fi
;;
"4") 
  if [ $operation = "on" ]; then 
    xmit 1383683 
    echo "Channel Four On" 
  else 
    xmit 1383692 
    echo "Channel Four Off" 
  fi
;;
esac    


