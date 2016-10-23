#!/bin/bash

# GPIO header pin 40

gpio mode 29 out  

for ((i=0; i<=2000; i++)); do
#  echo "$i"
  gpio write 29 0
  gpio write 29 1
done

