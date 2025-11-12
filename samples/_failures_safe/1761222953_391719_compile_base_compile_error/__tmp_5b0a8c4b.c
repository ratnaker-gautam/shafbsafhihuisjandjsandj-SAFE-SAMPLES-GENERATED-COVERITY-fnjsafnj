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

struct Transform {
    struct Matrix3x3 rotation;
    struct Vector3D translation;
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

struct Vector3D matrix_vector_multiply(struct Matrix3x3 m, struct Vector3D v) {
    struct Vector3D result;
    result.x = m.elements[0][0] * v.x + m.elements[0][1] * v.y + m.elements[0][2] * v.z;
    result.y = m.elements[1][0] * v.x + m.elements[1][1] * v.y + m.elements[1][2] * v.z;
    result.z = m.elements[2][0] * v.x + m.elements[2][1] * v.y + m.elements[2][2] * v.z;
    return result;
}

struct Transform transform_inverse(struct Transform t) {
    struct Transform result;
    double det = t.rotation.elements[0][0] * (t.rotation.elements[1][1] * t.rotation.elements[2][2] - t.rotation.elements[1][2] * t.rotation.elements[2][1])
               - t.rotation.elements[0][1] * (t.rotation.elements[1][0] * t.rotation.elements[2][2] - t.rotation.elements[1][2] * t.rotation.elements[2][0])
               + t.rotation.elements[0][2] * (t.rotation.elements[1][0] * t.rotation.elements[2][1] - t.rotation.elements[1][1] * t.rotation.elements[2][0]);
    if (fabs(det) < 1e-12) {
        result.rotation.elements[0][0] = 1.0; result.rotation.elements[0][1] = 0.0; result.rotation.elements[0][2] = 0.0;
        result.rotation.elements[1][0] = 0.0; result.rotation.elements[1][1] = 1.0; result.rotation.elements[1][2] = 0.0;
        result.rotation.elements[2][0] = 0.0; result.rotation.elements[2][1] = 0.0; result.rotation.elements[2][2] = 1.0;
        result.translation.x = 0.0;
        result.translation.y = 0.0;
        result.translation.z = 0.0;
        return result;
    }
    double inv_det = 1.0 / det;
    result.rotation.elements[0][0] = (t.rotation.elements[1][1] * t.rotation.elements[2][2] - t.rotation.elements[1][2] * t.rotation.elements[2][1]) * inv_det;
    result.rotation.elements[0][1] = (t.rotation.elements[0][2] * t.rotation.elements[2][1] - t.rotation.elements[0][1] * t.rotation.elements[2][2]) * inv_det;
    result.rotation.elements[0][2] = (t.rotation.elements[0][1] * t.rotation.elements[1][2] - t.rotation.elements[0][2] * t.rotation.elements[1][1]) * inv_det;
    result.rotation.elements[1][0] = (t.rotation.elements[1][2] * t.rotation.elements[2][0] - t.rotation.elements[1][0] * t.rotation.elements[2][2]) * inv_det;
    result.rotation.elements[1][1] = (t.rotation.elements[0][0] * t.rotation.elements[2][2] - t.rotation.elements[0][2] * t.rotation.elements[2][0]) * inv_det;
    result.rotation.elements[1][2] = (t.rotation.elements[0][2] * t.rotation.elements