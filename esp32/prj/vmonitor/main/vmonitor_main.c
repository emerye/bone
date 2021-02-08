/* i2c - Example

   For other examples please check:
   https://github.com/espressif/esp-idf/tree/master/examples

   See README.md file to get detailed usage of this example.

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "esp_log.h"
#include "driver/i2c.h"
#include "sdkconfig.h"
#include "ads1015.h"
#include "lcdchar.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "driver/dac.h"

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "protocol_examples_common.h"

#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

#include "esp_log.h"
#include "mqtt_client.h"

#define DEFAULT_VREF 1100 //Use adc2_vref_to_gpio() to obtain a better estimate
#define NO_OF_SAMPLES 128 //Multisampling

static const char *TAG = "vmonitor";

#define _I2C_NUMBER(num) I2C_NUM_##num
#define I2C_NUMBER(num) _I2C_NUMBER(num)

#define DATA_LENGTH 512                  /*!< Data buffer length of test buffer */
#define RW_TEST_LENGTH 128               /*!< Data length for r/w test, [0,DATA_LENGTH] */
#define DELAY_TIME_BETWEEN_ITEMS_MS 1000 /*!< delay time between different test items */

#define I2C_SLAVE_SCL_IO CONFIG_I2C_SLAVE_SCL               /*!< gpio number for i2c slave clock */
#define I2C_SLAVE_SDA_IO CONFIG_I2C_SLAVE_SDA               /*!< gpio number for i2c slave data */
#define I2C_SLAVE_NUM I2C_NUMBER(CONFIG_I2C_SLAVE_PORT_NUM) /*!< I2C port number for slave dev */
#define I2C_SLAVE_TX_BUF_LEN (2 * DATA_LENGTH)              /*!< I2C slave tx buffer size */
#define I2C_SLAVE_RX_BUF_LEN (2 * DATA_LENGTH)              /*!< I2C slave rx buffer size */

#define I2C_MASTER_SCL_IO CONFIG_I2C_MASTER_SCL               /*!< gpio number for I2C master clock */
#define I2C_MASTER_SDA_IO CONFIG_I2C_MASTER_SDA               /*!< gpio number for I2C master data  */
#define I2C_MASTER_NUM I2C_NUMBER(CONFIG_I2C_MASTER_PORT_NUM) /*!< I2C port number for master dev */
#define I2C_MASTER_FREQ_HZ CONFIG_I2C_MASTER_FREQUENCY        /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE 0                           /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE 0                           /*!< I2C master doesn't need buffer */

#define BH1750_SENSOR_ADDR CONFIG_BH1750_ADDR   /*!< slave address for BH1750 sensor */
#define ADS1015_SENSOR_ADDR CONFIG_ADS1015_ADDR /*!< slave address for ADS1015 sensor */
#define BH1750_CMD_START CONFIG_BH1750_OPMODE   /*!< Operation mode */
#define ESP_SLAVE_ADDR CONFIG_I2C_SLAVE_ADDRESS /*!< ESP32 slave address, you can set any 7bit value */
#define WRITE_BIT I2C_MASTER_WRITE              /*!< I2C master write */
#define READ_BIT I2C_MASTER_READ                /*!< I2C master read */
#define ACK_CHECK_EN 0x1                        /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS 0x0                       /*!< I2C master will not check ack from slave */
#define ACK_VAL 0x0                             /*!< I2C ack value */
#define NACK_VAL 0x1                            /*!< I2C nack value */

#define MCP3424ADDR 0x68                        /* MCP3425 I2C address */

SemaphoreHandle_t print_mux = NULL;

static esp_adc_cal_characteristics_t *adc_chars;
static const adc_channel_t channel = ADC1_CHANNEL_6;
static const adc_atten_t atten = ADC_ATTEN_DB_0;
static const adc_unit_t unit = ADC_UNIT_1;
static const int16_t VOFFSET = 0;

esp_mqtt_client_handle_t client;


static esp_err_t mqtt_event_handler_cb(esp_mqtt_event_handle_t event)
{
    client = event->client;
    int msg_id;
    // your_context_t *context = event->context;
    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            msg_id = esp_mqtt_client_publish(client, "/topic/qos1", "data_3", 0, 1, 0);
            ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);

            msg_id = esp_mqtt_client_subscribe(client, "/topic/qos0", 0);
            ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

            msg_id = esp_mqtt_client_subscribe(client, "/topic/qos1", 1);
            ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

            msg_id = esp_mqtt_client_unsubscribe(client, "/topic/qos1");
            ESP_LOGI(TAG, "sent unsubscribe successful, msg_id=%d", msg_id);
            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
            break;

        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            msg_id = esp_mqtt_client_publish(client, "/topic/qos0", "data", 0, 0, 0);
            ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
            break;
        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_PUBLISHED:
         //   ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "MQTT_EVENT_DATA");
            printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
            printf("DATA=%.*s\r\n", event->data_len, event->data);
            break;
        case MQTT_EVENT_ERROR:
            ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
            break;
        default:
            ESP_LOGI(TAG, "Other event id:%d", event->event_id);
            break;
    }
    return ESP_OK;
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    mqtt_event_handler_cb(event_data);
}

static void mqtt_app_start(void)
{
    esp_mqtt_client_config_t mqtt_cfg = {
        .uri = CONFIG_BROKER_URL,
        .username = "andy",
        .password = "andy",
    };
    client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    esp_mqtt_client_start(client);
}


/**
 * @brief test code to read esp-i2c-slave
 *        We need to fill the buffer of esp slave device, then master can read them out.
 *
 * _______________________________________________________________________________________
 * | start | slave_addr + rd_bit +ack | read n-1 bytes + ack | read 1 byte + nack | stop |
 * --------|--------------------------|----------------------|--------------------|------|
 *
 */
static esp_err_t i2c_master_read_slave(i2c_port_t i2c_num, uint8_t tgtAddr, uint8_t *data_rd, size_t size)
{
    if (size == 0)
    {
        return ESP_OK;
    }
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (tgtAddr << 1) | READ_BIT, ACK_CHECK_EN);
    if (size > 1)
    {
        i2c_master_read(cmd, data_rd, size - 1, ACK_VAL);
    }
    i2c_master_read_byte(cmd, data_rd + size - 1, NACK_VAL);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}

/**
 * @brief test code to read esp-i2c-slave
 *        We need to fill the buffer of esp slave device, then master can read them out.
 *
 * _______________________________________________________________________________________
 * | start | slave_addr + rd_bit +ack | read n-1 bytes + ack | read 1 byte + nack | stop |
 * --------|--------------------------|----------------------|--------------------|------|
 *
 */
static esp_err_t i2c_read_slave(i2c_port_t i2c_num, uint8_t tgtAddress, uint8_t *data_rd, size_t size)
{
    if (size == 0)
    {
        return ESP_OK;
    }
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (tgtAddress << 1) | READ_BIT, ACK_CHECK_EN);
    if (size > 1)
    {
        i2c_master_read(cmd, data_rd, size - 1, ACK_VAL);
    }
    i2c_master_read_byte(cmd, data_rd + size - 1, NACK_VAL);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}

/**
 * @brief Test code to write esp-i2c-slave
 *        Master device write data to slave(both esp32),
 *        the data will be stored in slave buffer.
 *        We can read them out from slave buffer.
 *
 * ___________________________________________________________________
 * | start | slave_addr + wr_bit + ack | write n bytes + ack  | stop |
 * --------|---------------------------|----------------------|------|
 *
 */
static esp_err_t i2c_master_write_slave(i2c_port_t i2c_num, uint8_t tgtAddr, uint8_t *data_wr, size_t size)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (tgtAddr << 1) | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write(cmd, data_wr, size, ACK_CHECK_EN);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}

esp_err_t ic2_master_write_byte(i2c_port_t i2c_num, uint8_t address, uint8_t data_wr)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (address << 1) | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, data_wr, ACK_CHECK_EN);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}

/** Read a block of data from a slave.
 * esp_err_t I2CReadBlock(i2c_port_t i2c_num, uint8_t slaveTgtAddress, uint8_t startAddress, 
 * uint8_t *readData, uint8_t numBytestoRead)
 * 
 * i2c_num is either 0 or 1
 * 
 * Returns standard errors. ESP_OK for success
 */
static esp_err_t I2CReadBlock(i2c_port_t i2c_num, uint8_t slaveTgtAddress, uint8_t startAddress,
                              uint8_t *readData, uint8_t numBytestoRead)
{
    i2c_cmd_handle_t cmd;

    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, 0x48 << 1 | I2C_MASTER_READ, ACK_CHECK_EN);
    for (int i = 0; i < numBytestoRead - 1; i++)
    {
        i2c_master_read_byte(cmd, readData++, ACK_VAL);
    }
    i2c_master_read_byte(cmd, readData, NACK_VAL);
    i2c_master_stop(cmd);
    esp_err_t status = i2c_master_cmd_begin(i2c_num, cmd, 10 / portTICK_RATE_MS);
    if (status != ESP_OK)
    {
        printf("Error reading slave. Error: %d\n", status);
    }
    i2c_cmd_link_delete(cmd);

    return (status);
}

/**
 * @brief test code to operate on ADC1015 analog to digital converter.
 *
 * 1. set operation mode(e.g One time L-resolution mode)
 * _________________________________________________________________
 * | start | slave_addr + wr_bit + ack | write 1 byte + ack  | stop |
 * --------|---------------------------|---------------------|------|
 * 2. wait more than 24 ms
 * 3. read data
 * ______________________________________________________________________________________
 * | start | slave_addr + rd_bit + ack | read 1 byte + ack  | read 1 byte + nack | stop |
 * --------|---------------------------|--------------------|--------------------|------|
 */
static void i2c_ads1015(void *i2c_num_arg)
{
    int ret;
    uint8_t data_h = 0;
    uint8_t data_l = 0;
    uint8_t cmdBuffer[64];
    uint16_t adcCfg = 0;
    i2c_cmd_handle_t cmd;
    i2c_port_t i2c_num;

    if (i2c_num_arg == 0)
    {
        i2c_num = I2C_NUM_0;
    }
    else
    {
        i2c_num = I2C_NUM_1;
    }
    cmd = i2c_cmd_link_create();
    adcCfg = ADS1015_REG_CONFIG_MUX_SINGLE_0 | ADS1015_REG_CONFIG_PGA_4_096V |
             ADS1115_REG_CONFIG_DR_16SPS;

    cmdBuffer[0] = 0x48 << 1 | I2C_MASTER_WRITE;
    cmdBuffer[1] = ADS1015_REG_POINTER_CONFIG;
    cmdBuffer[2] = adcCfg >> 8;
    cmdBuffer[3] = adcCfg & 0x00FF;
    i2c_master_start(cmd);
    i2c_master_write(cmd, cmdBuffer, 4, ACK_VAL);
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(i2c_num, cmd, 3 / portTICK_RATE_MS);
    if (ret != ESP_OK)
    {
        printf("Sending setup of i2c returned error %d\n", ret);
    }
    i2c_cmd_link_delete(cmd);

    cmd = i2c_cmd_link_create();
    cmdBuffer[0] = 0x48 << 1 | I2C_MASTER_WRITE;
    cmdBuffer[1] = ADS1015_REG_POINTER_CONVERT;
    i2c_master_start(cmd);
    i2c_master_write(cmd, cmdBuffer, 2, ACK_CHECK_EN);
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(i2c_num, cmd, 20 / portTICK_RATE_MS);
    if (ret != ESP_OK)
    {
        printf("Sending setup to ADC1015 returned error %d\n", ret);
    }
    i2c_cmd_link_delete(cmd);

    //Read
    while (1)
    {
        cmd = i2c_cmd_link_create();

        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, 0x48 << 1 | I2C_MASTER_READ, ACK_CHECK_EN);
        i2c_master_read_byte(cmd, &data_h, ACK_VAL);
        i2c_master_read_byte(cmd, &data_l, NACK_VAL);
        i2c_master_stop(cmd);
        ret = i2c_master_cmd_begin(i2c_num, cmd, 10 / portTICK_RATE_MS);
        if (ret != ESP_OK)
        {
            printf("Error reading ADS1015. Error: %d\n", ret);
        }
        printf("High 0x%02x Low 0x%02x  %.3fV\n", (unsigned int)data_h, (unsigned int)data_l,
               (double)((data_h * 256 + (data_l >> 4)) / 32768.0) * 4.096);
        i2c_cmd_link_delete(cmd);

        ret = I2CReadBlock(i2c_num, 0x48, 0, cmdBuffer, 2);
        if (ret != ESP_OK)
        {
            printf("Sending blockread returned error %d\n", ret);
        }
        printf("I2CReadBlock   High 0x%02x Low 0x%02x  %.3fV\n", (unsigned int)cmdBuffer[1], (unsigned int)cmdBuffer[0],
               (double)((data_h * 256 + (data_l >> 4)) / 32768.0) * 4.096);

        memset(cmdBuffer, 0, sizeof(cmdBuffer));
        i2c_read_slave(i2c_num, 0x48, cmdBuffer, 2);
        if (ret != ESP_OK)
        {
            printf("Sending MasterReadBlock returned error %d\n", ret);
        }
        printf("MasterReadSlave  High 0x%02x Low 0x%02x  %.3fV\n", (unsigned int)cmdBuffer[1], (unsigned int)cmdBuffer[0],
               (double)((data_h * 256 + (data_l >> 4)) / 32768.0) * 4.096);

        puts("");
        vTaskDelay(300 / portTICK_RATE_MS);
    } //End while1
}

/**
 * @brief i2c master initialization
 */
static esp_err_t i2c_master_init(void)
{
    int i2c_master_port = I2C_MASTER_NUM;

    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_MASTER_SDA_IO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = I2C_MASTER_SCL_IO;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
    i2c_param_config(i2c_master_port, &conf);
    return i2c_driver_install(i2c_master_port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
}


/*
 * @brief test function to show buffer
 */
static void disp_buf(uint8_t *buf, int len)
{
    int i;
    for (i = 0; i < len; i++)
    {
        printf("%02x ", buf[i]);
        if ((i + 1) % 16 == 0)
        {
            printf("\n");
        }
    }
    printf("\n");
}


static void initGPIOs()
{

    gpio_pad_select_gpio(GPIO_NUM_16);
    gpio_set_direction(GPIO_NUM_16, GPIO_MODE_OUTPUT);
    gpio_set_level(GPIO_NUM_16, 0);
}


static void print_char_val_type(esp_adc_cal_value_t val_type)
{
    if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP)
    {
        printf("Characterized using Two Point Value\n");
    }
    else if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF)
    {
        printf("Characterized using eFuse Vref\n");
    }
    else
    {
        printf("Characterized using Default Vref\n");
    }
}


void init_adc()
{
    //Check TP is burned into eFuse
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_TP) == ESP_OK)
    {
        printf("eFuse Two Point: Supported\n");
    }
    else
    {
        printf("eFuse Two Point: NOT supported\n");
    }

    //Check Vref is burned into eFuse
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_VREF) == ESP_OK)
    {
        printf("eFuse Vref: Supported\n");
    }
    else
    {
        printf("eFuse Vref: NOT supported\n");
    }
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(channel, atten);

    //Route vREF to ADC pin so it can be measured.
    //Adafruit Feather measured vREF is 1.115
    /*
    esp_err_t status = adc2_vref_to_gpio(GPIO_NUM_25);
    if (status == ESP_OK) {
        printf("v_ref routed to GPIO\n");
    } else {
        printf("failed to route v_ref\n");
    }
    */

    //Characterize ADC
    adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(unit, atten, ADC_WIDTH_BIT_12, DEFAULT_VREF, adc_chars);
    print_char_val_type(val_type);

    /* Init DAC */
    dac_output_enable(DAC_CHANNEL_1);       //GPIO25
  //  dac_output_voltage(DAC_CHANNEL_1, 158); //2.000V
  //  dac_output_voltage(DAC_CHANNEL_1, 158); //2.000V
    dac_output_voltage(DAC_CHANNEL_1, 118); //1.5V
}

/** Calculate temperature based on thermistor resistance
 * NTCLP100E3472H Metal pipe thermistor
 * Returns temperature in deg F
 */
double calcSteinHart(double resth)
{
    double a = 1.327532184E-3;
    double b = 2.312481108E-4;
    double c = 1.177982663E-7;

    //Using 4.7K data from datasheet
    /*
    double a = 1.304074410E-3;
    double b = 2.352070455E-4;
    double c = 1.012284959E-7;
    */
    double tKelvin, oneOverK;

    oneOverK = a + b * (log(resth)) + c * pow((log(resth)), 3);
    tKelvin = 1.0 / oneOverK;
    //printf("Rth: %f deg K: %f deg C: %.1f  deg F: %.1f\n", resth, tKelvin,
    //		tKelvin - 273.15, ((tKelvin - 273.15) * 1.8) + 32);
    return (((tKelvin - 273.15) * 1.8) + 32);
}


void mainloop(void)
{
    while (1)
    {
        char vBuffer[40];
        uint32_t adc_reading = 0;
        double resTh, degF;

        //Multisampling
        for (int i = 0; i < NO_OF_SAMPLES; i++)
        {
            if (unit == ADC_UNIT_1)
            {
                adc_reading += adc1_get_raw((adc1_channel_t)channel);
            }
            else
            {
                int raw;
                adc2_get_raw((adc2_channel_t)channel, ADC_WIDTH_BIT_12, &raw);
                adc_reading += raw;
            }
        }
        adc_reading /= NO_OF_SAMPLES;

        //Convert adc_reading to voltage in mV
        uint32_t voltage = esp_adc_cal_raw_to_voltage(adc_reading, adc_chars) - VOFFSET;
        //printf("Raw: %d\tVoltage: %dmV\n", adc_reading, voltage);
        sprintf(vBuffer, "%d mV   \n", voltage);
        WriteString(0, 0, vBuffer);

        //VRef = 2.0V  R1 = 8200
      //  double currentMa = (2.0 - ((double)voltage / 1000)) / 8200;
        double currentMa = (1.5 - ((double)voltage / 1000)) / 8200;
        resTh = voltage / currentMa / 1000;
        degF = calcSteinHart(resTh);
        printf("Thermistor Resistance %.0f  Temperature %.1f\n", resTh, degF);
        sprintf(vBuffer, "Temperature %.1f deg\n", degF);
        WriteString(1, 0, vBuffer);

        sprintf(vBuffer, "%.1f", degF);
        int msg_id = esp_mqtt_client_publish(client, "/Reading", vBuffer, 0, 1, 0);
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}

double mcp3424_read()
{
	uint8_t buffer[10] = { 0 };
	double adcReading;
	uint8_t sign;
	char stBuffer[20] = { 0 };

  //  i2c_master_read_slave(1, (uint8_t)MCP3424ADDR, buffer, 6);
    i2c_read_slave(I2C_MASTER_NUM, (uint8_t)MCP3424ADDR, buffer, 7);

	
	if( (buffer[0] & 0x02) == 0x2) {
		//Negative
		sign = 0xFF;
		adcReading = (int32_t)(sign << 24 | ((buffer[0] & 0x01) | 0xFE)<<16 | buffer[1]<<8 | buffer[2]) * 0.000015625;
	} else {
		sign = 0;
		adcReading = (int32_t)(sign << 24 | (buffer[0] & 01) <<16 | buffer[1]<<8 | buffer[2]) * 0.000015625;
	}
	printf("ADC reading %.5f %02x %02x %02x %02x %02x %02x\n", adcReading, buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[6]);
	sprintf(stBuffer, "%.5fV    ", adcReading);
	WriteString(2, 0, stBuffer);
    return adcReading; 
}


void protocol_init(void) {
    ESP_LOGI(TAG, "[APP] Startup..");
    ESP_LOGI(TAG, "[APP] Free memory: %d bytes", esp_get_free_heap_size());
    ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());

    esp_log_level_set("*", ESP_LOG_INFO);
    esp_log_level_set("MQTT_CLIENT", ESP_LOG_VERBOSE);
    esp_log_level_set("MQTT_EXAMPLE", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT_TCP", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT_SSL", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT", ESP_LOG_VERBOSE);
    esp_log_level_set("OUTBOX", ESP_LOG_VERBOSE);

    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    /* This helper function configures Wi-Fi or Ethernet, as selected in menuconfig.
     * Read "Establishing Wi-Fi or Ethernet Connection" section in
     * examples/protocols/README.md for more information about this function.
     */
    ESP_ERROR_CHECK(example_connect());
    vTaskDelay(3000 / portTICK_RATE_MS);
}


void app_main(void)
{
    uint8_t mcp3424cfg = 0x1C;
    double voltsRead;
    char buffer[20]; 

    ESP_ERROR_CHECK(i2c_master_init());
    //xTaskCreate(i2c_test_task, "i2c_test_task_0", 1024 * 2, (void *)0, 10, NULL);
    //xTaskCreate(i2c_test_task, "i2c_test_task_1", 1024 * 2, (void *)1, 10, NULL);

    //xTaskCreate(i2c_ads1015, "i2c_ads1015", 1024 * 2, (int *)1, 10, NULL);
  
    initGPIOs();
    Setup4bit();
    DisplayClear();
   // init_adc();
    protocol_init();

    esp_err_t status = ic2_master_write_byte(I2C_MASTER_NUM, MCP3424ADDR, mcp3424cfg);
    if(status != ESP_OK) {
        printf("Error communicating with MCP3424 ADC\n");
    }

    WriteString(0, 0, (char *)"Hello");
    WriteString(3, 0, (char *)"February 5, 2021");

 //   xTaskCreate((void *) mainloop, "i2c_task", 1024 * 2, (void *)0, 10, NULL);
    //xTaskCreate((void *) mqtt_app_start, "imqtt_task", 1024 * 2, (void *)0, 10, NULL);
    mqtt_app_start();

    while(1) {
        voltsRead = mcp3424_read();
        sprintf(buffer, "%.5f", voltsRead);
        int msg_id = esp_mqtt_client_publish(client, "/Reading", buffer, 0, 1, 0);
        vTaskDelay((1000) / portTICK_RATE_MS);
    }

    puts("End Program");
}
