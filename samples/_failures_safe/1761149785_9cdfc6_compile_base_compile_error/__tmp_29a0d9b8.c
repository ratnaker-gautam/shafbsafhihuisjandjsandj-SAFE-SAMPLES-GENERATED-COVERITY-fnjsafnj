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

int is_valid_vector(const struct Vector3D* vec) {
    if (vec == NULL) return 0;
    if (isnan(vec->x) || isnan(vec->y) || isnan(vec->z)) return 0;
    if (isinf(vec->x) || isinf(vec->y) || isinf(vec->z)) return 0;
    return 1;
}

int is_valid_matrix(const struct Matrix3x3* mat) {
    if (mat == NULL) return 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (isnan(mat->elements[i][j]) || isinf(mat->elements[i][j])) {
                return 0;
            }
        }
    }
    return 1;
}

struct Vector3D transform_vector(const struct Vector3D* vec, const struct Matrix3x3* mat) {
    struct Vector3D result = {0.0, 0.0, 0.0};
    if (!is_valid_vector(vec) || !is_valid_matrix(mat)) {
        return result;
    }
    result.x = vec->x * mat->elements[0][0] + vec->y * mat->elements[0][1] + vec->z * mat->elements[0][2];
    result.y = vec->x * mat->elements[1][0] + vec->y * mat->elements[1][1] + vec->z * mat->elements[1][2];
    result.z = vec->x * mat->elements[2][0] + vec->y * mat->elements[2][1] + vec->z * mat->elements[2][2];
    return result;
}

double compute_magnitude(const struct Vector3D* vec) {
    if (!is_valid_vector(vec)) return 0.0;
    double sum = vec->x * vec->x + vec->y * vec->y + vec->z * vec->z;
    if (sum < 0.0) return 0.0;
    return sqrt(sum);
}

struct TransformResult process_transformation(const struct Vector3D* vec, double angle) {
    struct TransformResult result;
    result.valid = 0;
    result.original = create_vector(0.0, 0.0, 0.0);
    result.transformed = create_vector(0.0, 0.0, 0.0);
    result.magnitude = 0.0;
    
    if (!is_valid_vector(vec)) {
        return result;
    }
    if (isnan(angle) || isinf(angle)) {
        return result;
    }
    
    result.original = *vec;
    struct Matrix3x3 rotation_mat = create_rotation_matrix(angle);
    if (!is_valid_matrix(&rotation_mat)) {
        return result;
    }
    
    result.transformed = transform_vector(vec, &rotation_mat);
    if (!is_valid_vector(&result.transformed)) {
        return result;
    }
    
    result.magnitude = compute_magnitude(&result.transformed);
    if (isnan(result.magnitude) || isinf(result.magnitude)) {
        return result;
    }
    
    result.valid = 1;
    return result;
}

int main() {
    struct Vector3D input_vector;
    double rotation_angle;
    
    printf("Enter vector coordinates (x y z): ");
    if (scanf("%lf %lf %lf", &input_vector.x, &input_vector.y, &input_vector.z) != 3) {
        printf("Invalid vector input.\n");
        return 1;
    }
    
    printf("Enter rotation angle in radians: ");
    if (scanf("%lf", &rotation_angle) != 1) {
        printf("Invalid angle input.\n");
        return 1;
    }
    
    struct TransformResult result = process_transformation(&input_vector, rotation_angle);
    
    if (!result.valid) {
        printf("Transformation failed due to invalid input or computation error.\n");
        return 1;
    }
    
    printf("Original vector: (%f, %