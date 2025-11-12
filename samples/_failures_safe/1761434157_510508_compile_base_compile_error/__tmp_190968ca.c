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

int is_valid_double(double value) {
    return !isnan(value) && !isinf(value);
}

struct Vector3D transform_vector(struct Vector3D vec, struct Matrix3x3 mat) {
    struct Vector3D result;
    result.x = vec.x * mat.elements[0][0] + vec.y * mat.elements[0][1] + vec.z * mat.elements[0][2];
    result.y = vec.x * mat.elements[1][0] + vec.y * mat.elements[1][1] + vec.z * mat.elements[1][2];
    result.z = vec.x * mat.elements[2][0] + vec.y * mat.elements[2][1] + vec.z * mat.elements[2][2];
    return result;
}

double vector_magnitude(struct Vector3D vec) {
    return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

struct TransformResult perform_transformation(struct Vector3D vec, double angle) {
    struct TransformResult result;
    result.original = vec;
    result.valid = 1;
    
    if (!is_valid_double(angle)) {
        result.valid = 0;
        return result;
    }
    
    struct Matrix3x3 rotation = create_rotation_matrix(angle);
    result.transformed = transform_vector(vec, rotation);
    
    if (!is_valid_double(result.transformed.x) || 
        !is_valid_double(result.transformed.y) || 
        !is_valid_double(result.transformed.z)) {
        result.valid = 0;
        return result;
    }
    
    result.magnitude = vector_magnitude(result.transformed);
    if (!is_valid_double(result.magnitude)) {
        result.valid = 0;
    }
    
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
    printf("==============================\n");
    
    if (!read_double("Enter vector X coordinate: ", &input_vector.x)) {
        printf("Error: Invalid X coordinate\n");
        return 1;
    }
    
    if (!read_double("Enter vector Y coordinate: ", &input_vector.y)) {
        printf("Error: Invalid Y coordinate\n");
        return 1;
    }
    
    if (!read_double("Enter vector Z coordinate: ", &input_vector.z)) {
        printf("Error: Invalid Z coordinate\n");
        return 1;
    }
    
    if (!read_double("Enter rotation angle (radians): ", &rotation_angle)) {
        printf("Error: Invalid rotation angle\n");
        return 1;
    }
    
    if (!is_valid_double(input_vector.x) || !is_valid_double(input_vector.y) || 
        !is_valid_double(input_vector.z) || !is_valid_double(rotation_angle)) {
        printf("Error: Invalid numeric input detected\n");
        return 1;
    }
    
    struct TransformResult result = perform_transformation(input_vector, rotation_angle);
    
    if (!result.valid) {
        printf("Error: Transformation produced invalid result\n");
        return 1;
    }
    
    printf("\nTransformation Results:\n");
    printf("Original vector: (%.6f, %.6f, %.6f)\n", 
           result.original.x, result.original.y, result.original.z);
    printf("Transformed vector: (%.6f, %.6f, %.6f)\n", 
           result.transformed.x, result.transformed.y, result.transformed.z);
    printf("Magnitude of transformed vector: %.6f\n", result.magnitude);
    
    return 0;