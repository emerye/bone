#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


//Interval Timer Handler
void timer_handler(int signum)
{
    static int count = 0;
    int cumcount = 0;
    unsigned char buf[100];
    FILE *fp;

    if ((fp = fopen("./cumcount.txt", "w+")) == NULL) {
	puts("Error opening file cumcount.txt\n");
	return;
    }

    int retval = fread((void *) &cumcount, 4, 1, fp);
    if (retval < 0) {
	printf("read error %d\n", errno);
    }
    cumcount++;
    fseek(fp, 0, SEEK_SET);

    fwrite(&cumcount, 4, 1, fp);
    fclose(fp); 

    printf("Elasped: %d Cumcount: %d\n", ++count, cumcount);

}


int main()
{
    struct sigaction sa;
    struct itimerval timer;

    /* Install timer_handler as the signal handler for SIGVTALRM. */
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &timer_handler;
    sigaction(SIGVTALRM, &sa, NULL);

    /* Configure the timer to expire after 250 msec... */
    timer.it_value.tv_sec = 1;
    timer.it_value.tv_usec = 0;
    /* ... and every 250 msec after that. */
    timer.it_interval.tv_sec = 1;
    timer.it_interval.tv_usec = 0;
    /* Start a virtual timer. It counts down whenever this process is
       executing. */
    setitimer(ITIMER_VIRTUAL, &timer, NULL);

    /* Do busy work. */
    while(1); 
}
