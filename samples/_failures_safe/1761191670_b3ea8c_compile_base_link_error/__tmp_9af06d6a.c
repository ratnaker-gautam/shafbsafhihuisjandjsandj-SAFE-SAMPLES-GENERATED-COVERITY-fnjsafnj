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

struct TransformResult apply_transform(struct Vector3D vec, double angle) {
    struct TransformResult res;
    res.original = vec;
    res.magnitude_original = vector_magnitude(vec);
    
    struct Matrix3x3 rot_mat = create_rotation_matrix(angle);
    res.transformed = matrix_vector_multiply(rot_mat, vec);
    res.magnitude_transformed = vector_magnitude(res.transformed);
    
    return res;
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
    
    printf("3D Vector Rotation Calculator\n");
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
    
    printf("\nResults:\n");
    printf("========================================\n");
    
    for (int i = 0; i < 3; i++) {
        struct TransformResult result = apply_transform(vectors[i], angles[i]);
        
        printf("Vector %d:\n", i + 1);
        printf("  Original: (%.3f, %.3f, %.3f)\n", 
               result.original.x, result.original.y, result.original.z);
        printf("  Magnitude: %.3f\n", result.magnitude_original);
        printf("  Transformed: (%.3f, %.3f, %.3f)\n", 
               result.transformed.x, result.transformed.y, result.transformed.z);
        printf("  Magnitude: %.3f\n", result.magnitude_transformed);
        printf("  Angle: %.3f radians\n", angles[i]);
        printf("----------------------------------------\n");
    }
    
    return 0;
}