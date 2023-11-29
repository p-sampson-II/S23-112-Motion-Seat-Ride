#include "CylCtrlInterf.h"
#include <math.h>

CylCtrlInterf::CylCtrlInterf() : dutyCycle(0), pccr(NULL), direction(GPIO_PIN_RESET) {}

CylCtrlInterf::CylCtrlInterf(volatile uint32_t &pccr, GPIO_TypeDef *dirPort, uint16_t dirPin) :
 dutyCycle(0), pccr(&pccr), dirPort(dirPort), dirPin(dirPin), direction(GPIO_PIN_RESET) {}

void CylCtrlInterf::setPWM() {
    *pccr = (uint16_t)(this->dutyCycle*this->arr_val);
}

void CylCtrlInterf::setDir() {
    HAL_GPIO_WritePin(dirPort,dirPin,direction);
}

float CylCtrlInterf::getDutyCycle() {
    return this->dutyCycle;
}

void CylCtrlInterf::setDutyCycle(const float &dutyCycle) {
    float magDC=abs(dutyCycle);
    if (magDC < 0.07) {
        this->dutyCycle = 0;
    }
    else {
        if (dutyCycle < 0) {
            this->direction = GPIO_PIN_RESET;
        }
        else {
            this->direction = GPIO_PIN_SET;
        }

        if (magDC > 1) {
            this->dutyCycle = 1;
        }
        else {
            this->dutyCycle = magDC;
        }
    }

    setDir();
    setPWM();
}
