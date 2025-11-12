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
    double scale_factor;
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
    double sum = vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
    if (sum < 0.0) return 0.0;
    return sqrt(sum);
}

struct Vector3D matrix_vector_multiply(struct Matrix3x3 mat, struct Vector3D vec) {
    struct Vector3D result;
    result.x = mat.elements[0][0] * vec.x + mat.elements[0][1] * vec.y + mat.elements[0][2] * vec.z;
    result.y = mat.elements[1][0] * vec.x + mat.elements[1][1] * vec.y + mat.elements[1][2] * vec.z;
    result.z = mat.elements[2][0] * vec.x + mat.elements[2][1] * vec.y + mat.elements[2][2] * vec.z;
    return result;
}

struct TransformResult transform_vector(struct Vector3D vec, double angle) {
    struct TransformResult result;
    result.original = vec;
    
    struct Matrix3x3 rotation_mat = create_rotation_matrix(angle);
    result.transformed = matrix_vector_multiply(rotation_mat, vec);
    
    result.magnitude_original = vector_magnitude(vec);
    result.magnitude_transformed = vector_magnitude(result.transformed);
    
    if (result.magnitude_original > 0.0) {
        result.scale_factor = result.magnitude_transformed / result.magnitude_original;
    } else {
        result.scale_factor = 1.0;
    }
    
    return result;
}

int read_double(const char* prompt, double* value) {
    char buffer[256];
    printf("%s", prompt);
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    
    char* endptr;
    *value = strtod(buffer, &endptr);
    
    if (endptr == buffer || *endptr != '\n') {
        return 0;
    }
    
    if (!isfinite(*value)) {
        return 0;
    }
    
    return 1;
}

int main(void) {
    struct Vector3D input_vector;
    double rotation_angle;
    
    printf("3D Vector Rotation Transformation\n");
    printf("=================================\n\n");
    
    printf("Enter vector coordinates and rotation angle:\n");
    
    if (!read_double("X coordinate: ", &input_vector.x)) {
        printf("Error: Invalid X coordinate input.\n");
        return 1;
    }
    
    if (!read_double("Y coordinate: ", &input_vector.y)) {
        printf("Error: Invalid Y coordinate input.\n");
        return 1;
    }
    
    if (!read_double("Z coordinate: ", &input_vector.z)) {
        printf("Error: Invalid Z coordinate input.\n");
        return 1;
    }
    
    if (!read_double("Rotation angle (radians): ", &rotation_angle)) {
        printf("Error: Invalid rotation angle input.\n");
        return 1;
    }
    
    if (!isfinite(rotation_angle)) {
        printf("Error: Rotation angle must be finite.\n");
        return 1;
    }
    
    struct TransformResult result = transform_vector(input_vector, rotation_angle);
    
    printf("\nTransformation Results:\n");
    printf("Original vector: (%.6f, %.6f, %.6f)\n", 
           result.original.x, result.original.y, result.original.z);
    printf("Original magnitude: %.6f\n", result.magnitude_original);
    printf("Transformed vector: (%.6f, %.6f, %.6f)\n", 
           result.transformed.x, result.transformed.y, result.transformed.z);
    printf("Transformed magnitude: %.6f\n", result.magnitude_transformed);
    printf("Scale factor: %.6f\n", result.scale_factor);
    
    if (fabs(result.scale_factor - 1.0) < 1e-10) {
        printf