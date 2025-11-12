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

struct Vector3D vector_normalize(struct Vector3D v) {
    struct Vector3D result = {0.0, 0.0, 0.0};
    double mag = vector_magnitude(v);
    if (mag > 1e-12) {
        result.x = v.x / mag;
        result.y = v.y / mag;
        result.z = v.z / mag;
    }
    return result;
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

struct TransformResult apply_transformation(struct Matrix3x3 transform, struct Vector3D input) {
    struct TransformResult result;
    result.original = input;
    result.transformed = matrix_vector_multiply(transform, input);
    result.magnitude = vector_magnitude(result.transformed);
    result.valid = (result.magnitude < 1e12) ? 1 : 0;
    return result;
}

int read_double(double *value) {
    char buffer[64];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    return (sscanf(buffer, "%lf", value) == 1);
}

int main(void) {
    struct Vector3D vec1, vec2;
    struct Matrix3x3 rotation, scaling, composite;
    
    printf("Enter first vector (x y z): ");
    if (!read_double(&vec1.x) || !read_double(&vec1.y) || !read_double(&vec1.z)) {
        printf("Invalid input for first vector\n");
        return 1;
    }
    
    printf("Enter second vector (x y z): ");
    if (!read_double(&vec2.x) || !read_double(&vec2.y) || !read_double(&vec2.z)) {
        printf("Invalid input for second vector\n");
        return 1;
    }
    
    printf("\nVector operations:\n");
    struct Vector3D sum = vector_add(vec1, vec2);
    printf("Sum: (%.3f, %.3f, %.3f)\n", sum.x, sum.y, sum.z);
    
    struct Vector3D diff = vector_subtract(vec1, vec2);
    printf("Difference: (%.3f, %.3f, %.3f)\n", diff.x, diff.y, diff.z);
    
    double dot = vector_dot_product(vec1, vec2);
    printf("Dot product: %.3f\n", dot);
    
    struct Vector3D cross = vector_cross_product(vec1, vec2);
    printf("Cross product: (%.3f, %.3f,