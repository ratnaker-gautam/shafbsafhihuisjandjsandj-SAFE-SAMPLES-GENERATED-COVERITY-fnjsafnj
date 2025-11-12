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

struct Vector3D vector3d_create(double x, double y, double z) {
    struct Vector3D v;
    v.x = x;
    v.y = y;
    v.z = z;
    return v;
}

struct Matrix3x3 matrix3x3_create(double a11, double a12, double a13,
                                  double a21, double a22, double a23,
                                  double a31, double a32, double a33) {
    struct Matrix3x3 m;
    m.elements[0][0] = a11;
    m.elements[0][1] = a12;
    m.elements[0][2] = a13;
    m.elements[1][0] = a21;
    m.elements[1][1] = a22;
    m.elements[1][2] = a23;
    m.elements[2][0] = a31;
    m.elements[2][1] = a32;
    m.elements[2][2] = a33;
    return m;
}

struct Vector3D matrix_vector_multiply(struct Matrix3x3 m, struct Vector3D v) {
    struct Vector3D result;
    result.x = m.elements[0][0] * v.x + m.elements[0][1] * v.y + m.elements[0][2] * v.z;
    result.y = m.elements[1][0] * v.x + m.elements[1][1] * v.y + m.elements[1][2] * v.z;
    result.z = m.elements[2][0] * v.x + m.elements[2][1] * v.y + m.elements[2][2] * v.z;
    return result;
}

double vector_dot_product(struct Vector3D a, struct Vector3D b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

struct Vector3D vector_cross_product(struct Vector3D a, struct Vector3D b) {
    struct Vector3D result;
    result.x = a.y * b.z - a.z * b.y;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;
    return result;
}

double matrix_determinant(struct Matrix3x3 m) {
    double det = m.elements[0][0] * (m.elements[1][1] * m.elements[2][2] - m.elements[1][2] * m.elements[2][1])
               - m.elements[0][1] * (m.elements[1][0] * m.elements[2][2] - m.elements[1][2] * m.elements[2][0])
               + m.elements[0][2] * (m.elements[1][0] * m.elements[2][1] - m.elements[1][1] * m.elements[2][0]);
    return det;
}

int solve_linear_system(struct LinearSystem *system) {
    double det = matrix_determinant(system->coefficients);
    if (fabs(det) < 1e-12) {
        return 0;
    }
    struct Matrix3x3 inv;
    inv.elements[0][0] = (system->coefficients.elements[1][1] * system->coefficients.elements[2][2] - system->coefficients.elements[1][2] * system->coefficients.elements[2][1]) / det;
    inv.elements[0][1] = (system->coefficients.elements[0][2] * system->coefficients.elements[2][1] - system->coefficients.elements[0][1] * system->coefficients.elements[2][2]) / det;
    inv.elements[0][2] = (system->coefficients.elements[0][1] * system->coefficients.elements[1][2] - system->coefficients.elements[0][2] * system->coefficients.elements[1][1]) / det;
    inv.elements[1][0] = (system->coefficients.elements[1][2] * system->coefficients.elements[2][0] - system->coefficients.elements[1][0] * system->coefficients.elements[2][2]) / det;
    inv.elements[1][1] = (system->coefficients.elements[0][0] * system->coefficients.elements[2][2] - system->coefficients.elements[0][2] * system->coefficients.elements[2][0]) / det;
    inv.elements[1][2] = (system->coefficients.elements[0][2] * system->coefficients.elements[1][0] - system->coefficients.elements[0][0] * system->coefficients.elements[1][2]) / det;
    inv.elements[2][0] = (system->coefficients.elements[1][0] * system->coe