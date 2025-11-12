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
    double dot_product;
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

double dot_product(struct Vector3D a, struct Vector3D b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

struct Vector3D matrix_vector_multiply(struct Matrix3x3 mat, struct Vector3D vec) {
    struct Vector3D result;
    result.x = mat.elements[0][0] * vec.x + mat.elements[0][1] * vec.y + mat.elements[0][2] * vec.z;
    result.y = mat.elements[1][0] * vec.x + mat.elements[1][1] * vec.y + mat.elements[1][2] * vec.z;
    result.z = mat.elements[2][0] * vec.x + mat.elements[2][1] * vec.y + mat.elements[2][2] * vec.z;
    return result;
}

struct TransformResult perform_transform(struct Vector3D vec, double angle) {
    struct TransformResult result;
    result.original = vec;
    
    struct Matrix3x3 rotation = create_rotation_matrix(angle);
    result.transformed = matrix_vector_multiply(rotation, vec);
    
    result.magnitude_original = vector_magnitude(vec);
    result.magnitude_transformed = vector_magnitude(result.transformed);
    result.dot_product = dot_product(vec, result.transformed);
    
    return result;
}

int validate_input(const char* input, double* value) {
    char* endptr;
    *value = strtod(input, &endptr);
    if (endptr == input || *endptr != '\n') {
        return 0;
    }
    return 1;
}

int main(void) {
    char buffer[256];
    double x, y, z, angle;
    
    printf("Enter vector x coordinate: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    if (!validate_input(buffer, &x)) {
        printf("Invalid x coordinate\n");
        return 1;
    }
    
    printf("Enter vector y coordinate: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    if (!validate_input(buffer, &y)) {
        printf("Invalid y coordinate\n");
        return 1;
    }
    
    printf("Enter vector z coordinate: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    if (!validate_input(buffer, &z)) {
        printf("Invalid z coordinate\n");
        return 1;
    }
    
    printf("Enter rotation angle in radians: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    if (!validate_input(buffer, &angle)) {
        printf("Invalid angle\n");
        return 1;
    }
    
    struct Vector3D input_vector = create_vector(x, y, z);
    struct TransformResult result = perform_transform(input_vector, angle);
    
    printf("\nOriginal vector: (%.6f, %.6f, %.6f)\n", 
           result.original.x, result.original.y, result.original.z);
    printf("Transformed vector: (%.6f, %.6f, %.6f)\n", 
           result.transformed.x, result.transformed.y, result.transformed.z);
    printf("Original magnitude: %.6f\n", result.magnitude_original);
    printf("Transformed magnitude: %.6f\n", result.magnitude_transformed);
    printf("Dot product between vectors: %.6f\n", result.dot_product);
    
    return 0;
}