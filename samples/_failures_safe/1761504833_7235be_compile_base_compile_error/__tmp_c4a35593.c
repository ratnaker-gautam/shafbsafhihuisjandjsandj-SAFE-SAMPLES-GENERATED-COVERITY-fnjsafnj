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

struct Vector3D create_vector(double x, double y, double z) {
    struct Vector3D vec;
    vec.x = x;
    vec.y = y;
    vec.z = z;
    return vec;
}

struct Matrix3x3 create_rotation_matrix(double angle) {
    struct Matrix3x3 mat;
    double cos_a = cos(angle);
    double sin_a = sin(angle);
    mat.elements[0][0] = cos_a;
    mat.elements[0][1] = -sin_a;
    mat.elements[0][2] = 0.0;
    mat.elements[1][0] = sin_a;
    mat.elements[1][1] = cos_a;
    mat.elements[1][2] = 0.0;
    mat.elements[2][0] = 0.0;
    mat.elements[2][1] = 0.0;
    mat.elements[2][2] = 1.0;
    return mat;
}

int validate_vector(const struct Vector3D* vec) {
    if (vec == NULL) return 0;
    if (isnan(vec->x) || isnan(vec->y) || isnan(vec->z)) return 0;
    if (isinf(vec->x) || isinf(vec->y) || isinf(vec->z)) return 0;
    return 1;
}

struct Vector3D transform_vector(const struct Vector3D* vec, const struct Matrix3x3* mat) {
    struct Vector3D result = {0.0, 0.0, 0.0};
    if (!validate_vector(vec) || mat == NULL) return result;
    
    result.x = vec->x * mat->elements[0][0] + vec->y * mat->elements[0][1] + vec->z * mat->elements[0][2];
    result.y = vec->x * mat->elements[1][0] + vec->y * mat->elements[1][1] + vec->z * mat->elements[1][2];
    result.z = vec->x * mat->elements[2][0] + vec->y * mat->elements[2][1] + vec->z * mat->elements[2][2];
    
    return result;
}

double compute_magnitude(const struct Vector3D* vec) {
    if (!validate_vector(vec)) return 0.0;
    double sum = vec->x * vec->x + vec->y * vec->y + vec->z * vec->z;
    if (sum < 0.0) return 0.0;
    return sqrt(sum);
}

struct TransformResult process_vector(const struct Vector3D* vec, double angle) {
    struct TransformResult result = {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, 0.0, 0};
    
    if (!validate_vector(vec)) return result;
    if (isnan(angle) || isinf(angle)) return result;
    
    struct Matrix3x3 rotation = create_rotation_matrix(angle);
    struct Vector3D transformed = transform_vector(vec, &rotation);
    
    if (!validate_vector(&transformed)) return result;
    
    result.original = *vec;
    result.transformed = transformed;
    result.magnitude = compute_magnitude(&transformed);
    result.valid = 1;
    
    return result;
}

int main(void) {
    struct Vector3D test_vectors[5];
    test_vectors[0] = create_vector(1.0, 0.0, 0.0);
    test_vectors[1] = create_vector(0.0, 1.0, 0.0);
    test_vectors[2] = create_vector(0.0, 0.0, 1.0);
    test_vectors[3] = create_vector(1.0, 1.0, 1.0);
    test_vectors[4] = create_vector(2.5, -1.5, 0.8);
    
    double angles[] = {0.0, 3.14159265358979323846/4.0, 3.14159265358979323846/2.0, 3.14159265358979323846, 2.0*3.14159265358979323846};
    
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            struct TransformResult result = process_vector(&test_vectors[i], angles[j]);
            
            if (result.valid) {
                printf("Vector: (%.3f, %.3f, %.3f) -> ", 
                       result.original.x, result.original.y, result.original.z);
                printf("Transformed: (%.3f, %.3f, %.3f) ", 
                       result.transformed.x, result.transformed.y, result.transformed.z);
                printf("Magnitude: %.3f\n", result.magnitude);
            } else {
                printf("