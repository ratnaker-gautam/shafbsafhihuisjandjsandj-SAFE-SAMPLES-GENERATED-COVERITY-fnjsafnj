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

struct TransformResult perform_transformation(struct Vector3D vec, double angle) {
    struct TransformResult result;
    result.original = vec;
    
    struct Matrix3x3 rot_mat = create_rotation_matrix(angle);
    result.transformed = matrix_vector_multiply(rot_mat, vec);
    result.magnitude = vector_magnitude(vec);
    result.dot_product = dot_product(vec, result.transformed);
    
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
    struct Vector3D vectors[3];
    double angles[3];
    struct TransformResult results[3];
    
    printf("3D Vector Transformation Analysis\n");
    printf("Enter 3 vectors and rotation angles:\n");
    
    for (int i = 0; i < 3; i++) {
        double x, y, z, angle;
        
        printf("\nVector %d:\n", i + 1);
        if (!read_double("  X coordinate: ", &x)) {
            printf("Invalid input for X coordinate\n");
            return 1;
        }
        
        if (!read_double("  Y coordinate: ", &y)) {
            printf("Invalid input for Y coordinate\n");
            return 1;
        }
        
        if (!read_double("  Z coordinate: ", &z)) {
            printf("Invalid input for Z coordinate\n");
            return 1;
        }
        
        if (!read_double("  Rotation angle (radians): ", &angle)) {
            printf("Invalid input for rotation angle\n");
            return 1;
        }
        
        vectors[i] = create_vector(x, y, z);
        angles[i] = angle;
    }
    
    for (int i = 0; i < 3; i++) {
        results[i] = perform_transformation(vectors[i], angles[i]);
    }
    
    printf("\nTransformation Results:\n");
    printf("=======================\n");
    
    for (int i = 0; i < 3; i++) {
        printf("\nVector %d:\n", i + 1);
        printf("  Original: (%.3f, %.3f, %.3f)\n", 
               results[i].original.x, results[i].original.y, results[i].original.z);
        printf("  Transformed: (%.3f, %.3f, %.3f)\n", 
               results[i].transformed.x, results[i].transformed.y, results[i].transformed.z);
        printf("  Magnitude: %.3f\n", results[i].magnitude);
        printf("  Dot product with transformed: %.3f\n", results[i].dot_product);
    }
    
    printf("\nSummary Statistics:\n");
    double total_magnitude = 0.0;