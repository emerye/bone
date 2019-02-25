/* Deep sleep 

   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "sys/time.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_deep_sleep.h"

/* Can run 'make menuconfig' to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/
#define BLINK_GPIO CONFIG_BLINK_GPIO
#define GPIO_DEEP_SLEEP_DURATION      10

RTC_DATA_ATTR static time_t last;	//remember last boot in RTC Memory``

void app_main()
{
struct timeval now;

printf("start ESP32\n");
printf("deep sleep (%lds since last reset, %lds since last boot)\n", now.tv_sec, now.tv_sec-last); 

last = now.tv_sec;
esp_deep_sleep(1000000LL * GPIO_DEEP_SLEEP_DURATION);
//esp_deep_sleep_start(); 


}
