#pragma once

#include <array>
#include "pins.h"
#include "distanceInterf.h"

class DistGov {
private:
    sensArr6 sensors;

    void init(I2C_HandleTypeDef &hi2c, byteArr6 sensorAddrs);
    void init(I2C_HandleTypeDef &hi2c);
public:
    DistGov(I2C_HandleTypeDef &hi2c);
    DistGov(I2C_HandleTypeDef &hi2c, byteArr6 sensorAddrs);

    fArr6 readDistances();

    void e_fuse_run(uint8_t sensor, uint8_t new_address);
};