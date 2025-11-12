//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>
#include <float.h>

struct Vector3D {
    double x;
    double y;
    double z;
};

struct Matrix3x3 {
    double elements[3][3];
};

struct TransformResult {
    struct Vector3D original;
    struct Vector3D transformed;
    double magnitude;
    int valid;
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

double vector_dot(struct Vector3D a, struct Vector3D b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

struct Vector3D vector_cross(struct Vector3D a, struct Vector3D b) {
    struct Vector3D result;
    result.x = a.y * b.z - a.z * b.y;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;
    return result;
}

double vector_magnitude(struct Vector3D v) {
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
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

struct Vector3D matrix_vector_multiply(struct Matrix3x3 m, struct Vector3D v) {
    struct Vector3D result;
    result.x = m.elements[0][0] * v.x + m.elements[0][1] * v.y + m.elements[0][2] * v.z;
    result.y = m.elements[1][0] * v.x + m.elements[1][1] * v.y + m.elements[1][2] * v.z;
    result.z = m.elements[2][0] * v.x + m.elements[2][1] * v.y + m.elements[2][2] * v.z;
    return result;
}

int is_finite_number(double x) {
    return x == x && x <= DBL_MAX && x >= -DBL_MAX;
}

int validate_vector(struct Vector3D v) {
    return is_finite_number(v.x) && is_finite_number(v.y) && is_finite_number(v.z);
}

int validate_matrix(struct Matrix3x3 m) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (!is_finite_number(m.elements[i][j])) {
                return 0;
            }
        }
    }
    return 1;
}

struct TransformResult apply_transformation(struct Matrix3x3 transform, struct Vector3D input) {
    struct TransformResult result;
    result.original = input;
    result.valid = 0;
    result.magnitude = 0.0;
    
    if (!validate_matrix(transform) || !validate_vector(input)) {
        return result;
    }
    
    struct Vector3D transformed = matrix_vector_multiply(transform, input);
    
    if (!validate_vector(transformed)) {
        return result;
    }
    
    double mag = vector_magnitude(transformed);
    if (!is_finite_number(mag)) {
        return result;
    }
    
    result.transformed = transformed;
    result.magnitude = mag;
    result.valid = 1;
    return result;
}

int main(void) {
    struct Vector3D vec1 = {1.0, 2.0, 3.0};
    struct Vector3D vec2 = {4.0, 5.0, 6.0};
    
    struct Matrix3x3 rotation_x = {
        .elements = {
            {1.0, 0.0, 0.0},
            {0.0, 0.0, -1.0},
            {0.0, 1.0, 0.0}
        }
    };
    
    struct Matrix3x3 rotation_y = {
        .elements = {
            {0.0, 0.0, 1.0},
            {0.0, 1.0, 0.0},
            {-1.0, 0.0, 0.0}
        }
    };
    
    struct Matrix3x3 composite = matrix_multiply(rotation_y, rotation_x);
    
    printf("Vector 1: (%.2f, %.2f, %.2f)\n", vec1.x, vec1.y, vec1