#pragma once

#include "stm32f4xx_hal.h"

class CylCtrlInterf {
private:
    const uint16_t arr_val = 60000;

    //TIM_HandleTypeDef *htim;
    GPIO_TypeDef *dirPort;
    uint16_t dirPin;

    float dutyCycle = 0;
    GPIO_PinState direction = GPIO_PIN_RESET;
    
    volatile uint32_t *pccr;
    void setPWM();
    void setDir();
public:
    CylCtrlInterf();
    CylCtrlInterf(volatile uint32_t &pccr, GPIO_TypeDef *dirPort, uint16_t dirPin);

    float getDutyCycle();
    void setDutyCycle(const float &dutyCycle);
};