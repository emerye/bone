#!/bin/bash

# shell script for daily backup  

/usr/bin/rsync -av --delete /cygdrive/c/users/andy/Documents /cygdrive/c/business /cygdrive/c/emery 'andy@rbackup:/backup' 
