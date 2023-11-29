#pragma once

#include "stm32f4xx_hal.h"

class i2cInterf {
private:
    I2C_HandleTypeDef *hi2c;
    uint8_t devAddr;
public:
    i2cInterf();
    i2cInterf(I2C_HandleTypeDef &hi2c, uint8_t devAddr);

    uint8_t getDevAddr();

    void write(char * register_address, uint8_t length);
    void read(char * data, uint8_t length);
};