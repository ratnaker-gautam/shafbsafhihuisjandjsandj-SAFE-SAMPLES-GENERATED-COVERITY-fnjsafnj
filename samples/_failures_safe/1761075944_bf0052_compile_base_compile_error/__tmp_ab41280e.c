//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: numeric_computation
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

struct Transform {
    struct Matrix3x3 rotation;
    struct Vector3D translation;
};

struct BoundingBox {
    struct Vector3D min;
    struct Vector3D max;
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

struct Vector3D vector_cross(struct Vector3D a, struct Vector3D b) {
    struct Vector3D result;
    result.x = a.y * b.z - a.z * b.y;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;
    return result;
}

double vector_dot(struct Vector3D a, struct Vector3D b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

double vector_length(struct Vector3D v) {
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

struct Vector3D vector_normalize(struct Vector3D v) {
    struct Vector3D result = {0.0, 0.0, 0.0};
    double len = vector_length(v);
    if (len > 1e-12) {
        result.x = v.x / len;
        result.y = v.y / len;
        result.z = v.z / len;
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

struct Transform transform_combine(struct Transform a, struct Transform b) {
    struct Transform result;
    result.rotation = matrix_multiply(a.rotation, b.rotation);
    result.translation = vector_add(matrix_vector_multiply(a.rotation, b.translation), a.translation);
    return result;
}

struct Vector3D transform_point(struct Transform t, struct Vector3D p) {
    return vector_add(matrix_vector_multiply(t.rotation, p), t.translation);
}

struct BoundingBox transform_bounds(struct Transform t, struct BoundingBox box) {
    struct BoundingBox result;
    struct Vector3D corners[8];
    corners[0] = (struct Vector3D){box.min.x, box.min.y, box.min.z};
    corners[1] = (struct Vector3D){box.min.x, box.min.y, box.max.z};
    corners[2] = (struct Vector3D){box.min.x, box.max.y, box.min.z};
    corners[3] = (struct Vector3D){box.min.x, box.max.y, box.max.z};
    corners[4] = (struct Vector3D){box.max.x, box.min.y, box.min.z};
    corners[5] = (struct Vector3D){box.max.x, box.min.y, box.max.z};
    corners[6] = (struct Vector3D){box.max.x, box.max.y, box.min.z};
    corners[7] = (struct Vector3D){box.max.x, box.max.y, box.max.z};
    
    result.min = transform_point(t, corners[0]);
    result.max = result.min;
    
    for (int i = 1; i < 8; i++) {
        struct Vector3D transformed = transform_point(t, corners[i]);
        if (transformed.x < result.min.x) result.min.x = transformed.x;
        if (transformed.y < result.min.y) result.min.y = transformed.y;
        if (transformed.z < result.min.z) result.min.z = transformed.z;
        if (transformed.x > result.max.x) result.max.x = transformed.x;
        if (