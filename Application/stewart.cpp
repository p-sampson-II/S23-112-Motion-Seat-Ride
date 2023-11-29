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

#include <math.h>
#include <array>
#include <tuple>
#include "arm_math.h"
#include "stewart.h"
#include "matrixdefs.h"

// Control Matrix Implementation
// Performs the matrix math required to convert roll, pitch and yaw into cylinder lengths.
// Then, performs PI control on 6 linear actuators to achieve the desired roll, pitch and yaw.
class StewartImpl {
public:
    StewartImpl();
    ~StewartImpl();
    mtx3D calcRotationMtx(fArr3 rpy);
    fArr6 xfmLinPos(fArr3 rpy);
private:
    fArr6 calcCylinderPos(mtx3D rotationMtx);
    float32_t calcNorm(fArr3 vector);
    float32_t performTrigOp(trigOp f, float x);
};

float32_t StewartImpl::performTrigOp(trigOp f, float x) {
    switch(trigEnum(f)) {
        case 0:
        case 1:
            return float(int(f));
        case COS:
            return cos(x);
        case SIN:
            return sin(x);
        case -COS:
            return -cos(x);
        case -SIN:
            return -sin(x);
        case -1:
            throw int(f); // Happens if we somehow get a trigOp of -1
    }
    return 0;
}

Stewart::Stewart() {
    impl = new StewartImpl();
}

Stewart::~Stewart() {
    delete impl;
}

fArr6 Stewart::xfmLinPos(fArr3 rpy) {
    return impl->xfmLinPos(rpy);
}

StewartImpl::StewartImpl() {}

StewartImpl::~StewartImpl() {}

// TODO: Figure out the math necessary to convert a set of cylinder positions into roll, pitch and yaw.
/*fArr3 StewartImpl::xfmRPY(fArr6 cylinderPos) {
    
}*/

/**
 * @brief Convert [Roll, Pitch, Yaw] -> [l1, l2, ... l6], 
                    where l1,...l6 are desired lengths of cylinders
                    on a stewart platform, and Roll, Pitch and Yaw
                    describe polar rotations of the seat on the platform.
 * 
 * @param rpy [Roll, Pitch, Yaw]
 * @return fArr6 [l1, l2, ... , l6]
 */
fArr6 StewartImpl::xfmLinPos(fArr3 rpy) {
    mtx3D rotationMtx = calcRotationMtx(rpy);
    fArr6 cylinderPos = calcCylinderPos(rotationMtx);
    return cylinderPos;
}


/**
 * @brief Performs a trig operation on a float depending on 
            the value of a trigOp enumeration
 * 
 * @param f trigOp enumeration describing the operation to perform
 * @param x Input of the trig operation being performed
 * @return float Result of the trig operation
 */

/**
 * @brief Calculate a 3x3 rotation matrix from [Roll, Pitch, Yaw].
 * 
 * @param rpy Array representing [Roll, Pitch, Yaw]
 * @return mtx3D Rotation Matrix calculated from rpy
 */
mtx3D StewartImpl::calcRotationMtx(fArr3 rpy) { 
    // Array of matricies repressenting R_x, R_y, R_z
    std::array<mtx3D,3> rotComponents;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 9; j++) {
            // Calculate the elements of each component matrix
            rotComponents[i][j] = performTrigOp(ROTATION_COMPONENTS[i][j],rpy[i]);
        }
    }

    // Initialize matrix math objects
    arm_matrix_instance_f32 am_accum[2];
    mtx3D accumBuf[2];
    arm_matrix_instance_f32 am_rotComponents[3];

    for(int i = 0; i < 3; i++) {
        if( i < 2) {
            arm_mat_init_f32(&am_accum[i],3,3,(float *)(accumBuf[i].data()));
        }
        arm_mat_init_f32(&am_rotComponents[i],3,3,(float *)(rotComponents[i].data()));
    }

    // Calculate R, the product of the rotation matrix components.
    // Do so in two steps, the code was just easier to write this way.
    arm_mat_mult_f32(&am_rotComponents[0],&am_rotComponents[1],&am_accum[0]);
    arm_mat_mult_f32(&am_accum[0],&am_rotComponents[2],&am_accum[1]);
    
    return accumBuf[1];
}

/**
 * @brief Performs the operation [l1,l2,...,l6] = [norm(P)], where 
 * P = T+R*p_i-b_i is the position of the head of each cylinder
 * in cartesian coordinates, R is rotationMtx, p_i, T, and b_i are
 * constant vectors derived from Hannah's calculations
 * 
 * @param rotationMtx A matrix describing how each element in a 
                        vector is scaled based on the seat's rotation
 * @return fArr6 A vector representing the length that each cylinder 
                    must be to achieved the desired roll, pitch, and yaw
 */
fArr6 StewartImpl::calcCylinderPos(mtx3D rotationMtx) {
    arm_matrix_instance_f32 am_rotMtx;
    arm_matrix_instance_f32 am_T;
    fArr6 lengths;
    
    // Matrix math initializations
    arm_mat_init_f32(&am_T,NO_DOF,1, (float *)VECTOR_T_DATA);
    arm_mat_init_f32(&am_rotMtx,NO_DOF,NO_DOF,(float *) rotationMtx.data());

    // Perform for each cylinder:
    for (int i = 0; i < NO_CYLS; i++) {
        // Point to the particular three values representing p and b
        // for the particular cylinder
        float32_t * p = (float32_t *)MATRIX_p_DATA + NO_DOF*i;
        float32_t * b = (float32_t *)MATRIX_b_DATA + NO_DOF*i;
        
        // Buffers for matrix math
        fArr3 RdotpBuf;
        fArr3 RpSubbBuf;
        fArr3 lengthVec;

        // More matrix math initializations
        arm_matrix_instance_f32 am_p;
        arm_matrix_instance_f32 am_b;

        arm_matrix_instance_f32 am_Rdotp;
        arm_matrix_instance_f32 am_RpSubb;
        arm_matrix_instance_f32 am_lengthVec;

        arm_mat_init_f32(&am_lengthVec,NO_DOF,1,lengthVec.data());
        arm_mat_init_f32(&am_p,NO_DOF,1,p);
        arm_mat_init_f32(&am_b,NO_DOF,1,b);
        arm_mat_init_f32(&am_Rdotp,NO_DOF,1,RdotpBuf.data());
        arm_mat_init_f32(&am_RpSubb,NO_DOF,1,RpSubbBuf.data());

        // Do the math:
        arm_mat_mult_f32(&am_rotMtx,&am_p,&am_Rdotp); // R*p = Rdotp
        arm_mat_sub_f32(&am_Rdotp,&am_b,&am_RpSubb); // Rdotp-b = RpSubb
        arm_mat_add_f32(&am_T,&am_RpSubb,&am_lengthVec); // T+RpSubb = lengthVec
        
        // lengthVec is in 3D cartesian coordinates. Get its magnitude:
        lengths[i] = calcNorm(lengthVec);
    }
    return lengths;
}

/**
 * @brief Finds the pythagorian magnitude of a vector, e.g.:
            y=sqrt(x_1^2+x_2^2+...+x_n^2) where n is the number of elements
 * 
 * @param vector The vector to find the norm of
 * @return float The norm of the vector parameter
 */
float StewartImpl::calcNorm(fArr3 vector) {
    // Buffer containing the square of sums
    float vecPower = 0;
    
    // Perform sum of squares of the input vector
    arm_power_f32((float *)vector.data(), 3, &vecPower);

    // Take the square root of the sum of squares
    float norm = sqrt(vecPower);
    return norm;
}