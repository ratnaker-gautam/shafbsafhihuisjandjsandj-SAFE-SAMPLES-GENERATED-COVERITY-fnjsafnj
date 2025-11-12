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

struct Transform {
    struct Matrix3x3 rotation;
    struct Vector3D translation;
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

struct Vector3D vector3d_scale(struct Vector3D v, double scalar) {
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

struct Vector3D transform_apply(struct Transform t, struct Vector3D v) {
    struct Vector3D rotated = matrix3x3_transform(t.rotation, v);
    return vector3d_add(rotated, t.translation);
}

struct Matrix3x3 matrix3x3_rotation_x(double angle) {
    struct Matrix3x3 result;
    double c = cos(angle);
    double s = sin(angle);
    result.elements[0][0] = 1.0; result.elements[0][1] = 0.0; result.elements[0][2] = 0.0;
    result.elements[1][0] = 0.0; result.elements[1][1] = c;   result.elements[1][2] = -s;
    result.elements[2][0] = 0.0; result.elements[2][1] = s;   result.elements[2][2] = c;
    return result;
}

struct Matrix3x3 matrix3x3_rotation_y(double angle) {
    struct Matrix3x3 result;
    double c = cos(angle);
    double s = sin(angle);
    result.elements[0][0] = c;   result.elements[0][1] = 0.0; result.elements[0][2] = s;
    result.elements[1][0] = 0.0; result.elements[1][1] = 1.0; result.elements[1][2] = 0.0;
    result.elements[2][0] = -s;  result.elements[2][1] = 0.0; result.elements[2][2] = c;
    return result;
}

struct Matrix3x3 matrix3x3_rotation_z(double angle) {
    struct Matrix3x3 result;
    double c = cos(angle);
    double s = sin(angle);
    result.elements[0][0] = c;   result.elements[0][1] = -s;  result.elements[0][2] = 0.0;
    result.elements[1][0] = s;   result.elements[1][1] = c;   result.elements[1][2] = 0.0;
    result.elements[2][0] = 0.0; result.elements[2][1] = 0.0; result.elements[2][2