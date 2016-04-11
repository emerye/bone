#ifndef _ITIMER_H
#define _ITIMER_H

#include <stdio.h>
#include <sys/time.h>
#include <signal.h>

extern unsigned int count; 

extern struct sigaction sa; 
extern struct itimerval timer; 

void startIntervalTimer(int seconds);  

#endif
