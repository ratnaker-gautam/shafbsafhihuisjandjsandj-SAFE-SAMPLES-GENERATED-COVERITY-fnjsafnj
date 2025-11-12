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

int validate_vector_input(const char* input, double* x, double* y, double* z) {
    if (input == NULL) return 0;
    
    char* endptr;
    *x = strtod(input, &endptr);
    if (endptr == input) return 0;
    
    while (*endptr == ' ' || *endptr == ',') endptr++;
    *y = strtod(endptr, &endptr);
    if (endptr == input) return 0;
    
    while (*endptr == ' ' || *endptr == ',') endptr++;
    *z = strtod(endptr, &endptr);
    if (endptr == input) return 0;
    
    return 1;
}

int validate_angle_input(const char* input, double* angle) {
    if (input == NULL) return 0;
    
    char* endptr;
    *angle = strtod(input, &endptr);
    if (endptr == input) return 0;
    
    return 1;
}

int main(void) {
    char input_buffer[256];
    double x, y, z, angle;
    
    printf("Enter vector coordinates (x,y,z): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading vector input\n");
        return 1;
    }
    
    if (!validate_vector_input(input_buffer, &x, &y, &z)) {
        printf("Invalid vector input format\n");
        return 1;
    }
    
    printf("Enter rotation angle in radians: ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading angle input\n");
        return 1;
    }
    
    if (!validate_angle_input(input_buffer, &angle)) {
        printf("Invalid angle input format\n");
        return 1;
    }
    
    struct Vector3D original_vector = create_vector(x, y, z);
    struct Matrix3x3 rotation_matrix = create_rotation_matrix(angle);
    struct Vector3D transformed_vector = matrix_vector_multiply(rotation_matrix, original_vector);
    
    struct TransformResult result;
    result.original = original_vector;
    result.transformed = transformed_vector;
    result.magnitude_original = vector_magnitude(original_vector);
    result.magnitude_transformed = vector_magnitude(transformed_vector);
    
    printf("\nOriginal vector: (%.3f, %.3f, %.3f)\n", 
           result.original.x, result.original.y, result.original.z);
    printf("Transformed vector: (%.3f, %.3f, %.3f)\n", 
           result.transformed.x, result.transformed.y, result.transformed.z);
    printf("Original magnitude: %.3f\n", result.magnitude_original);
    printf("Transformed magnitude: %.3f\n", result.magnitude_transformed);
    
    if (fabs(result.magnitude_original - result.magnitude_transformed) < 1e-10) {
        printf("Rotation preserves vector magnitude (as expected)\n");
    } else {
        printf("Warning: Magnitude changed during transformation\n");
    }
    
    return 0;
}