#include <stdio.h>
#include <sys/timerfd.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/timerfd.h>
#include <unistd.h>
#include <fcntl.h>
#include <wiringPi.h>

#define TIMEOUTSEC 0
#define TIMEOUTNSEC 1960000
#define POLL_PERIOD 1

struct itimerspec timeout;
int fd = -1;

//Set a timeout and return when expireed
int setTimeout(long duration)
{
	unsigned long long missed = 0;
	int ret;

	timeout.it_value.tv_nsec = duration;
	ret = timerfd_settime(fd, 0, &timeout, NULL);
	if (ret) {
		printf("Failed to set timer duration in send0.\n");
		return 1;
	}
	ret = read(fd, &missed, sizeof(missed));
	if(ret != 8) {
		printf("Interrupt missed: %d\n", ret);
	}
	return 0;
}

void send0(void) {
	digitalWrite(29, HIGH);
	setTimeout(TIMEOUTNSEC);
	digitalWrite(29, LOW);
	setTimeout(TIMEOUTNSEC * 3);
	digitalWrite(29, HIGH);
	setTimeout(TIMEOUTNSEC);
	digitalWrite(29, LOW);
	setTimeout(TIMEOUTNSEC * 3);
}

void send1(void) {
	digitalWrite(29, HIGH);
	setTimeout(TIMEOUTNSEC * 3);
	digitalWrite(29, LOW);
	setTimeout(TIMEOUTNSEC);
	digitalWrite(29, HIGH);
	setTimeout(TIMEOUTNSEC * 3);
	digitalWrite(29, LOW);
	setTimeout(TIMEOUTNSEC);
}

void sendF(void) {
	digitalWrite(29, HIGH);
	setTimeout(TIMEOUTNSEC);
	digitalWrite(29, LOW);
	setTimeout(TIMEOUTNSEC * 3);
	digitalWrite(29, HIGH);
	setTimeout(TIMEOUTNSEC * 3);
	digitalWrite(29, LOW);
	setTimeout(TIMEOUTNSEC);
}

void sendSync() {
	digitalWrite(29, HIGH);
	setTimeout(TIMEOUTNSEC);
	digitalWrite(29, LOW);
	setTimeout(TIMEOUTNSEC * 23);
}


int main(int argc, char *argv[]) {
	int ret;
	unsigned long long missed= 0;
	int count = 0;
	int currentState = 0;

	wiringPiSetup();
	pinMode(29, OUTPUT);
	digitalWrite(29, LOW);

	/* create new timer */
	fd = timerfd_create(CLOCK_MONOTONIC, 0);
	if (fd <= 0) {
		printf("Failed to create timer\n");
		return 1;
	}
	/* set to non-blocking */
	//   ret = fcntl(fd, F_SETFL, O_NONBLOCK);
	/*
	 ret = fcntl(fd, F_SETFL);
	 if (ret) {
	 printf("Failed to set to non blocking mode\n");
	 return 1;
	 }
	 */

	/* set timeout */
	timeout.it_value.tv_sec = TIMEOUTSEC;
	timeout.it_value.tv_nsec = TIMEOUTNSEC;

	timeout.it_interval.tv_sec = TIMEOUTSEC; /* recurring */
	timeout.it_interval.tv_nsec = TIMEOUTNSEC;
	timeout.it_interval.tv_sec = 0;
	timeout.it_interval.tv_nsec = 0;
	/*
	ret = timerfd_settime(fd, 0, &timeout, NULL);
	if (ret) {
		printf("Failed to set timer duration\n");
		return 1;
	}
	*/

	send0();
	send0();
	send0();
	send0();
	send1();
	send1();
	send1();
	send1();
	sendF();
	sendF();
	sendF();
	sendF();
	sendSync();
	sendSync();
	sendSync();
	sendSync();


	return 0;

	while (count < 20) {
		//   printf("Polling\n");
		digitalWrite(29, HIGH);
		while (read(fd, &missed, sizeof(missed)) < 0) {
			missed += 1;
		}
		count++;
		currentState ^= 1;
		digitalWrite(29, currentState);
		digitalWrite(29, LOW);
		usleep(2000);
		timeout.it_value.tv_sec = TIMEOUTSEC * 3;
		timeout.it_value.tv_nsec = TIMEOUTNSEC * 3;
		ret = timerfd_settime(fd, 0, &timeout, NULL);
		if (ret) {
			printf("Failed to set timer duration\n");
			return 1;
		}
	}
	digitalWrite(29, LOW);
	printf("Count %d  Missed %d\n", count, missed);
	fflush(stdout);
	sleep(2);
	return 0;
}
