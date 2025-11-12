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
    double magnitude_original;
    double magnitude_transformed;
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

double vector_magnitude(struct Vector3D vec) {
    return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

struct Vector3D matrix_vector_multiply(struct Matrix3x3 mat, struct Vector3D vec) {
    struct Vector3D result;
    result.x = mat.elements[0][0] * vec.x + mat.elements[0][1] * vec.y + mat.elements[0][2] * vec.z;
    result.y = mat.elements[1][0] * vec.x + mat.elements[1][1] * vec.y + mat.elements[1][2] * vec.z;
    result.z = mat.elements[2][0] * vec.x + mat.elements[2][1] * vec.y + mat.elements[2][2] * vec.z;
    return result;
}

int validate_vector(struct Vector3D vec) {
    if (isnan(vec.x) || isnan(vec.y) || isnan(vec.z)) return 0;
    if (isinf(vec.x) || isinf(vec.y) || isinf(vec.z)) return 0;
    return 1;
}

int validate_matrix(struct Matrix3x3 mat) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (isnan(mat.elements[i][j]) || isinf(mat.elements[i][j])) return 0;
        }
    }
    return 1;
}

struct TransformResult perform_transform(struct Vector3D vec, double angle) {
    struct TransformResult result;
    result.original = vec;
    result.magnitude_original = vector_magnitude(vec);
    
    struct Matrix3x3 rot_mat = create_rotation_matrix(angle);
    if (!validate_matrix(rot_mat)) {
        result.transformed = vec;
        result.magnitude_transformed = result.magnitude_original;
        return result;
    }
    
    result.transformed = matrix_vector_multiply(rot_mat, vec);
    if (!validate_vector(result.transformed)) {
        result.transformed = vec;
        result.magnitude_transformed = result.magnitude_original;
        return result;
    }
    
    result.magnitude_transformed = vector_magnitude(result.transformed);
    return result;
}

void print_vector(struct Vector3D vec) {
    printf("(%.3f, %.3f, %.3f)", vec.x, vec.y, vec.z);
}

void print_result(struct TransformResult res) {
    printf("Original vector: ");
    print_vector(res.original);
    printf(" (magnitude: %.3f)\n", res.magnitude_original);
    printf("Transformed vector: ");
    print_vector(res.transformed);
    printf(" (magnitude: %.3f)\n", res.magnitude_transformed);
}

int main() {
    struct Vector3D test_vectors[5];
    test_vectors[0] = create_vector(1.0, 0.0, 0.0);
    test_vectors[1] = create_vector(0.0, 1.0, 0.0);
    test_vectors[2] = create_vector(0.0, 0.0, 1.0);
    test_vectors[3] = create_vector(1.0, 1.0, 1.0);
    test_vectors[4] = create_vector(2.0, -1.0, 0.5);
    
    double angles[3] = {0.0, 3.14159265358979323846 / 4.0, 3.14159265358979323846 / 2.0};
    
    for (int i = 0; i < 5; i++) {
        if (!validate_vector(test_vectors[i])) {
            printf("Invalid test vector %d\n", i);
            continue;
        }
        
        for (int j = 0; j < 3; j++) {
            printf("Test case %d-%d:\n", i, j);
            printf("Rotation angle: %.3f radians\n", angles[j]);
            struct