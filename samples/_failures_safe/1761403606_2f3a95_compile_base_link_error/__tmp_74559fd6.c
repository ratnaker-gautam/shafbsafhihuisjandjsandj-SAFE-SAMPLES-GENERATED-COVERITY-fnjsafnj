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

int validate_angle(double angle) {
    if (isnan(angle) || isinf(angle)) return 0;
    return 1;
}

struct TransformResult perform_transform(struct Vector3D vec, double angle) {
    struct TransformResult result;
    result.original = vec;
    result.magnitude_original = vector_magnitude(vec);
    
    struct Matrix3x3 rot_mat = create_rotation_matrix(angle);
    result.transformed = matrix_vector_multiply(rot_mat, vec);
    result.magnitude_transformed = vector_magnitude(result.transformed);
    
    return result;
}

void print_vector(struct Vector3D vec) {
    printf("(%.3f, %.3f, %.3f)", vec.x, vec.y, vec.z);
}

void print_result(struct TransformResult res) {
    printf("Original vector: ");
    print_vector(res.original);
    printf(" Magnitude: %.3f\n", res.magnitude_original);
    
    printf("Transformed vector: ");
    print_vector(res.transformed);
    printf(" Magnitude: %.3f\n", res.magnitude_transformed);
}

int main() {
    struct Vector3D input_vector;
    double rotation_angle;
    
    printf("Enter vector coordinates (x y z): ");
    if (scanf("%lf %lf %lf", &input_vector.x, &input_vector.y, &input_vector.z) != 3) {
        printf("Error: Invalid vector input\n");
        return 1;
    }
    
    if (!validate_vector(input_vector)) {
        printf("Error: Invalid vector values\n");
        return 1;
    }
    
    printf("Enter rotation angle in radians: ");
    if (scanf("%lf", &rotation_angle) != 1) {
        printf("Error: Invalid angle input\n");
        return 1;
    }
    
    if (!validate_angle(rotation_angle)) {
        printf("Error: Invalid angle value\n");
        return 1;
    }
    
    struct TransformResult result = perform_transform(input_vector, rotation_angle);
    
    printf("\nTransformation Results:\n");
    print_result(result);
    
    return 0;
}