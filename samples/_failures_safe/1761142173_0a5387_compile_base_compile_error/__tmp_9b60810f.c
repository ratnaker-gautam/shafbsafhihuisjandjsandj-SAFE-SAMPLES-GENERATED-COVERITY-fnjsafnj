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

struct GeometricTransform {
    struct Matrix3x3 rotation;
    struct Vector3D translation;
    double scale;
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

struct Vector3D vector_scale(struct Vector3D v, double s) {
    struct Vector3D result;
    result.x = v.x * s;
    result.y = v.y * s;
    result.z = v.z * s;
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

double vector_length(struct Vector3D v) {
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

struct Vector3D vector_normalize(struct Vector3D v) {
    double len = vector_length(v);
    struct Vector3D result = {0.0, 0.0, 0.0};
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

struct GeometricTransform transform_point(struct GeometricTransform t, struct Vector3D point) {
    struct GeometricTransform result;
    struct Vector3D rotated = matrix_vector_multiply(t.rotation, point);
    struct Vector3D scaled = vector_scale(rotated, t.scale);
    result.translation = vector_add(scaled, t.translation);
    result.rotation = t.rotation;
    result.scale = t.scale;
    return result;
}

struct BoundingBox calculate_bounding_box(struct Vector3D points[], int count) {
    struct BoundingBox box;
    if (count <= 0) {
        box.min.x = box.min.y = box.min.z = 0.0;
        box.max.x = box.max.y = box.max.z = 0.0;
        return box;
    }
    box.min = points[0];
    box.max = points[0];
    for (int i = 1; i < count; i++) {
        if (points[i].x < box.min.x) box.min.x = points[i].x;
        if (points[i].y < box.min.y) box.min.y = points[i].y;
        if (points[i].z < box.min.z) box.min.z = points[i].z;
        if (points[i].x > box.max.x) box.max.x = points[i].x;
        if (points[i].y > box.max.y) box.max.y = points[i].y;
        if (points[i].z > box.max.z) box.max.z = points[i].z;
    }
    return box;
}

double calculate_volume(struct BoundingBox box) {
    double dx = box.max.x - box.min.x;
    double dy = box.max.y - box.min.y;
    double dz = box.max.z - box.min.z;
    if (dx < 0.0