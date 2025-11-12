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

struct Vector3D matrix_vector_multiply(struct Matrix3x3 m, struct Vector3D v) {
    struct Vector3D result;
    result.x = m.elements[0][0] * v.x + m.elements[0][1] * v.y + m.elements[0][2] * v.z;
    result.y = m.elements[1][0] * v.x + m.elements[1][1] * v.y + m.elements[1][2] * v.z;
    result.z = m.elements[2][0] * v.x + m.elements[2][1] * v.y + m.elements[2][2] * v.z;
    return result;
}

int is_finite_vector(struct Vector3D v) {
    return isfinite(v.x) && isfinite(v.y) && isfinite(v.z);
}

struct TransformResult apply_transformation(struct Vector3D v, struct Matrix3x3 m) {
    struct TransformResult result;
    result.original = v;
    result.valid = 0;
    
    if (!is_finite_vector(v)) {
        return result;
    }
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (!isfinite(m.elements[i][j])) {
                return result;
            }
        }
    }
    
    struct Vector3D transformed = matrix_vector_multiply(m, v);
    
    if (!is_finite_vector(transformed)) {
        return result;
    }
    
    double mag = vector_magnitude(transformed);
    if (!isfinite(mag)) {
        return result;
    }
    
    result.transformed = transformed;
    result.magnitude = mag;
    result.valid = 1;
    return result;
}

int main(void) {
    struct Vector3D vec1, vec2;
    struct Matrix3x3 matrix;
    
    printf("Enter first vector (x y z): ");
    if (scanf("%lf %lf %lf", &vec1.x, &vec1.y, &vec1.z) != 3) {
        printf("Invalid vector input\n");
        return 1;
    }
    
    printf("Enter second vector (x y z): ");
    if (scanf("%lf %lf %lf", &vec2.x, &vec2.y, &vec2.z) != 3) {
        printf("Invalid vector input\n");
        return 1;
    }
    
    printf("Enter 3x3 matrix (row by row):\n");
    for (int i = 0; i < 3; i++) {
        printf("Row %d: ", i + 1);
        if (scanf("%lf %lf %lf", &matrix.elements[i][0], &matrix.elements[i][1], &matrix.elements[i][2]) != 3) {
            printf("Invalid matrix input\n");
            return 1;
        }
    }
    
    if (!is_finite_vector(vec1) || !is_finite_vector(vec2)) {
        printf("Invalid vector values\n");
        return 1;
    }
    
    struct Vector3D sum = vector_add(vec1, vec2);
    struct Vector3D diff = vector_subtract(vec1, vec2);
    double dot = vector_dot_product(vec1, vec2);
    struct Vector3D cross = vector_cross_product(vec1, vec2);
    
    if (!is_finite_vector(sum) || !is_finite_vector(diff) || !is_finite_vector(cross) || !isfinite(dot)) {
        printf("Vector operation produced invalid result\n");
        return 1;
    }
    
    struct TransformResult transform1 = apply_transformation(vec1, matrix);
    struct TransformResult transform2 = apply_transformation(vec2, matrix);
    
    printf("\nVector Operations:\n");