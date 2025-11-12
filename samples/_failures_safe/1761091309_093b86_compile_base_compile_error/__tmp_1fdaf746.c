//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: numeric_computation
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

int validate_vector_input(double x, double y, double z) {
    if (x != x || y != y || z != z) return 0;
    if (x > 1e100 || x < -1e100) return 0;
    if (y > 1e100 || y < -1e100) return 0;
    if (z > 1e100 || z < -1e100) return 0;
    return 1;
}

struct TransformResult perform_vector_operations(struct Vector3D v1, struct Vector3D v2, struct Matrix3x3 m) {
    struct TransformResult result;
    result.original = v1;
    result.valid = 1;
    
    struct Vector3D sum = vector_add(v1, v2);
    struct Vector3D diff = vector_subtract(v1, v2);
    double dot = vector_dot_product(v1, v2);
    struct Vector3D cross = vector_cross_product(v1, v2);
    
    struct Vector3D transformed = matrix_vector_multiply(m, v1);
    
    if (transformed.x != transformed.x || transformed.y != transformed.y || transformed.z != transformed.z) {
        result.valid = 0;
        return result;
    }
    
    result.transformed = transformed;
    result.magnitude = vector_magnitude(transformed);
    
    if (result.magnitude != result.magnitude) {
        result.valid = 0;
        return result;
    }
    
    printf("Vector 1: (%.3f, %.3f, %.3f)\n", v1.x, v1.y, v1.z);
    printf("Vector 2: (%.3f, %.3f, %.3f)\n", v2.x, v2.y, v2.z);
    printf("Sum: (%.3f, %.3f, %.3f)\n", sum.x, sum.y, sum.z);
    printf("Difference: (%.3f, %.3f, %.3f)\n", diff.x, diff.y, diff.z);
    printf("Dot product: %.3f\n", dot);
    printf("Cross product: (%.3f, %.3f, %.3f)\n", cross.x, cross.y, cross.z);
    printf("Transformed vector: (%.3f, %.3f, %.3f)\n", transformed.x, transformed.y, transformed.z);
    printf("Magnitude of transformed vector: %.3f\n", result.magnitude);
    
    return result;
}

int main