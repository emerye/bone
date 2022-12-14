#!/bin/bash
#convert -density 100 -font FreeSans -size 72x96  label:"1" 1.xbm 
#display 1.xbm



for ((i=0; i<10; i++)) 
do	
  cat $i.xbm >> font160.c
done

