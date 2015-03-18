#!/bin/bash

webdir=/root/bone
cd $webdir 
nohup $webdir/i2c &
nohup /usr/bin/node $webdir/webserver &
