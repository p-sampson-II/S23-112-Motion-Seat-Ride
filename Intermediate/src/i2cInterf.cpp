#include "i2cInterf.h"
#include "stm32f4xx_hal.h"

i2cInterf::i2cInterf() : hi2c(NULL), devAddr(0) {}
i2cInterf::i2cInterf(I2C_HandleTypeDef &hi2c, uint8_t devAddr) : hi2c(&hi2c), devAddr(devAddr) {}

uint8_t i2cInterf::getDevAddr() {
    return devAddr;
}

void i2cInterf::write(char * register_address, uint8_t length) {
    HAL_I2C_Master_Transmit(hi2c, this->devAddr,(unsigned char *)register_address, length,100);
}

void i2cInterf::read(char * data, uint8_t length) {
    HAL_I2C_Master_Receive(hi2c, this->devAddr,(unsigned char *)data, length,100);
}