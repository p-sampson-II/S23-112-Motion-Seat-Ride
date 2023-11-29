#include "cylGovernor.h"
    //const uint16_t arr_val = 60000;

    //cylArr6 cylinders;



CylGov::CylGov() {
    init();
}

CylGov::CylGov(pccrArr6 pccrs, portArr6 dirPorts, pinArr6 dirPins) {
    init(pccrs, dirPorts, dirPins);
}

void CylGov::init() {
    pccrArr6 pccrs;
    portArr6 dirPorts;
    pinArr6 dirPins;

    pccrs = pccr_defs;
    dirPorts = direction_ports;
    dirPins = direction_pins;

    init(pccrs, dirPorts, dirPins);
}

void CylGov::init(pccrArr6 pccrs, portArr6 dirPorts, pinArr6 dirPins) {
    for (int i = 0; i < NO_CYLS; i++) {
        cylinders[i] = new CylCtrlInterf(*pccrs[i],dirPorts[i],dirPins[i]);
    }
}

fArr6 CylGov::getDutyCycles() {
    fArr6 output;
    for (int i = 0; i < NO_CYLS; i++) {
        output[i] = cylinders[i]->getDutyCycle();
    }
    return output;
}
void CylGov::setDutyCycles(const fArr6 &dutyCycles) {
    for (int i = 0; i < NO_CYLS; i++) {
        cylinders[i]->setDutyCycle(dutyCycles[i]);
    }
}
