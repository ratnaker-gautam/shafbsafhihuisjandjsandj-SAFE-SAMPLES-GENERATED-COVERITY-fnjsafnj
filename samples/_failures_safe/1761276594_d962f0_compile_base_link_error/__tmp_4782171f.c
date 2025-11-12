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

struct Vector3D matrix_vector_multiply(struct Matrix3x3 mat, struct Vector3D vec) {
    struct Vector3D result;
    result.x = mat.elements[0][0] * vec.x + mat.elements[0][1] * vec.y + mat.elements[0][2] * vec.z;
    result.y = mat.elements[1][0] * vec.x + mat.elements[1][1] * vec.y + mat.elements[1][2] * vec.z;
    result.z = mat.elements[2][0] * vec.x + mat.elements[2][1] * vec.y + mat.elements[2][2] * vec.z;
    return result;
}

double vector_magnitude(struct Vector3D vec) {
    return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

struct TransformResult transform_vector(struct Vector3D vec, double angle) {
    struct TransformResult result;
    result.original = vec;
    result.valid = 1;
    
    if (angle < -6.28318530718 || angle > 6.28318530718) {
        result.valid = 0;
        return result;
    }
    
    struct Matrix3x3 rot_mat = create_rotation_matrix(angle);
    result.transformed = matrix_vector_multiply(rot_mat, vec);
    result.magnitude = vector_magnitude(result.transformed);
    
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
    if (endptr == buffer) {
        return 0;
    }
    return 1;
}

int main(void) {
    double x, y, z, angle;
    
    printf("3D Vector Rotation Calculator\n");
    printf("Enter vector coordinates and rotation angle:\n");
    
    if (!read_double("X coordinate: ", &x)) {
        printf("Invalid X coordinate input.\n");
        return 1;
    }
    
    if (!read_double("Y coordinate: ", &y)) {
        printf("Invalid Y coordinate input.\n");
        return 1;
    }
    
    if (!read_double("Z coordinate: ", &z)) {
        printf("Invalid Z coordinate input.\n");
        return 1;
    }
    
    if (!read_double("Rotation angle (radians, -2π to 2π): ", &angle)) {
        printf("Invalid angle input.\n");
        return 1;
    }
    
    struct Vector3D input_vector = create_vector(x, y, z);
    struct TransformResult result = transform_vector(input_vector, angle);
    
    if (!result.valid) {
        printf("Error: Angle must be between -2π and 2π radians.\n");
        return 1;
    }
    
    printf("\nResults:\n");
    printf("Original vector: (%.3f, %.3f, %.3f)\n", 
           result.original.x, result.original.y, result.original.z);
    printf("Transformed vector: (%.3f, %.3f, %.3f)\n", 
           result.transformed.x, result.transformed.y, result.transformed.z);
    printf("Magnitude of transformed vector: %.3f\n", result.magnitude);
    
    return 0;
}