//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: numeric_computation
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

struct TransformResult apply_transform(struct Vector3D vec, struct Matrix3x3 mat) {
    struct TransformResult result;
    result.original = vec;
    result.transformed = matrix_vector_multiply(mat, vec);
    result.magnitude_original = vector_magnitude(vec);
    result.magnitude_transformed = vector_magnitude(result.transformed);
    return result;
}

int read_double(const char* prompt, double* value) {
    char buffer[256];
    char* endptr;
    
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    
    *value = strtod(buffer, &endptr);
    if (endptr == buffer) {
        return 0;
    }
    
    return 1;
}

int main(void) {
    struct Vector3D input_vector;
    double rotation_angle;
    
    printf("3D Vector Rotation Calculator\n");
    printf("=============================\n\n");
    
    printf("Enter vector coordinates:\n");
    if (!read_double("X: ", &input_vector.x)) {
        printf("Error: Invalid input for X coordinate\n");
        return 1;
    }
    
    if (!read_double("Y: ", &input_vector.y)) {
        printf("Error: Invalid input for Y coordinate\n");
        return 1;
    }
    
    if (!read_double("Z: ", &input_vector.z)) {
        printf("Error: Invalid input for Z coordinate\n");
        return 1;
    }
    
    if (!read_double("Enter rotation angle in radians: ", &rotation_angle)) {
        printf("Error: Invalid input for rotation angle\n");
        return 1;
    }
    
    if (rotation_angle < -6.28318530718 || rotation_angle > 6.28318530718) {
        printf("Warning: Angle outside [-2π, 2π] range\n");
    }
    
    struct Matrix3x3 rotation_matrix = create_rotation_matrix(rotation_angle);
    struct TransformResult result = apply_transform(input_vector, rotation_matrix);
    
    printf("\nResults:\n");
    printf("Original vector: (%.6f, %.6f, %.6f)\n", 
           result.original.x, result.original.y, result.original.z);
    printf("Original magnitude: %.6f\n", result.magnitude_original);
    printf("Transformed vector: (%.6f, %.6f, %.6f)\n", 
           result.transformed.x, result.transformed.y, result.transformed.z);
    printf("Transformed magnitude: %.6f\n", result.magnitude_transformed);
    
    double magnitude_diff = fabs(result.magnitude_original - result.magnitude_transformed);
    if (magnitude_diff > 1e-10) {
        printf("Warning: Magnitude changed by %.10f (rotation should preserve magnitude)\n", magnitude_diff);
    }
    
    return 0;
}