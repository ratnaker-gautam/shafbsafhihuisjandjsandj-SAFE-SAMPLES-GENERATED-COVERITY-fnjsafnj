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

double matrix3x3_determinant(struct Matrix3x3 m) {
    double det = m.elements[0][0] * (m.elements[1][1] * m.elements[2][2] - m.elements[1][2] * m.elements[2][1])
               - m.elements[0][1] * (m.elements[1][0] * m.elements[2][2] - m.elements[1][2] * m.elements[2][0])
               + m.elements[0][2] * (m.elements[1][0] * m.elements[2][1] - m.elements[1][1] * m.elements[2][0]);
    return det;
}

int linear_system_solve(struct LinearSystem *sys) {
    double det = matrix3x3_determinant(sys->coefficients);
    if (fabs(det) < 1e-12) {
        return 0;
    }
    struct Matrix3x3 m1 = matrix3x3_create(sys->constants.x, sys->coefficients.elements[0][1], sys->coefficients.elements[0][2],
                                           sys->constants.y, sys->coefficients.elements[1][1], sys->coefficients.elements[1][2],
                                           sys->constants.z, sys->coefficients.elements[2][1], sys->coefficients.elements[2][2]);
    struct Matrix3x3 m2 = matrix3x3_create(sys->coefficients.elements[0][0], sys->constants.x, sys->coefficients.elements[0][2],
                                           sys->coefficients.elements[1][0], sys->constants.y, sys->coefficients.elements[1][2],
                                           sys->coefficients.elements[2][0], sys->constants.z, sys->coefficients.elements[2][2]);
    struct Matrix3x3 m3 = matrix3x3_create(sys->coefficients.elements[0][0], sys->coefficients.elements[0][1], sys->constants.x,
                                           sys->coefficients.elements[1][0], sys->coefficients.elements[1][1], sys->constants.y,
                                           sys->coefficients.elements[2][0], sys->coefficients.elements[2][1], sys->constants.z);
    sys->solution.x = matrix3x3_determinant(m1) / det;
    sys->solution.y = matrix3x3_determinant(m2) / det;
    sys->solution.z = matrix3x3_determinant(m3) / det;
    return 1;
}

int read_double(const char *prompt, double *value) {
    char buffer[256];
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    char *endptr;
    *value = strtod(buffer, &endptr);
    if (endptr == buffer) {
        return 0;
    }
    return 1;
}

int main(void) {
    struct LinearSystem sys;
    printf("Enter coefficients for 3x3 linear system:\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            char prompt[64];
            snprintf(prompt, sizeof(prompt), "A[%d][%d]: ", i+1, j+1);
            if (!read_double(prompt, &sys.coefficients.elements[i][j])) {
                printf("Invalid input\n");
                return 1;
            }
        }
    }
    printf("Enter constants vector:\n");
    if (!read_double("b1: ", &sys.constants.x) ||
        !read_double("b2: ", &sys.constants.y) ||
        !read_double("b3: ", &sys.constants.z)) {
        printf("Invalid input\n");
        return 1;
    }
    if (linear_system_solve(&sys)) {