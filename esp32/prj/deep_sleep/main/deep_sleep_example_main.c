/* Deep sleep wake up example */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_sleep.h"
#include "esp_log.h"
#include "esp32/ulp.h"
#include "driver/touch_pad.h"
#include "driver/adc.h"
#include "driver/rtc_io.h"
#include "soc/sens_periph.h"
#include "soc/rtc.h"

static RTC_DATA_ATTR struct timeval sleep_enter_time;

#ifdef CONFIG_ENABLE_ULP_TEMPERATURE_WAKEUP
#if CONFIG_IDF_TARGET_ESP32

/*
 * Offset (in 32-bit words) in RTC Slow memory where the data is placed
 * by the ULP coprocessor. It can be chosen to be any value greater or equal
 * to ULP program size, and less than the CONFIG_ESP32_ULP_COPROC_RESERVE_MEM/4 - 6,
 * where 6 is the number of words used by the ULP coprocessor.
 */
#define ULP_DATA_OFFSET     36

_Static_assert(ULP_DATA_OFFSET < CONFIG_ESP32_ULP_COPROC_RESERVE_MEM/4 - 6,
        "ULP_DATA_OFFSET is set too high, or CONFIG_ESP32_ULP_COPROC_RESERVE_MEM is not sufficient");


/**
 * @brief Utility function which writes data to be ready by ULP program
 *
 * @param offset offset from ULP_DATA_OFFSET in RTC Slow memory, in words
 * @param value lower 16-bit part of the word to be stored
 */
static inline void ulp_data_write(size_t offset, uint16_t value)
{
    RTC_SLOW_MEM[ULP_DATA_OFFSET + offset] = value;
}
#endif // CONFIG_IDF_TARGET_ESP32
#endif // CONFIG_ENABLE_ULP_TEMPERATURE_WAKEUP


void app_main(void)
{
    struct timeval now;
    gettimeofday(&now, NULL);
    int sleep_time_ms = (now.tv_sec - sleep_enter_time.tv_sec) * 1000 + (now.tv_usec - sleep_enter_time.tv_usec) / 1000;

    switch (esp_sleep_get_wakeup_cause()) {
        case ESP_SLEEP_WAKEUP_EXT1: {
            uint64_t wakeup_pin_mask = esp_sleep_get_ext1_wakeup_status();
            if (wakeup_pin_mask != 0) {
                int pin = __builtin_ffsll(wakeup_pin_mask) - 1;
                printf("Wake up from GPIO %d\n", pin);
            } else {
                printf("Wake up from GPIO\n");
            }
            break;
        }
        case ESP_SLEEP_WAKEUP_TIMER: {
            printf("Wake up from timer. Time spent in deep sleep: %dms\n", sleep_time_ms);
            break;
        }
        case ESP_SLEEP_WAKEUP_UNDEFINED:
        default:
            printf("Not a deep sleep reset\n");
    }

    vTaskDelay(1000 / portTICK_PERIOD_MS);

    const int wakeup_time_sec = 5;
    printf("Enabling timer wakeup, %ds\n", wakeup_time_sec);
    esp_sleep_enable_timer_wakeup(wakeup_time_sec * 1000000);

  #if CONFIG_IDF_TARGET_ESP32
    // Isolate GPIO12 pin from external circuits. This is needed for modules
    // which have an external pull-up resistor on GPIO12 (such as ESP32-WROVER)
    // to minimize current consumption.
    rtc_gpio_isolate(GPIO_NUM_12);
#endif

    printf("Entering deep sleep\n");
    gettimeofday(&sleep_enter_time, NULL);
    esp_deep_sleep_start();
} 

