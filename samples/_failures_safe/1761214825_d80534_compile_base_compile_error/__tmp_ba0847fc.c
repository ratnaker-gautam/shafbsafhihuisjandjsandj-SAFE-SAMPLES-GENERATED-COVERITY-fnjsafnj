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
    struct Vector3D scale;
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

struct Vector3D vector3d_multiply(struct Vector3D v, double scalar) {
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
    double length = vector3d_length(v);
    struct Vector3D result = {0.0, 0.0, 0.0};
    if (length > 1e-10) {
        result.x = v.x / length;
        result.y = v.y / length;
        result.z = v.z / length;
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

struct Vector3D matrix3x3_transform(struct Matrix3x3 m, struct Vector3D v) {
    struct Vector3D result;
    result.x = m.elements[0][0] * v.x + m.elements[0][1] * v.y + m.elements[0][2] * v.z;
    result.y = m.elements[1][0] * v.x + m.elements[1][1] * v.y + m.elements[1][2] * v.z;
    result.z = m.elements[2][0] * v.x + m.elements[2][1] * v.y + m.elements[2][2] * v.z;
    return result;
}

struct Transform transform_combine(struct Transform a, struct Transform b) {
    struct Transform result;
    result.rotation = matrix3x3_multiply(a.rotation, b.rotation);
    result.translation = vector3d_add(matrix3x3_transform(a.rotation, b.translation), a.translation);
    result.scale.x = a.scale.x * b.scale.x;
    result.scale.y = a.scale.y * b.scale.y;
    result.scale.z = a.scale.z * b.scale.z;
    return result;
}

struct Vector3D transform_apply(struct Transform t, struct Vector3D v) {
    struct Vector3D scaled;
    scaled.x = v.x * t.scale.x;
    scaled.y = v.y * t.scale.y;
    scaled.z = v.z * t.scale.z;
    struct Vector3D rotated = matrix3x3_transform(t.rotation, scaled);
    return vector3d_add(rotated, t.translation);
}

struct BoundingBox boundingbox_calculate(struct Mesh* mesh) {
    struct BoundingBox bounds;
    if (mesh == NULL || mesh->vertex_count == 0) {
        bounds.min.x = bounds.min.y = bounds.min.z = 0.0;
        bounds.max.x = bounds.max.y = bounds.max.z = 0.0;
        return bounds;
    }
    bounds.min.x = bounds.max.x = mesh->vertices[0].x;
    bounds.min.y = bounds.max.y = mesh->vertices[0].y;
    bounds.min.z = bounds.max.z = mesh->vertices[0].z;
    for