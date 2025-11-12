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

struct TransformResult apply_transform(struct Vector3D vec, struct Matrix3x3 mat) {
    struct TransformResult res;
    res.original = vec;
    res.transformed = matrix_vector_multiply(mat, vec);
    res.magnitude_original = vector_magnitude(vec);
    res.magnitude_transformed = vector_magnitude(res.transformed);
    return res;
}

void print_vector(struct Vector3D vec) {
    printf("(%.3f, %.3f, %.3f)", vec.x, vec.y, vec.z);
}

void print_matrix(struct Matrix3x3 mat) {
    printf("[%.3f, %.3f, %.3f]\n", mat.elements[0][0], mat.elements[0][1], mat.elements[0][2]);
    printf("[%.3f, %.3f, %.3f]\n", mat.elements[1][0], mat.elements[1][1], mat.elements[1][2]);
    printf("[%.3f, %.3f, %.3f]", mat.elements[2][0], mat.elements[2][1], mat.elements[2][2]);
}

void print_result(struct TransformResult res) {
    printf("Original vector: ");
    print_vector(res.original);
    printf(" (magnitude: %.3f)\n", res.magnitude_original);
    printf("Transformed vector: ");
    print_vector(res.transformed);
    printf(" (magnitude: %.3f)\n", res.magnitude_transformed);
}

int main(void) {
    double angle;
    struct Vector3D vectors[3];
    struct Matrix3x3 rotation_matrix;
    struct TransformResult results[3];
    
    printf("Enter rotation angle in radians: ");
    if (scanf("%lf", &angle) != 1) {
        printf("Invalid input for angle.\n");
        return 1;
    }
    
    printf("Enter 3 vectors (x y z for each):\n");
    for (int i = 0; i < 3; i++) {
        double x, y, z;
        printf("Vector %d: ", i + 1);
        if (scanf("%lf %lf %lf", &x, &y, &z) != 3) {
            printf("Invalid input for vector %d.\n", i + 1);
            return 1;
        }
        vectors[i] = create_vector(x, y, z);
    }
    
    rotation_matrix = create_rotation_matrix(angle);
    
    printf("\nRotation matrix (Z-axis):\n");
    print_matrix(rotation_matrix);
    printf("\n\n");
    
    for (int i = 0; i < 3; i++) {
        results[i] = apply_transform(vectors[i], rotation_matrix);
        printf("Result %d:\n", i + 1);
        print_result(results[i]);
        printf("\n");
    }
    
    return 0;
}