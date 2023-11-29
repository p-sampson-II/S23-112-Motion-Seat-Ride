#include "distGovernor.h"


void DistGov::init(I2C_HandleTypeDef &hi2c, byteArr6 sensorAddrs) {
    for (int i = 0; i < NO_CYLS; i++) {
        sensors[i] = new distanceInterf(hi2c, sensorAddrs[i]);
    }
}

void DistGov::init(I2C_HandleTypeDef &hi2c) {
    //I2C_HandleTypeDef &hi2c = NULL;
    byteArr6 sensorAddrs = sensor_addresses;
    init(hi2c, sensorAddrs);
}

fArr6 DistGov::readDistances() {
    fArr6 output;
    for (int i = 0; i < NO_CYLS; i++) {
        output[i] = sensors[i]->readDistance();
    }
    return output;
}

void DistGov::e_fuse_run(uint8_t sensor, uint8_t new_address) {
    sensors[sensor]->e_fuse_run(new_address);
}

DistGov::DistGov(I2C_HandleTypeDef &hi2c) {
    init(hi2c);
}

DistGov::DistGov(I2C_HandleTypeDef &hi2c, byteArr6 sensorAddrs) {
    init(hi2c, sensorAddrs);
}