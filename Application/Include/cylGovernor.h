#pragma once

#include <array>
#include "pins.h"
#include "CylCtrlInterf.h"

class CylGov {
private:
    cylArr6 cylinders;

    void init(pccrArr6 pccrs, portArr6 dirPorts, pinArr6 dirPins);
    void init();
public:
    CylGov();
    CylGov(pccrArr6 pccrs, portArr6 dirPorts, pinArr6 dirPins);

    fArr6 getDutyCycles();
    void setDutyCycles(const fArr6 &dutyCycles);
};