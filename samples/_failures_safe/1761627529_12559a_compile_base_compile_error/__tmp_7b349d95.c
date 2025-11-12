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

struct Vector3D vector3d_create(double x, double y, double z) {
    struct Vector3D v;
    v.x = x;
    v.y = y;
    v.z = z;
    return v;
}

struct Matrix3x3 matrix3x3_create(double elements[3][3]) {
    struct Matrix3x3 m;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            m.elements[i][j] = elements[i][j];
        }
    }
    return m;
}

int is_finite(double value) {
    return value == value && value <= DBL_MAX && value >= -DBL_MAX;
}

struct Vector3D matrix_vector_multiply(struct Matrix3x3 m, struct Vector3D v) {
    struct Vector3D result = {0, 0, 0};
    for (int i = 0; i < 3; i++) {
        double sum = 0.0;
        for (int j = 0; j < 3; j++) {
            if (!is_finite(m.elements[i][j]) || !is_finite(v.x) || !is_finite(v.y) || !is_finite(v.z)) {
                result.x = result.y = result.z = 0.0;
                return result;
            }
            sum += m.elements[i][j] * (j == 0 ? v.x : (j == 1 ? v.y : v.z));
        }
        if (i == 0) result.x = sum;
        else if (i == 1) result.y = sum;
        else result.z = sum;
    }
    return result;
}

double vector_magnitude(struct Vector3D v) {
    if (!is_finite(v.x) || !is_finite(v.y) || !is_finite(v.z)) {
        return 0.0;
    }
    double sum = v.x * v.x + v.y * v.y + v.z * v.z;
    if (sum > DBL_MAX || sum < -DBL_MAX) {
        return 0.0;
    }
    return sqrt(sum);
}

struct TransformResult transform_vector(struct Matrix3x3 m, struct Vector3D v) {
    struct TransformResult result;
    result.original = v;
    result.transformed = matrix_vector_multiply(m, v);
    result.magnitude = vector_magnitude(result.transformed);
    result.valid = is_finite(result.magnitude) && result.magnitude > 0.0;
    return result;
}

int main(void) {
    double matrix_data[3][3] = {
        {2.0, 0.0, 0.0},
        {0.0, 3.0, 0.0},
        {0.0, 0.0, 4.0}
    };
    
    struct Matrix3x3 transform_matrix = matrix3x3_create(matrix_data);
    
    struct Vector3D test_vectors[5];
    test_vectors[0] = vector3d_create(1.0, 1.0, 1.0);
    test_vectors[1] = vector3d_create(0.0, 5.0, -2.0);
    test_vectors[2] = vector3d_create(3.0, 0.0, 0.0);
    test_vectors[3] = vector3d_create(-1.0, -1.0, -1.0);
    test_vectors[4] = vector3d_create(0.5, 1.5, 2.5);
    
    printf("Vector Transformation Results:\n");
    printf("==============================\n");
    
    for (int i = 0; i < 5; i++) {
        struct TransformResult result = transform_vector(transform_matrix, test_vectors[i]);
        
        printf("Test %d:\n", i + 1);
        printf("Original: (%.2f, %.2f, %.2f)\n", 
               result.original.x, result.original.y, result.original.z);
        printf("Transformed: (%.2f, %.2f, %.2f)\n", 
               result.transformed.x, result.transformed.y, result.transformed.z);
        printf("Magnitude: %.4f\n", result.magnitude);
        printf("Valid: %s\n", result.valid ? "Yes" : "No");
        printf("---\n");
    }
    
    double total_magnitude = 0.0;
    int valid_count = 0;
    
    for (int i = 0; i < 5; i++) {
        struct TransformResult result = transform_vector(transform_matrix, test_vectors[i]);
        if (result.valid) {
            total_magnitude += result.magnitude;
            valid_count++;
        }
    }
    
    if (valid_count > 0) {
        double average_magnitude = total_magnitude / valid_count;
        printf("Average magnitude of valid transformations: %.4f\n", average_magnitude