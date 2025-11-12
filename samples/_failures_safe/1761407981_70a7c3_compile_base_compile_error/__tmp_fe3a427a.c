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

struct Vector3D vector3d_create(double x, double y, double z) {
    struct Vector3D v;
    v.x = x;
    v.y = y;
    v.z = z;
    return v;
}

struct Vector3D vector3d_add(struct Vector3D a, struct Vector3D b) {
    struct Vector3D result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    return result;
}

struct Vector3D vector3d_subtract(struct Vector3D a, struct Vector3D b) {
    struct Vector3D result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    return result;
}

double vector3d_dot(struct Vector3D a, struct Vector3D b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

struct Matrix3x3 matrix3x3_create(double a11, double a12, double a13,
                                  double a21, double a22, double a23,
                                  double a31, double a32, double a33) {
    struct Matrix3x3 m;
    m.elements[0][0] = a11; m.elements[0][1] = a12; m.elements[0][2] = a13;
    m.elements[1][0] = a21; m.elements[1][1] = a22; m.elements[1][2] = a23;
    m.elements[2][0] = a31; m.elements[2][1] = a32; m.elements[2][2] = a33;
    return m;
}

struct Matrix3x3 matrix3x3_multiply(struct Matrix3x3 a, struct Matrix3x3 b) {
    struct Matrix3x3 result;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result.elements[i][j] = 0.0;
            for (int k = 0; k < 3; k++) {
                result.elements[i][j] += a.elements[i][k] * b.elements[k][j];
            }
        }
    }
    return result;
}

double matrix3x3_determinant(struct Matrix3x3 m) {
    double det = m.elements[0][0] * (m.elements[1][1] * m.elements[2][2] - m.elements[1][2] * m.elements[2][1])
               - m.elements[0][1] * (m.elements[1][0] * m.elements[2][2] - m.elements[1][2] * m.elements[2][0])
               + m.elements[0][2] * (m.elements[1][0] * m.elements[2][1] - m.elements[1][1] * m.elements[2][0]);
    return det;
}

int linear_system_solve(struct LinearSystem *system) {
    if (system == NULL) {
        return 0;
    }
    double det = matrix3x3_determinant(system->coefficients);
    if (fabs(det) < 1e-12) {
        return 0;
    }
    struct Matrix3x3 inv;
    inv.elements[0][0] = (system->coefficients.elements[1][1] * system->coefficients.elements[2][2] - system->coefficients.elements[1][2] * system->coefficients.elements[2][1]) / det;
    inv.elements[0][1] = (system->coefficients.elements[0][2] * system->coefficients.elements[2][1] - system->coefficients.elements[0][1] * system->coefficients.elements[2][2]) / det;
    inv.elements[0][2] = (system->coefficients.elements[0][1] * system->coefficients.elements[1][2] - system->coefficients.elements[0][2] * system->coefficients.elements[1][1]) / det;
    inv.elements[1][0] = (system->coefficients.elements[1][2] * system->coefficients.elements[2][0] - system->coefficients.elements[1][0] * system->coefficients.elements[2][2]) / det;
    inv.elements[1][1] = (system->coefficients.elements[0][0] * system->coefficients.elements[2][2] - system->coefficients.elements[0][2] * system->coefficients.elements[2][0]) / det;
    inv.elements[1][2] = (system->coefficients.elements[0][2] * system->coe