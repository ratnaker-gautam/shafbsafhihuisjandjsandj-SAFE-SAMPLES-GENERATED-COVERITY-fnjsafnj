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
    double magnitude_original;
    double magnitude_transformed;
    double dot_product;
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

double vector_magnitude(struct Vector3D v) {
    double sum = v.x * v.x + v.y * v.y + v.z * v.z;
    if (sum <= 0.0) return 0.0;
    return sqrt(sum);
}

struct Vector3D matrix_vector_multiply(struct Matrix3x3 m, struct Vector3D v) {
    struct Vector3D result;
    result.x = m.elements[0][0] * v.x + m.elements[0][1] * v.y + m.elements[0][2] * v.z;
    result.y = m.elements[1][0] * v.x + m.elements[1][1] * v.y + m.elements[1][2] * v.z;
    result.z = m.elements[2][0] * v.x + m.elements[2][1] * v.y + m.elements[2][2] * v.z;
    return result;
}

int validate_vector_input(double x, double y, double z) {
    if (x != x || y != y || z != z) return 0;
    if (x > 1e100 || x < -1e100) return 0;
    if (y > 1e100 || y < -1e100) return 0;
    if (z > 1e100 || z < -1e100) return 0;
    return 1;
}

int validate_matrix_input(struct Matrix3x3 m) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (m.elements[i][j] != m.elements[i][j]) return 0;
            if (m.elements[i][j] > 1e100 || m.elements[i][j] < -1e100) return 0;
        }
    }
    return 1;
}

struct TransformResult compute_transformation(struct Vector3D v, struct Matrix3x3 m) {
    struct TransformResult result;
    result.original = v;
    result.transformed = matrix_vector_multiply(m, v);
    result.magnitude_original = vector_magnitude(v);
    result.magnitude_transformed = vector_magnitude(result.transformed);
    result.dot_product = vector_dot_product(v, result.transformed);
    return result;
}

int main(void) {
    struct Vector3D vector1, vector2;
    struct Matrix3x3 transform_matrix;
    
    printf("Enter first vector (x y z): ");
    if (scanf("%lf %lf %lf", &vector1.x, &vector1.y, &vector1.z) != 3) {
        printf("Invalid vector input\n");
        return 1;
    }
    if (!validate_vector_input(vector1.x, vector1.y, vector1.z)) {
        printf("Invalid vector values\n");
        return 1;
    }
    
    printf("Enter second vector (x y z): ");
    if (scanf("%lf %lf %lf", &vector2.x, &vector2.y, &vector2.z) != 3) {
        printf("Invalid vector input\n");
        return 1;
    }
    if (!validate_vector_input(vector2.x, vector2.y, vector2.z)) {
        printf("Invalid vector values\n");
        return 1;
    }
    
    printf("Enter 3x3 transformation matrix (9 numbers, row-major):\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (scanf("%lf", &transform_matrix.elements[i][j]) != 1) {
                printf("Invalid matrix input\n");
                return 1;
            }
        }
    }
    if (!validate_matrix_input(transform_matrix)) {
        printf("Invalid matrix values\n");
        return 1;
    }
    
    struct Vector3D sum = vector_add(vector1, vector2);
    struct Vector3D difference = vector_subtract(vector1, vector2);
    double dot = vector_dot_product(vector1, vector2);
    
    struct TransformResult transform1 = compute_transformation(vector1, transform_matrix);
    struct TransformResult transform2 = compute_transformation(vector2, transform_matrix);
    
    printf("\nVector Operations:\n");