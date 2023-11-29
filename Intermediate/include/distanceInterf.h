#pragma once
#include "i2cInterf.h"

class distanceInterf {
private:
    i2cInterf i2c;
    const float barrel_length_inches = 15.5;
    const uint8_t register_address = 0x5E;
    //uint8_t sensorAddr;
    uint8_t vpp = 0;

    void _i2c_write(uint8_t register_address1, uint8_t register_address2);
    uint8_t _i2c_read(uint8_t register_address);
    void _vpp_on(void);
    void _vpp_off(void);
    void e_fuse_stage1();
    void e_fuse_stage2();
    void e_fuse_stage3();
    void e_fuse_stage4(uint8_t new_address);
    void e_fuse_stage5();
    void e_fuse_stage6();
    void e_fuse_stage7();
    void e_fuse_stage8();
    uint8_t e_fuse_stage9();
    void e_fuse_stage10_1_1();
    void e_fuse_stage10_2_1();
    void e_fuse_stage10_3_1();
    void e_fuse_stage10_4_1();
    void e_fuse_stage10_5_1();
    void e_fuse_stage10_6_1();
    void e_fuse_stage10_1_2();
    void e_fuse_stage10_2_2();
    void e_fuse_stage10_3_2();
    void e_fuse_stage10_4_2();
    void e_fuse_stage10_5_2();
    void e_fuse_stage10_6_2();
    void e_fuse_stage10_7();
    void e_fuse_stage10_8();
public:
    distanceInterf();
    distanceInterf(i2cInterf i2c);
    distanceInterf(I2C_HandleTypeDef &hi2c);
    distanceInterf(I2C_HandleTypeDef &hi2c, uint8_t sensorAddr);

    float readDistance();

    void e_fuse_stage10_9();
    void e_fuse_run(uint8_t new_address);
};