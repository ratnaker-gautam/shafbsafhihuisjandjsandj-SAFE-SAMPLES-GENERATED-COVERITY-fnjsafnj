//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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

int is_finite(double value) {
    return value == value && value <= 1.0 / 0.0 && value >= -1.0 / 0.0;
}

struct Vector3D transform_vector(struct Vector3D vec, struct Matrix3x3 mat) {
    struct Vector3D result;
    result.x = vec.x * mat.elements[0][0] + vec.y * mat.elements[0][1] + vec.z * mat.elements[0][2];
    result.y = vec.x * mat.elements[1][0] + vec.y * mat.elements[1][1] + vec.z * mat.elements[1][2];
    result.z = vec.x * mat.elements[2][0] + vec.y * mat.elements[2][1] + vec.z * mat.elements[2][2];
    return result;
}

double vector_magnitude(struct Vector3D vec) {
    double sum = vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
    if (sum < 0.0 || !is_finite(sum)) {
        return -1.0;
    }
    return sqrt(sum);
}

struct TransformResult process_vector(struct Vector3D vec, double angle) {
    struct TransformResult result;
    result.original = vec;
    result.valid = 1;
    
    if (!is_finite(angle) || angle < -6.28318530718 || angle > 6.28318530718) {
        result.valid = 0;
        return result;
    }
    
    if (!is_finite(vec.x) || !is_finite(vec.y) || !is_finite(vec.z)) {
        result.valid = 0;
        return result;
    }
    
    struct Matrix3x3 rot_mat = create_rotation_matrix(angle);
    result.transformed = transform_vector(vec, rot_mat);
    
    if (!is_finite(result.transformed.x) || !is_finite(result.transformed.y) || !is_finite(result.transformed.z)) {
        result.valid = 0;
        return result;
    }
    
    result.magnitude = vector_magnitude(result.transformed);
    if (result.magnitude < 0.0) {
        result.valid = 0;
    }
    
    return result;
}

int main(void) {
    struct Vector3D test_vectors[5];
    double angles[5];
    
    test_vectors[0] = create_vector(1.0, 0.0, 0.0);
    test_vectors[1] = create_vector(0.0, 1.0, 0.0);
    test_vectors[2] = create_vector(0.0, 0.0, 1.0);
    test_vectors[3] = create_vector(1.0, 1.0, 1.0);
    test_vectors[4] = create_vector(2.5, -1.5, 0.8);
    
    angles[0] = 0.0;
    angles[1] = 1.57079632679;
    angles[2] = 3.14159265359;
    angles[3] = -1.0471975512;
    angles[4] = 0.785398163397;
    
    printf("Vector Transformation Results:\n");
    printf("==============================\n");
    
    for (int i = 0; i < 5; i++) {
        struct TransformResult result = process_vector(test_vectors[i], angles[i]);
        
        if (!result.valid) {
            printf("Result %d: INVALID TRANSFORMATION\n", i + 1);
            continue;
        }
        
        printf("Result %d:\n", i + 1);
        printf("  Original: (%.3f, %.3f, %.3f)\n", 
               result.original.x, result.original.y, result.original.z);
        printf("  Transformed: (%.3f, %.3f, %.3f)\n", 
               result.transformed.x, result.transformed.y, result.transformed.z);
        printf("  Magnitude: %.3f\n", result.magn