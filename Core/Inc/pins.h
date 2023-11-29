#pragma once

#include "matrixdefs.h"
#include "CylCtrlInterf.h"
#include "distanceInterf.h"

// For use in conjunction with CylGov object:
using cylArr6 = std::array<CylCtrlInterf*, NO_CYLS>;
using pinArr6 = std::array<uint16_t, NO_CYLS>;
using portArr6 = std::array<GPIO_TypeDef*, NO_CYLS>;
using pccrArr6 = std::array<volatile uint32_t*, NO_CYLS>;

// For use in conjunction with distanceInterf object:
using sensArr6 = std::array<distanceInterf*, NO_CYLS>;
using byteArr6 = std::array<uint8_t,NO_CYLS>;

const pinArr6 direction_pins = {GPIO_PIN_0,GPIO_PIN_1,GPIO_PIN_2,GPIO_PIN_3,
                                    GPIO_PIN_4,GPIO_PIN_5};

const portArr6 direction_ports = {GPIOC,GPIOC,GPIOC,GPIOC,GPIOC,GPIOC};

const pccrArr6 pccr_defs = {&(TIM1->CCR1), &(TIM1->CCR2), &(TIM1->CCR3), &(TIM1->CCR4),
                                &(TIM8->CCR1), &(TIM8->CCR2)};

const byteArr6 sensor_addresses = {0x80,0x20,0x30,0x40,0x50,0x60};