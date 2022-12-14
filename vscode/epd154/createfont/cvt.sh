#!/bin/bash
#convert -density 100 -font FreeSans -size 72x96  label:"1" 1.xbm 
#display 1.xbm



for ((i=0; i<10; i++)) 
do	
  convert -threshold 80 -font FreeSans -pointsize 158 label:$i $i.xbm
done

convert -threshold 80 -font FreeSans -pointsize 158 label:"0123456789" all.xbm
