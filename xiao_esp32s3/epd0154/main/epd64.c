#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_log.h"

// Include your custom external 96px font array dataset
#include "font_48x96.h"

static const char *TAG = "EPD_CLOCK";

// XIAO ESP32S3 Pin Definitions for EPD_0154-2
#define EPD_MOSI_PIN     9
#define EPD_SCLK_PIN     7
#define EPD_CS_PIN       3
#define EPD_DC_PIN       4
#define EPD_RST_PIN      5
#define EPD_BUSY_PIN     6


// Display metrics 
#define EPD_WIDTH        200
#define EPD_HEIGHT       200
#define BUFFER_SIZE      ((EPD_WIDTH * EPD_HEIGHT) / 8)


static spi_device_handle_t spi_handle;
static uint8_t image_buffer[BUFFER_SIZE];


// --- EPD Low-Level Base SPI Protocol Driver Handling ---
void epd_write_command(uint8_t command) {
    gpio_set_level(EPD_DC_PIN, 0);
    spi_transaction_t t = { .length = 8, .tx_buffer = &command };
    spi_device_polling_transmit(spi_handle, &t);
}

void epd_write_data(uint8_t data) {
    gpio_set_level(EPD_DC_PIN, 1);
    spi_transaction_t t = { .length = 8, .tx_buffer = &data };
    spi_device_polling_transmit(spi_handle, &t);
}

void epd_wait_busy(void) {
    while (gpio_get_level(EPD_BUSY_PIN) == 1) {
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void epd_init(void) {
    gpio_set_level(EPD_RST_PIN, 0); vTaskDelay(pdMS_TO_TICKS(20));
    gpio_set_level(EPD_RST_PIN, 1); vTaskDelay(pdMS_TO_TICKS(20));
    epd_wait_busy();

    // SW Reset
    epd_write_command(0x12);
    epd_wait_busy();

    // Height control setup
    epd_write_command(0x01);
    epd_write_data((EPD_HEIGHT - 1) & 0xFF);
    epd_write_data(((EPD_HEIGHT - 1) >> 8) & 0xFF);
    epd_write_data(0x00);

    // Entry options mode
    epd_write_command(0x11);
    epd_write_data(0x03);

    // RAM Width Limits
    epd_write_command(0x44);
    epd_write_data(0x00);
    epd_write_data((EPD_WIDTH / 8) - 1);

    // RAM Height Limits
    epd_write_command(0x45);
    epd_write_data(0x00); epd_write_data(0x00);
    epd_write_data((EPD_HEIGHT - 1) & 0xFF);
    epd_write_data(((EPD_HEIGHT - 1) >> 8) & 0xFF);

    // Border Waveform Setup
    epd_write_command(0x3C); epd_write_data(0x05);
    epd_write_command(0x22); epd_write_data(0x91);
    epd_write_command(0x20); epd_wait_busy();
}

void epd_display_frame(const uint8_t *buffer) {
    epd_write_command(0x4E); epd_write_data(0x00);
    epd_write_command(0x4F); epd_write_data(0x00); epd_write_data(0x00);

    epd_write_command(0x24);
    for (int i = 0; i < BUFFER_SIZE; i++) { epd_write_data(buffer[i]); }

    // Refresh flag
    epd_write_command(0x22); epd_write_data(0xF7);
    epd_write_command(0x20); epd_wait_busy();
}

void epd_sleep(void) {
    epd_write_command(0x10); epd_write_data(0x01);
}

void draw_pixel(int x, int y, uint8_t color) {
    if (x < 0 || x >= EPD_WIDTH || y < 0 || y >= EPD_HEIGHT) return;
    int index = (x / 8) + (y * (EPD_WIDTH / 8));
    uint8_t bit_mask = (0x80 >> (x % 8));
    if (color == 0) image_buffer[index] &= ~bit_mask;
    else            image_buffer[index] |= bit_mask;
}

// Renders a single 48x96 clock symbol from the font_bitmap_64px array
void draw_clock_char(int x, int y, char c, uint8_t color) {
    int idx = -1;

    // Map characters to array indices
    if (c >= '0' && c <= '9') {
        idx = c - '0';
    } else if (c == ':') {
        idx = 10;
    }

    if (idx == -1) return;

    for (int row = 0; row < 96; row++) {
        for (int byte_col = 0; byte_col < 6; byte_col++) {
            // Unpack 6 bytes sequentially per row (596 bytes stride per char)
            uint8_t byte_data = font_48x96_digits[idx][(row * 6) + byte_col];

            for (int bit = 0; bit < 8; bit++) {
                if (byte_data & (0x80 >> bit)) {  //MSBFirst
                    int target_x = x + (byte_col * 8) + bit;
                    int target_y = y + row;
                    draw_pixel(target_x, target_y, color);
                }
            }
        }
    }
}


// Loops string characters across screen matrices
void draw_clock_string(int x, int y, const char *str, uint8_t color) {
    while (*str) {
        draw_clock_char(x, y, *str, color);
        // Step forward by exactly 32 horizontal pixels for the next element
        x += 48; 
        str++;
    }
}

void app_main(void) {
    ESP_LOGI(TAG, "Initializing System Bus Peripherals...");

    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << EPD_DC_PIN) | (1ULL << EPD_RST_PIN),
        .mode = GPIO_MODE_OUTPUT,
    };
    gpio_config(&io_conf);

    gpio_config_t input_conf = {
        .pin_bit_mask = (1ULL << EPD_BUSY_PIN),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
    };
    gpio_config(&input_conf);

    spi_bus_config_t buscfg = {
        .mosi_io_num = EPD_MOSI_PIN, .miso_io_num = -1, .sclk_io_num = EPD_SCLK_PIN,
        .quadwp_io_num = -1, .quadhd_io_num = -1, .max_transfer_sz = BUFFER_SIZE
    };
    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = 4 * 1000 * 1000, .mode = 0,
        .spics_io_num = EPD_CS_PIN, .queue_size = 1, .flags = SPI_DEVICE_HALFDUPLEX
    };
    ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO));
    ESP_ERROR_CHECK(spi_bus_add_device(SPI2_HOST, &devcfg, &spi_handle));

    // Clear memory canvas
    memset(image_buffer, 0xFF, sizeof(image_buffer));

    // Placed at X=20 to center the 160px wide string inside the 200px panel
    // draw_clock_string(20, 68, "12:34", 0);
    draw_clock_string(0, 0, "0123", 0);
    draw_clock_string(0, 100, "5678", 0);
    epd_init();
    ESP_LOGI(TAG, "Pushing clock frame payload...");
    epd_display_frame(image_buffer);

    ESP_LOGI(TAG, "Finished cycle operation and sending the display to sleep mode.");
    epd_sleep();
    
}
