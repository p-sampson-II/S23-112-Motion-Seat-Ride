/*
Authors: Paul Sampson and Kelvin Hoang

Date: Fall 2023

Description: Implementation of a single-dimension PID controller with optional dynamic integral term.
             This integral term can be turned on and off based on an error threshold ratio (error/desired).

Formulae used:

    Error = Desired - Actual

    Error Ratio = Error / Desired

    Error Sum = Sigma_{Initial Sample}^{Present Sample} (Error)

    Error Slope = (Error[k] - Error[k - 1]) / Sample Period

    Control Variable = Kp * Error + Ki * Error Sum + Kd * Error Slope
*/

#include <array>
#include "controller.h"

// Implementation of a single instance of the control system.
class CtrlImpl {
public:
    CtrlImpl();
    CtrlImpl(float const & kp,float const & ki, float const & kd, float const & samplePer);
    ~CtrlImpl();

    float calcSample(float const & desired, float const & actual);
private:
    float kp,ki,kd;
    float errorSum;
    float errorOld;
    float samplePer;

    bool calculateIntegral;
    float integralErrorThreshold;
};

Controller::Controller() {
    init(K_P,K_I,K_D,SAMPLE_PER);
}

Controller::Controller(float const & kp,float const & ki, float const & kd, float const & samplePer) {
    init(kp,ki,kd,samplePer);
}

void Controller::init(float const & kp,float const & ki, float const & kd, float const & samplePer) {
    for (int i = 0; i < 6; i++){
        // Instantiate each cylinder postion controller`1
        impl[i] = new CtrlImpl(kp, ki, kd, samplePer);
    }
}

Controller::~Controller() {
    for (int i = 0; i < 6; i++){
        delete impl[i];
    }
}

/**
 * @brief Calculate the next sample of all controllers.
 * 
 * @param desired Array of the desired position for each controller
 * @param actual Array of the actual position of each controller, from sensor data
 * @return fArr6 The voltage to set each solenoid - Will likely be digital (HIGH, LOW)
 */
fArr6 Controller::step(fArr6 desired, fArr6 actual) {
    fArr6 output;
    for (int i = 0; i < 6; i++) {
        output[i] = (impl[i]->calcSample(desired[i],actual[i]))/norm;
    }
    return output;
}

CtrlImpl::CtrlImpl(float const & kp,float const & ki, float const & kd, float const & samplePer) : 
    kp(kp), ki(ki), kd(kd), errorSum(0), errorOld(0), samplePer(samplePer) {}

CtrlImpl::CtrlImpl() : 
    kp(K_P), ki(K_I), kd(K_D), errorSum(0), errorOld(0), samplePer(SAMPLE_PER) {}

CtrlImpl::~CtrlImpl() {}

/** @brief Calculate the value of the control variable based on the state of a system.
*
*   @param desired The desired input value for the controller to achieve
*   @param actual The actual value of the input variable
*
*   @returns The value to set the variable being controlled (Likely voltage).
*/
float CtrlImpl::calcSample(float const & desired, float const & actual) {

    float error = desired - actual; // e[k]
    float errRatio = error/desired; // Used to decide whether to use integral control
    float errorSlope = (error-errorOld)/samplePer; // Delta(e(kT))/Delta(kT)

    float output = kp*error + ki*errorSum + kd*errorSlope; // h(k)

    // Calculate these variables for the succeeding sample:

    if(ki > 0 && errRatio < integralErrorThreshold) {
        errorSum += error; // sum_{k_1}^{k}(error)
    }

    else {
        errorSum = 0;
    }
    
    errorOld = error; 

    return output;
}
