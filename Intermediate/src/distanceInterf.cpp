#include "i2cInterf.h"
#include "distanceInterf.h"
#include "stm32f4xx_hal.h"

distanceInterf::distanceInterf() : i2c() {}

distanceInterf::distanceInterf(i2cInterf i2c) :
        i2c(i2c) {}

distanceInterf::distanceInterf(I2C_HandleTypeDef &hi2c) :
        i2c(hi2c, 0) {}

distanceInterf::distanceInterf(I2C_HandleTypeDef &hi2c, uint8_t sensorAddr) :
        i2c(hi2c, sensorAddr) {}

float distanceInterf::readDistance() {
        uint8_t data[2];
        i2c.write((char *) &register_address, 1);
        i2c.read((char *) data, 2);
        float output = barrel_length_inches + 
            (float(((uint16_t) data[0] << 4) + (data[1] & 0x0f)) / 16 / 4/ 2.54);
        if (output > 35) {
            output = 0;
        }
        return output;
}

void distanceInterf::_i2c_write(uint8_t register_address1, uint8_t register_address2) {
    uint8_t data[] = { register_address1, register_address2 };
    i2c.write((char *) data, 2);
}

/*
 * I2C Data reading
 */
uint8_t distanceInterf::_i2c_read(uint8_t register_address) {
    uint8_t data;
    i2c.write((char *) &register_address, 1);
    i2c.read((char *) &data, 1);
    return data;
}


/*
 * Vpp 3.3V Power Supply
 */
void distanceInterf::_vpp_on(void) {
    vpp = 1;
}

/*
 * Vpp Ground
 */
void distanceInterf::_vpp_off(void) {
    vpp = 0;
}

/*
 * (Fig.40 E-Fuse Program Flow)
 * Stage 1
 * Data=0xFF is set in Address=0xEC.
 * 3.3V is applied in the Vpp terminal.
 */
void distanceInterf::e_fuse_stage1() {
    _i2c_write(0xEC, 0xFF);
    _vpp_on();
}

/*
 * (Fig.40 E-Fuse Program Flow)
 * Stage 2
 * Data=0x00 is set in Address=0xC8.
 */
void distanceInterf::e_fuse_stage2() {
    _i2c_write(0xC8, 0x00);
}

/*
 * (Fig.40 E-Fuse Program Flow)
 * Stage 3
 * Data=0x45 is set in Address=0xC9.
 * + programming bit #: 5 => 5 - 1 = 4
 * + bank value: 5 => Bank E
 */
void distanceInterf::e_fuse_stage3() {
    _i2c_write(0xC9, 0x45);
}

/*
 * (Fig.40 E-Fuse Program Flow)
 * Stage 4
 * Data=0x01 is set in Address=0xCD.
 * (Data=0x01 for slave address being changed to 0x10(write) and 0x11(read))
 * @param new_address 0-15 (Default address is 8, 0x80 for writing and 0x81 for reading)
 */
void distanceInterf::e_fuse_stage4(uint8_t new_address) {
    _i2c_write(0xCD, new_address);
}

/*
 * (Fig.40 E-Fuse Program Flow)
 * Stage 5
 * Data=0x01 is set in Address=0xCA.
 * Wait for 500us.
 */
void distanceInterf::e_fuse_stage5() {
    _i2c_write(0xCA, 0x01);
    HAL_Delay(1);
}

/*
 * (Fig.40 E-Fuse Program Flow)
 * Stage 6
 * Data=0x00 is set in Address=0xCA.
 * Vpp terminal is grounded.
 */
void distanceInterf::e_fuse_stage6() {
    _i2c_write(0xCA, 0x00);
    _vpp_off();
}

/*
 * (Fig.40 E-Fuse Program Flow)
 * Stage 7
 * Data=0x00 is set in Address=0xEF.
 * Data=0x40 is set in Address=0xC8.
 * Data=0x00 is set in Address=0xC8.
 */
void distanceInterf::e_fuse_stage7() {
    _i2c_write(0xEF, 0x00);
    _i2c_write(0xC8, 0x40);
    _i2c_write(0xC8, 0x00);
}

/*
 * (Fig.40 E-Fuse Program Flow)
 * Stage 8
 * Data=0x06 is set in Address=0xEE.
 */
void distanceInterf::e_fuse_stage8() {
    _i2c_write(0xEE, 0x06);
}

/*
 * (Fig.40 E-Fuse Program Flow)
 * Stage 9
 * Data=0xFF is set in Address=0xEC.
 * Data=0x03 is set in Address=0xEF.
 * Read out the data in Address=0x27.
 * Data=0x00 is set in Address=0xEF.
 * Data=0x7F is set in Address=0xEC.
 *
 * @return 0 for success, 1 for failure : 0x27[4:0] & 0b10000(0x10)
 */
uint8_t distanceInterf::e_fuse_stage9() {
    // Table.20 List of E-Fuse program flow and setting value
    _i2c_write(0xEF, 0x00); // add this though it's missing in 12-6 Example of E-Fuse Programming
    _i2c_write(0xEC, 0xFF);
    _i2c_write(0xEF, 0x03);
    const uint8_t check_value = _i2c_read(0x27);
    const uint8_t check = check_value & 0x1f;
    const uint8_t success = check & 0x10;
    // When lower 5bits data[4:0] is 00001, E-Fuse program is finished.
    // When lower 5bits data[4:0] is not 00001, go to stage10(bit replacement).
    _i2c_write(0xEF, 0x00);
    _i2c_write(0xEC, 0x7F);
    // Check Result
    return success;
}

/*
 * (Fig.40 E-Fuse Program Flow)
 * Stage 10-1
 * Data=0xFF is set in Address=0xEC.
 * 3.3V is applied in Vpp terminal.
 */
void distanceInterf::e_fuse_stage10_1_1() {
    _i2c_write(0xEC, 0xFF);
    _vpp_on();
}

/*
 * (Fig.40 E-Fuse Program Flow)
 * Stage 10-2
 * Data=0x37 is set in Address=0xC8.
 */
void distanceInterf::e_fuse_stage10_2_1() {
    _i2c_write(0xC8, 0x37);
}

/*
 * (Fig.40 E-Fuse Program Flow)
 * Stage 10-3
 * Data=0x74 is set in Address=0xC9.
 */
void distanceInterf::e_fuse_stage10_3_1() {
    _i2c_write(0xC9, 0x74);
}

/*
 * (Fig.40 E-Fuse Program Flow)
 * Stage 10-4
 * Data=0x04 is set in Address=0xCD.
 */
void distanceInterf::e_fuse_stage10_4_1() {
    _i2c_write(0xCD, 0x04);
}

/*
 * (Fig.40 E-Fuse Program Flow)
 * Stage 10-5
 * Data=0x01 is set in Address=0xCA.
 * Wait for 500us.
 */
void distanceInterf::e_fuse_stage10_5_1() {
    _i2c_write(0xCA, 0x01);
    HAL_Delay(1);
}

/*
 * (Fig.40 E-Fuse Program Flow)
 * Stage 10-6
 * Data=0x00 is set in Address=0xCA.
 * Vpp terminal is grounded.
 */
void distanceInterf::e_fuse_stage10_6_1() {
    _i2c_write(0xCA, 0x00);
    _vpp_off();
}

/*
 * (Fig.40 E-Fuse Program Flow)
 * Stage 10-1'
 * Data=0xFF is set in Address=0xEC.
 * 3.3V is applied in Vpp terminal.
 */
void distanceInterf::e_fuse_stage10_1_2() {
    _i2c_write(0xEC, 0xFF);
    _vpp_on();
}

/*
 * (Fig.40 E-Fuse Program Flow)
 * Stage 10-2'
 * Data=0x3F is set in Address=0xC8.
 */
void distanceInterf::e_fuse_stage10_2_2() {
    _i2c_write(0xC8, 0x3F);
}

/*
 * (Fig.40 E-Fuse Program Flow)
 * Stage 10-3'
 * Data=0x04 is set in Address=0xC9.
 */
void distanceInterf::e_fuse_stage10_3_2() {
    _i2c_write(0xC9, 0x04);
}

/*
 * (Fig.40 E-Fuse Program Flow)
 * Stage 10-4'
 * Data=0x01 is set in Address=0xCD.
 */
void distanceInterf::e_fuse_stage10_4_2() {
    _i2c_write(0xCD, 0x01);
}

/*
 * (Fig.40 E-Fuse Program Flow)
 * Stage 10-5'
 * Data=0x01 is set in Address=0xCA.
 * Wait for 500us.
 */
void distanceInterf::e_fuse_stage10_5_2() {
    _i2c_write(0xCA, 0x01);
    HAL_Delay(1);
}

/*
 * (Fig.40 E-Fuse Program Flow)
 * Stage 10-6'
 * Data=0x00 is set in Address=0xCA.
 * Vpp terminal is grounded.
 */
void distanceInterf::e_fuse_stage10_6_2() {
    _i2c_write(0xCA, 0x00);
    _vpp_off();
}

/*
 * (Fig.40 E-Fuse Program Flow)
 * Stage 10-7
 * Data=0x00 is set in Address=0xEF.
 * Data=0x40 is set in Address=0xC8.
 * Data=0x00 is set in Address=0xC8.
 */
void distanceInterf::e_fuse_stage10_7() {
    _i2c_write(0xEF, 0x00);
    _i2c_write(0xC8, 0x40);
    _i2c_write(0xC8, 0x00);
}

/*
 * (Fig.40 E-Fuse Program Flow)
 * Stage 10-8
 * Data=0x06 is set in Address=0xEE.
 */
void distanceInterf::e_fuse_stage10_8() {
    _i2c_write(0xEE, 0x06);
}

/*
 * (Fig.40 E-Fuse Program Flow)
 * Stage 10-9
 * Data=0xFF is set in Address=0xEC.
 * Data=0x03 is set in Address=0xEF.
 * Read out the data in Address=0x18 and Address=0x19.
 */
void distanceInterf::e_fuse_stage10_9() {
    _i2c_write(0xEC, 0xFF);
    _i2c_write(0xEF, 0x03);
}

/*
 * 12-6 Example of E-Fuse Programming
 */
void distanceInterf::e_fuse_run(uint8_t new_address) {

    if (new_address == i2c.getDevAddr()) {
        return;
    }
    if (new_address > 0x0f) {
        new_address = new_address>>4;
    }

    _vpp_off();
    HAL_Delay(1);

    e_fuse_stage1();
    e_fuse_stage2();
    e_fuse_stage3();
    e_fuse_stage4(new_address);
    e_fuse_stage5();
    e_fuse_stage6();
    e_fuse_stage7();
    e_fuse_stage8();
    const uint8_t result = e_fuse_stage9();
    if (result) {
        e_fuse_stage10_1_1();
        e_fuse_stage10_2_1();
        e_fuse_stage10_3_1();
        e_fuse_stage10_4_1();
        e_fuse_stage10_5_1();
        e_fuse_stage10_6_1();

        e_fuse_stage10_1_2();
        e_fuse_stage10_2_2();
        e_fuse_stage10_3_2();
        e_fuse_stage10_4_2();
        e_fuse_stage10_5_2();
        e_fuse_stage10_6_2();

        e_fuse_stage10_7();
        e_fuse_stage10_8();
        e_fuse_stage10_9();
    }
}