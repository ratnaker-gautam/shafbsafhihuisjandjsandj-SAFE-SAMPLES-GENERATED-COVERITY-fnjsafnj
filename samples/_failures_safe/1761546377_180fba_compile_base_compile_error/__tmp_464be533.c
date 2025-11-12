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

struct Vector3D vector_add(struct Vector3D a, struct Vector3D b) {
    struct Vector3D result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    return result;
}

struct Vector3D vector_subtract(struct Vector3D a, struct Vector3D b) {
    struct Vector3D result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    return result;
}

double vector_dot_product(struct Vector3D a, struct Vector3D b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

struct Matrix3x3 matrix_multiply(struct Matrix3x3 a, struct Matrix3x3 b) {
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
    inv.elements[2][0] = (system->coefficients.elements[1][0] * system->coefficients.elements[2][1] - system->coefficients.elements[1][1] * system->coefficients.elements[2][0]) / det;
    inv.elements[2][1] = (system->coefficients.elements[0][1] * system->coefficients.elements[2][0] - system->coefficients.elements[0][0] * system->coefficients.elements[2][1]) / det;
    inv.elements[2][2] = (system->coefficients.elements[0][0] * system->coefficients.elements[1][1] - system->coefficients.elements[0][1] * system->coefficients.elements[1][0]) / det;
    
    system->solution.x = inv.elements[0][0] * system->constants.x + inv.elements[0][1] * system