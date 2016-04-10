#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "itimer.h"


//Interval Timer Handler
void timer_handler(int signum)
{
    static int count = 0;
    int cumcount = 0;
    FILE *fp;

    if ((fp = fopen("./cumcount", "r+")) == NULL) {
	puts("Error opening file\n");
	return;
    }

    int retval = fread((void *) &cumcount, 4, 1, fp);
    if (retval < 0) {
	printf("read error %d\n", errno);
    }
    cumcount++;
    fseek(fp, 0, SEEK_SET);

    fwrite(&cumcount, 4, 1, fp);
    printf("Elasped: %d Cumcount: %d\n", ++count, cumcount);
    fclose(fp);
}



void startIntervalTimer(int seconds)
{

    /* Install timer_handler as the signal handler for SIGVTALRM. */
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &timer_handler;
    sigaction(SIGVTALRM, &sa, NULL);

    /* Configure the timer to expire after sec... */
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = 100000;
    /* ... and every sec after that. */
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 100000;
    /* Start a virtual timer. It counts down whenever this process is
       executing. */
    setitimer(ITIMER_VIRTUAL, &timer, NULL);

}

