#pragma once

#include "sdkconfig.h"
#include "esp_log.h"
#include "driver/i2c.h"

extern esp_err_t ic2_master_write_byte(i2c_port_t i2c_num, uint8_t address, uint8_t data_wr);