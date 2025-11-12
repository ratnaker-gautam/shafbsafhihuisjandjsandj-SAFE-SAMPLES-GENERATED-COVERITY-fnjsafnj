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

struct Vector3D vector_add(struct Vector3D a, struct Vector3D b) {
    struct Vector3D result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    return result;
}

struct Vector3D vector_subtract(struct Vector3D a, struct Vector3D b) {
    struct Vector3D result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    return result;
}

double vector_dot_product(struct Vector3D a, struct Vector3D b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

struct Vector3D vector_cross_product(struct Vector3D a, struct Vector3D b) {
    struct Vector3D result;
    result.x = a.y * b.z - a.z * b.y;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;
    return result;
}

double vector_magnitude(struct Vector3D v) {
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

struct Matrix3x3 matrix_multiply(struct Matrix3x3 a, struct Matrix3x3 b) {
    struct Matrix3x3 result;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result.elements[i][j] = 0.0;
            for (int k = 0; k < 3; k++) {
                result.elements[i][j] += a.elements[i][k] * b.elements[k][j];
            }
        }
    }
    return result;
}

struct Vector3D matrix_vector_multiply(struct Matrix3x3 m, struct Vector3D v) {
    struct Vector3D result;
    result.x = m.elements[0][0] * v.x + m.elements[0][1] * v.y + m.elements[0][2] * v.z;
    result.y = m.elements[1][0] * v.x + m.elements[1][1] * v.y + m.elements[1][2] * v.z;
    result.z = m.elements[2][0] * v.x + m.elements[2][1] * v.y + m.elements[2][2] * v.z;
    return result;
}

struct TransformResult apply_transformation(struct Vector3D v, struct Matrix3x3 m) {
    struct TransformResult result;
    result.original = v;
    result.transformed = matrix_vector_multiply(m, v);
    result.magnitude_original = vector_magnitude(v);
    result.magnitude_transformed = vector_magnitude(result.transformed);
    return result;
}

int main(void) {
    struct Vector3D vec1, vec2;
    struct Matrix3x3 mat1, mat2;
    
    printf("Enter first vector (x y z): ");
    if (scanf("%lf %lf %lf", &vec1.x, &vec1.y, &vec1.z) != 3) {
        printf("Invalid input for first vector\n");
        return 1;
    }
    
    printf("Enter second vector (x y z): ");
    if (scanf("%lf %lf %lf", &vec2.x, &vec2.y, &vec2.z) != 3) {
        printf("Invalid input for second vector\n");
        return 1;
    }
    
    printf("Enter first 3x3 matrix (row by row):\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (scanf("%lf", &mat1.elements[i][j]) != 1) {
                printf("Invalid input for matrix\n");
                return 1;
            }
        }
    }
    
    printf("Enter second 3x3 matrix (row by row):\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (scanf("%lf", &mat2.elements[i][j]) != 1) {
                printf("Invalid input for matrix\n");
                return 1;
            }
        }
    }
    
    struct Vector3D sum = vector_add(vec1, vec2);
    struct Vector3D diff = vector_subtract(vec1, vec2);
    double dot = vector_dot_product(vec1, vec2);
    struct Vector3D cross = vector_cross_product(vec1, vec2);
    struct Matrix3x3 mat_product = matrix_multiply(mat1, mat2);
    struct TransformResult transform1 = apply_transformation(vec1, mat1);
    struct TransformResult transform2 = apply_transformation(vec2, mat2);
    
    printf("\nVector Results:\n");
    printf("Vector 1: (%.3f, %.3f