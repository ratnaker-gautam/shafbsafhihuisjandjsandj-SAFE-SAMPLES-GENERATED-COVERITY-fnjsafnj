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

struct Vector3D vector3d_add(struct Vector3D a, struct Vector3D b) {
    struct Vector3D result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    return result;
}

struct Vector3D vector3d_subtract(struct Vector3D a, struct Vector3D b) {
    struct Vector3D result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    return result;
}

struct Vector3D vector3d_multiply_scalar(struct Vector3D v, double scalar) {
    struct Vector3D result;
    result.x = v.x * scalar;
    result.y = v.y * scalar;
    result.z = v.z * scalar;
    return result;
}

double vector3d_dot(struct Vector3D a, struct Vector3D b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

struct Vector3D vector3d_cross(struct Vector3D a, struct Vector3D b) {
    struct Vector3D result;
    result.x = a.y * b.z - a.z * b.y;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;
    return result;
}

double vector3d_length(struct Vector3D v) {
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

struct Vector3D vector3d_normalize(struct Vector3D v) {
    double len = vector3d_length(v);
    struct Vector3D result = {0.0, 0.0, 0.0};
    if (len > 1e-12) {
        result.x = v.x / len;
        result.y = v.y / len;
        result.z = v.z / len;
    }
    return result;
}

struct Matrix3x3 matrix3x3_multiply(struct Matrix3x3 a, struct Matrix3x3 b) {
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

struct Vector3D matrix3x3_transform_vector(struct Matrix3x3 m, struct Vector3D v) {
    struct Vector3D result;
    result.x = m.elements[0][0] * v.x + m.elements[0][1] * v.y + m.elements[0][2] * v.z;
    result.y = m.elements[1][0] * v.x + m.elements[1][1] * v.y + m.elements[1][2] * v.z;
    result.z = m.elements[2][0] * v.x + m.elements[2][1] * v.y + m.elements[2][2] * v.z;
    return result;
}

struct Vector3D transform_apply(struct Transform t, struct Vector3D v) {
    struct Vector3D rotated = matrix3x3_transform_vector(t.rotation, v);
    struct Vector3D scaled = vector3d_multiply_scalar(rotated, t.scale);
    return vector3d_add(scaled, t.translation);
}

struct BoundingBox compute_bounding_box(struct Mesh* mesh) {
    struct BoundingBox box;
    if (mesh->vertex_count == 0) {
        box.min.x = box.min.y = box.min.z = 0.0;
        box.max.x = box.max.y = box.max.z = 0.0;
        return box;
    }
    box.min = mesh->vertices[0];
    box.max = mesh->vertices[0];
    for (uint32_t i = 1; i < mesh->vertex_count; i++) {
        if (mesh->vertices[i].x < box.min.x) box.min.x = mesh->vertices[i].x;
        if (mesh->vertices[i].y < box.min.y) box.min.y = mesh->vertices[i].y;
        if (mesh->vertices[i].z < box.min.z) box.min.z = mesh->vertices[i].z;
        if (mesh->vertices[i].x > box.max.x) box.max.x = mesh->vertices[i].x;
        if (mesh->vertices[i].y > box.max.y) box.max.y = mesh->vertices[i].y;
        if (mesh->vertices[i].z > box.max.z) box.max.z = mesh->vertices[i].z;
    }
    return box