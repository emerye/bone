
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <pthread.h>

 /****************************************************************
 * Constants
 ****************************************************************/

#define SYSFS_GPIO_DIR "/sys/class/gpio"
#define POLL_TIMEOUT (3 * 1000)	/* 3 seconds */
#define MAX_BUF 64

/****************************************************************
 * gpio_export
 ****************************************************************/
int gpio_export(unsigned int gpio)
{
    int fd, len;
    char buf[MAX_BUF];

    fd = open(SYSFS_GPIO_DIR "/export", O_WRONLY);
    if (fd < 0) {
	perror("gpio/export");
	return fd;
    }

    len = snprintf(buf, sizeof(buf), "%d", gpio);
    write(fd, buf, len);
    close(fd);

    return 0;
}

/****************************************************************
 * gpio_unexport
 ****************************************************************/
int gpio_unexport(unsigned int gpio)
{
    int fd, len;
    char buf[MAX_BUF];

    fd = open(SYSFS_GPIO_DIR "/unexport", O_WRONLY);
    if (fd < 0) {
	perror("gpio/export");
	return fd;
    }

    len = snprintf(buf, sizeof(buf), "%d", gpio);
    write(fd, buf, len);
    close(fd);
    return 0;
}

/****************************************************************
 * gpio_set_dir
 ****************************************************************/
int gpio_set_dir(unsigned int gpio, unsigned int out_flag)
{
    int fd;
    char buf[MAX_BUF];

    snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/direction", gpio);

    fd = open(buf, O_WRONLY);
    if (fd < 0) {
	perror("gpio/direction");
	return fd;
    }

    if (out_flag)
	write(fd, "out", 4);
    else
	write(fd, "in", 3);

    close(fd);
    return 0;
}

/****************************************************************
 * gpio_set_value
 ****************************************************************/
int gpio_set_value(unsigned int gpio, unsigned int value)
{
    int fd;
    char buf[MAX_BUF];

    snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);

    fd = open(buf, O_WRONLY);
    if (fd < 0) {
	perror("gpio/set-value");
	return fd;
    }

    if (value)
	write(fd, "1", 2);
    else
	write(fd, "0", 2);

    close(fd);
    return 0;
}

/****************************************************************
 * gpio_get_value
 ****************************************************************/
int gpio_get_value(unsigned int gpio, unsigned int *value)
{
    int fd;
    char buf[MAX_BUF];
    char ch;

    snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);


    fd = open(buf, O_RDONLY);
    if (fd < 0) {
	perror("gpio/get-value");
	return fd;
    }

    read(fd, &ch, 1);

    if (ch != '0') {
	*value = 1;
    } else {
	*value = 0;
    }

    close(fd);
    return 0;
}


/****************************************************************
 * gpio_set_edge
 ****************************************************************/

int gpio_set_edge(unsigned int gpio, char *edge)
{
    int fd;
    char buf[MAX_BUF];

    snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/edge", gpio);

    fd = open(buf, O_WRONLY);
    if (fd < 0) {
	perror("gpio/set-edge");
	return fd;
    }

    write(fd, edge, strlen(edge) + 1);
    close(fd);
    return 0;
}

/****************************************************************
 * gpio_fd_open
 ****************************************************************/

int gpio_fd_open(unsigned int gpio)
{
    int fd;
    char buf[MAX_BUF];

    snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);

    fd = open(buf, O_RDONLY | O_NONBLOCK);
    if (fd < 0) {
	perror("gpio/fd_open");
    }
    return fd;
}

/****************************************************************
 * gpio_fd_close
 ****************************************************************/

int gpio_fd_close(int fd)
{
    return close(fd);
}

// This is the thread function that will execute when the thread is created
//  it passes and receives data by void pointers
void *threadFunction(void *value)
{
    int *x = (int *) value;	//cast the data passed to an int pointer
    while (*x < 100) {		//while the value of x is less than 5
	usleep(1000);		//sleep for 1ms - encourage main thread
	(*x)++;			//increment the value of x by 1
	printf("Thread count %d\n", *x);
    }
    puts(" Thread completed.\n");
    return x;			//return the pointer x (as a void*)
}




/****************************************************************
 * Main
 ****************************************************************/
int main(int argc, char **argv, char **envp)
{
    struct pollfd;
    int gpio_fd;
    unsigned int gpio;

    if (argc < 2) {
	printf(" Usage:gpio - int <gpio - pin > \n\n ");
	printf
	    (" Waits for a change in the GPIO pin voltage level or input on stdin \n ");
	exit(-1);
    }

    gpio = atoi(argv[1]);

    gpio_export(gpio);
    gpio_set_dir(gpio, 1);
 //   gpio_set_edge(gpio, "both");
//    gpio_fd = gpio_fd_open(gpio);

    while(1) {
    gpio_set_value(gpio, 1); 
    gpio_set_value(gpio, 0); 
    }
    
    gpio_fd_close(gpio_fd);
    return 0;
}
