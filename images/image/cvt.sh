#!/bin/bash

FILES=`ls *.jpg`

for file in $FILES; do
  name=$(echo $file | cut -f 1 -d '.')
  echo $name 
  convert $name.jpg -resize '800x480' -depth 8 rgb:- | ./rgbto565 >$name.rgb  
done

#convert $1.jpg -resize '800x480' -depth 8 rgb:- | ./rgbto565 >$1.rgb  

