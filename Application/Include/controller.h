/*
Authors: Paul Sampson and Kelvin Hoang

Date: Fall 2023

Description: Implementation of a single-dimension PID controller with optional dynamic integral term.
             This integral term can be turned on and off based on an error threshold ratio (error/desired).
*/

#ifndef CTRL_H
#define CTRL_H

// Default values for PID parameters:
#define K_P 7
#define K_I 0.1
#define K_D 0
#define SAMPLE_PER 0.025

#include <array>
#include "matrixdefs.h"

const float norm = 255;

class CtrlImpl;

// Separate interface from implementation; Use NO_CYLS number of implementations in the interface
class Controller {
public:
    Controller();
    Controller(float const & kp,float const & ki, float const & kd, float const & samplePer);
    ~Controller();

    fArr6 step(fArr6 desired, fArr6 actual);

private:
    void init(float const & kp,float const & ki, float const & kd, float const & samplePer);
    std::array<CtrlImpl*,NO_CYLS> impl;
};

#endif // CTRL_H