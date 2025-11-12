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

struct Transform {
    struct Matrix3x3 rotation;
    struct Vector3D translation;
    double scale;
};

struct BoundingBox {
    struct Vector3D min;
    struct Vector3D max;
};

struct Mesh {
    struct Vector3D* vertices;
    uint32_t vertex_count;
    struct BoundingBox bounds;
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

struct Transform transform_combine(struct Transform a, struct Transform b) {
    struct Transform result;
    result.rotation = matrix_multiply(a.rotation, b.rotation);
    result.translation = vector_add(matrix_vector_multiply(a.rotation, vector_scale(b.translation, a.scale)), a.translation);
    result.scale = a.scale * b.scale;
    return result;
}

struct Vector3D transform_point(struct Transform t, struct Vector3D point) {
    struct Vector3D rotated = matrix_vector_multiply(t.rotation, point);
    struct Vector3D scaled = vector_scale(rotated, t.scale);
    return vector_add(scaled, t.translation);
}

struct BoundingBox compute_bounds(struct Mesh* mesh) {
    struct BoundingBox bounds;
    if (mesh == NULL || mesh->vertex_count == 0) {
        bounds.min.x = bounds.min.y = bounds.min.z = 0.0;
        bounds.max.x = bounds.max.y = bounds.max.z = 0.0;
        return bounds;
    }
    bounds.min = mesh->vertices[0];
    bounds.max = mesh->vertices[0];
    for (uint32_t i = 1; i < mesh->vertex_count; i++) {
        if (mesh->vertices[i].x < bounds.min.x) bounds.min.x = mesh->vertices[i].x;
        if (mesh->vertices[i].y < bounds.min.y) bounds.min.y = mesh->vertices[i].y;
        if (mesh->vertices[i].z < bounds.min.z) bounds.min.z = mesh->vertices[i].z;
        if (mesh->vertices[i].x > bounds.max.x) bounds.max.x = mesh->vertices[i].x;