#!/bin/bash

# shell script for daily backup  
/usr/bin/rsync -a --delete /cygdrive/c/users/andy/Documents /cygdrive/c/business /cygdrive/c/emery /cygdrive/f/dailybackup 
