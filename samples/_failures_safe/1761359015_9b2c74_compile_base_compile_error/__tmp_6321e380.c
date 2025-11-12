//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

struct Vector3D {
    double x;
    double y;
    double z;
};

struct Matrix3x3 {
    double elements[3][3];
};

struct LinearSystem {
    struct Matrix3x3 coefficients;
    struct Vector3D constants;
    struct Vector3D solution;
};

struct Statistics {
    double mean;
    double variance;
    double standard_deviation;
};

struct Vector3D create_vector(double x, double y, double z) {
    struct Vector3D vec;
    vec.x = x;
    vec.y = y;
    vec.z = z;
    return vec;
}

struct Matrix3x3 create_matrix(double a11, double a12, double a13,
                              double a21, double a22, double a23,
                              double a31, double a32, double a33) {
    struct Matrix3x3 mat;
    mat.elements[0][0] = a11;
    mat.elements[0][1] = a12;
    mat.elements[0][2] = a13;
    mat.elements[1][0] = a21;
    mat.elements[1][1] = a22;
    mat.elements[1][2] = a23;
    mat.elements[2][0] = a31;
    mat.elements[2][1] = a32;
    mat.elements[2][2] = a33;
    return mat;
}

int validate_matrix_determinant(struct Matrix3x3 mat) {
    double det = mat.elements[0][0] * (mat.elements[1][1] * mat.elements[2][2] - mat.elements[1][2] * mat.elements[2][1])
               - mat.elements[0][1] * (mat.elements[1][0] * mat.elements[2][2] - mat.elements[1][2] * mat.elements[2][0])
               + mat.elements[0][2] * (mat.elements[1][0] * mat.elements[2][1] - mat.elements[1][1] * mat.elements[2][0]);
    return fabs(det) > 1e-10;
}

struct Vector3D matrix_vector_multiply(struct Matrix3x3 mat, struct Vector3D vec) {
    struct Vector3D result;
    result.x = mat.elements[0][0] * vec.x + mat.elements[0][1] * vec.y + mat.elements[0][2] * vec.z;
    result.y = mat.elements[1][0] * vec.x + mat.elements[1][1] * vec.y + mat.elements[1][2] * vec.z;
    result.z = mat.elements[2][0] * vec.x + mat.elements[2][1] * vec.y + mat.elements[2][2] * vec.z;
    return result;
}

int solve_linear_system(struct LinearSystem *system) {
    if (!validate_matrix_determinant(system->coefficients)) {
        return 0;
    }
    
    double det = system->coefficients.elements[0][0] * (system->coefficients.elements[1][1] * system->coefficients.elements[2][2] - system->coefficients.elements[1][2] * system->coefficients.elements[2][1])
               - system->coefficients.elements[0][1] * (system->coefficients.elements[1][0] * system->coefficients.elements[2][2] - system->coefficients.elements[1][2] * system->coefficients.elements[2][0])
               + system->coefficients.elements[0][2] * (system->coefficients.elements[1][0] * system->coefficients.elements[2][1] - system->coefficients.elements[1][1] * system->coefficients.elements[2][0]);
    
    if (fabs(det) < 1e-10) {
        return 0;
    }
    
    struct Matrix3x3 inv;
    inv.elements[0][0] = (system->coefficients.elements[1][1] * system->coefficients.elements[2][2] - system->coefficients.elements[1][2] * system->coefficients.elements[2][1]) / det;
    inv.elements[0][1] = (system->coefficients.elements[0][2] * system->coefficients.elements[2][1] - system->coefficients.elements[0][1] * system->coefficients.elements[2][2]) / det;
    inv.elements[0][2] = (system->coefficients.elements[0][1] * system->coefficients.elements[1][2] - system->coefficients.elements[0][2] * system->coefficients.elements[1][1]) / det;
    inv.elements[1][0] = (system->coefficients.elements[1][2] * system->coefficients.elements[2][0] - system->coefficients.elements[1][0] * system->coefficients.elements[2][2]) / det;
    inv.elements[1][1] = (system->coefficients.elements[0][0] * system->coefficients.elements[2][2] - system->coefficients.elements[0][2