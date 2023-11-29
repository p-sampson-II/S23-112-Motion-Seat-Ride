/**
Authors: Paul Sampson and Kelvin Hoang

Date: Fall 2023

Description: Matrix operations required to convert [Roll, Pitch, Yaw] -> [l1, l2, ... l6] for a stewart platform.

Formulae used:

    L = T + R*p-b
    Where:
        L = [l_x;l_y;l_z] is the cartesian location of the cylinder head.
        T = [0;0;23] is the translation matrix for the center of the moving platform.
        R = R_x*R_y*R_z is the rotation matrix of the moving platform.
        p and b are precalculated vectors representing unique cylinder position information.
    
    R=R_x*R_y*R_z
    Where:
            [1  0         0         ]
        R_x=[0  cos(roll) -sin(roll)]
            [0  sin(roll) cos(roll) ]

            [cos(pitch)  0  sin(pitch)]
        R_y=[0           1  0         ]
            [-sin(pitch) 0  sin(pitch)]

            [cos(yaw) -sin(yaw)   0 ]
        R_z=[sin(yaw) cos(yaw)    0 ]
            [0        0           1 ]

    l = sqrt(l_x^2+l_y^2+l_z^2) (euclidian distance)
*/

#ifndef STEW_H
#define STEW_H

#include <array>
#include <tuple>
#include "matrixdefs.h"

class StewartImpl;
class Stewart {
public:
    Stewart();
    ~Stewart();

    fArr3 xfmRPY(fArr6 cylinderPos); // Cyl. linear position -> roll, pitch and yaw
    fArr6 xfmLinPos(fArr3 rpy); // roll, pitch and yaw -> Cyl. linear position
private:
   StewartImpl *impl;
};

#endif // STEW_H