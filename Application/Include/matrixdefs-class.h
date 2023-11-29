
#ifndef MATRIX_DEFS_H
#define MATRIX_DEFS_H
#include <cstdint>
#include <array>
#include <tuple>
#include <iostream>
#include <math.h>

#define NO_CYLS 6 // One output per cylinder representing linear position
#define NO_DOF 3 // Pitch, Roll, and Yaw; calculated from transformation matrix

// Describes a trig operation to be performed. ZERO and ONE simply indicate that the function should return
// 0 or 1.
enum class trigEnum { negsin=-3, negcos=-2, negone=-1 ,zero=0, one=1, cos=2, sin=3 };

// Implements a union of int and trigEnum, allowing the implicit conversion seen in the rotation function matricies.
// This is done to make checking the function matricies at a glance a LOT easier.
class trigOp {
private:
    union {
    int8_t value;
    trigEnum enumeration;
    };
public:
  operator int() const{
    return value;
  }

  operator trigEnum() const{
    return enumeration;
  }

  trigOp& operator=(int const &i){
    this->set(i);
    return *this;
  }

  trigOp& operator=(trigEnum const &e){
    this->set(e);
    return *this;
  }

  /*trigOp& operator-(){
      this->value = -int(value);
      return *this;
  }*/

  void set(int i){
    value = i;
  }
  void set(trigEnum e){
    enumeration = e;
  }

  trigOp(trigEnum e){
    this->set(e);
  }

  trigOp(int i){
    this->set(i);
  }

  trigOp() {
    this->set(0);
  }
};

float performTrigOp(trigOp f, float x) {
    /*switch(trigEnum(f)) {
        case trigEnum::zero:
        case trigEnum::one:
            return float(int(f));
        case trigEnum::cos:
            return cos(x);
        case trigEnum::sin:
            return sin(x);
        case trigEnum::negcos:
            return -cos(x);
        case trigEnum::negsin:
            return -sin(x);
        case trigEnum::negone:
            throw int(f); // Happens if we somehow get a trigOp of -1
    }*/
    return 0;
}

constexpr trigEnum operator-(const trigEnum& e){
  return static_cast<trigEnum>(-int(e));
}

// Aliases for commonly used array dimensions
using fArr6 = std::array<float, NO_CYLS>;
using fArr3 = std::array<float, NO_DOF>;

// Matricies in 3D cartesian space have 3*3 = 9 elements.
using mtx3D = std::array<float, NO_DOF*NO_DOF>;

//using ctrlArr = std::array<Controller, NO_CYLS>;
using trigOpArr = std::array<trigOp, 9>;

// Represents the set of vectors which describe ...
const float MATRIX_b_DATA[NO_CYLS*NO_DOF] = 
                                {-1.75, 12.63, 0,
                                  1.75, 12.63, 0,
                                  11.81, -4.8, 0,
                                  10.06,-7.83, 0,
                                 -10.06,-7.83, 0,
                                 -11.81, -4.8, 0 };

// Represents the set of vectors which describe ....
const float MATRIX_p_DATA[NO_CYLS*NO_DOF] = 
                                {-8.1,   6.69, 0,
                                  8.1,   6.69, 0,
                                  9.84,  3.67, 0,
                                  1.74,-10.35, 0,
                                 -1.74,-10.35, 0,
                                 -9.84,  3.67, 0 };

// Represents the cartesian translation of the center of the seat plate
const float VECTOR_T_DATA[3] = { 0, 0, 23 };

// Rotation function matricies. Tells Stewart what operations to perform
// on each angle parameter to calculate the rotation matrix.

const trigOpArr ROTATION_X = { 1, 0, 0, 0, trigEnum::cos, -trigEnum::sin, 0, trigEnum::sin, trigEnum::cos };
const trigOpArr ROTATION_Y = { trigEnum::cos, 0, trigEnum::sin, 0, 1, 0, -trigEnum::sin, 0, trigEnum::cos };
const trigOpArr ROTATION_Z = { trigEnum::cos, -trigEnum::sin, 0, trigEnum::sin, trigEnum::cos, 0, 0, 0, 1 };

// This is never used; nevertheless, it is included for future use.
const std::array<trigOpArr,3> ROTATION_COMPONENTS = {ROTATION_X, ROTATION_Y, ROTATION_Z};

#endif // MATRIX_DEFS_H