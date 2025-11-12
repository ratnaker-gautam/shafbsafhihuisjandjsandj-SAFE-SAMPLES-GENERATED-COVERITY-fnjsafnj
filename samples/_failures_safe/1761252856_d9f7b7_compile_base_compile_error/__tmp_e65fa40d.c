//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>
#include <float.h>

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
    double magnitude_ratio;
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

double vector_dot(struct Vector3D a, struct Vector3D b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

struct Vector3D vector_cross(struct Vector3D a, struct Vector3D b) {
    struct Vector3D result;
    result.x = a.y * b.z - a.z * b.y;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;
    return result;
}

double vector_magnitude(struct Vector3D v) {
    double sum = v.x * v.x + v.y * v.y + v.z * v.z;
    if (sum < 0.0 || sum > DBL_MAX) {
        return 0.0;
    }
    return sqrt(sum);
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

struct TransformResult transform_vector(struct Vector3D v, struct Matrix3x3 m) {
    struct TransformResult result;
    result.original = v;
    result.transformed = matrix_vector_multiply(m, v);
    double orig_mag = vector_magnitude(v);
    double trans_mag = vector_magnitude(result.transformed);
    if (orig_mag == 0.0 || trans_mag == 0.0) {
        result.magnitude_ratio = 0.0;
        result.valid = 0;
    } else {
        result.magnitude_ratio = trans_mag / orig_mag;
        result.valid = 1;
    }
    return result;
}

int read_double(double *value) {
    char buffer[64];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    char *endptr;
    *value = strtod(buffer, &endptr);
    return (endptr != buffer);
}

int main(void) {
    struct Vector3D vectors[3];
    struct Matrix3x3 transform;
    
    printf("Enter 3 vectors (x y z for each):\n");
    for (int i = 0; i < 3; i++) {
        printf("Vector %d: ", i + 1);
        if (!read_double(&vectors[i].x) || !read_double(&vectors[i].y) || !read_double(&vectors[i].z)) {
            printf("Invalid input\n");
            return 1;
        }
    }
    
    printf("Enter 3x3 transformation matrix (row by row):\n");
    for (int i = 0; i < 3; i++) {
        printf("Row %d: ", i + 1);
        for (int j = 0; j < 3; j++) {
            if (!read_double(&transform.elements[i][j])) {
                printf("Invalid input\n");
                return 1;
            }
        }
    }
    
    printf("\nVector operations:\n");
    for (int i = 0; i < 3; i++) {
        for (int j = i + 1; j < 3; j++) {
            struct Vector3D sum = vector_add(vectors[i], vectors[j]);
            struct Vector3D diff = vector_subtract(vectors[i], vectors[j]);