#include <stdio.h>
#include <time.h>
#include <signal.h>

timer_t gTimerid;
int count = 0;

void start_timer(void)
{

struct itimerspec value;

value.it_value.tv_sec = 5;//waits for 5 seconds before sending timer signal
value.it_value.tv_nsec = 0;

value.it_interval.tv_sec = 5;//sends timer signal every 5 seconds
value.it_interval.tv_nsec = 0;

timer_create (CLOCK_REALTIME, NULL, &gTimerid);

timer_settime (gTimerid, 0, &value, NULL);

}

void stop_timer(void)
{

struct itimerspec value;

value.it_value.tv_sec = 0;
value.it_value.tv_nsec = 0;

value.it_interval.tv_sec = 0;
value.it_interval.tv_nsec = 0;

timer_settime (gTimerid, 0, &value, NULL);

}

void timer_callback(int sig) {

count += 1;
printf("Catch timer: %d  Count: %d\n", sig, count);

}

int main(int ac, char **av)
{
(void) signal(SIGALRM, timer_callback);
start_timer();
while(1);
}
