#!/bin/bash

device=$1
operation=$2
length=160

CODESEND='/home/andy/bone/433Utils/RPi_utils/codesend'; 

case "$device" in 
"1") 
  if [ $operation = "on" ]; then 
    $CODESEND 1381683 1 $length 
    echo "Channel One On" 
  else 
    $CODESEND 1381692 1 $length 
    echo "Channel One Off" 
  fi
;;
"2") 
  if [ $operation = "on" ]; then 
    $CODESEND 1381827 1 $length 
    echo "Channel Two On" 
  else 
    $CODESEND 1381836 1 $length
    echo "Channel Two Off" 
  fi
;;
"3") 
  if [ $operation = "on" ]; then 
    $CODESEND 1382147 1 $length
    echo "Channel Three On" 
  else 
    $CODESEND 1382156 1 $length
    echo "Channel Three Off" 
  fi
;;
"4") 
  if [ $operation = "on" ]; then 
    $CODESEND 1383683 1 $length 
    echo "Channel Four On" 
  else 
    $CODESEND 1383692 1 $length 
    echo "Channel Four Off" 
  fi
;;
esac    


