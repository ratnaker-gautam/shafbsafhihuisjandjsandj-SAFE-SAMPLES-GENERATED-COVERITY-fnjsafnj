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

struct Vector3D vector3d_create(double x, double y, double z) {
    struct Vector3D v;
    v.x = x;
    v.y = y;
    v.z = z;
    return v;
}

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

double vector3d_magnitude(struct Vector3D v) {
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

struct Matrix3x3 matrix3x3_create_identity(void) {
    struct Matrix3x3 m;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            m.elements[i][j] = (i == j) ? 1.0 : 0.0;
        }
    }
    return m;
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

struct Transform transform_create(struct Matrix3x3 rotation, struct Vector3D translation) {
    struct Transform t;
    t.rotation = rotation;
    t.translation = translation;
    return t;
}

struct Vector3D transform_apply(struct Transform t, struct Vector3D v) {
    struct Vector3D rotated = matrix3x3_transform(t.rotation, v);
    return vector3d_add(rotated, t.translation);
}

struct Matrix3x3 matrix3x3_create_rotation_x(double angle) {
    struct Matrix3x3 m = matrix3x3_create_identity();
    double c = cos(angle);
    double s = sin(angle);
    m.elements[1][1] = c;
    m.elements[1][2] = -s;
    m.elements[2][1] = s;
    m.elements[2][2] = c;
    return m;
}

struct Matrix3x3 matrix3x3_create_rotation_y(double angle) {
    struct Matrix3x3 m = matrix3x3_create_identity();
    double c = cos(angle);
    double s = sin(angle);
    m.elements[0][0] = c;
    m.elements[0][2] = s;
    m.elements[2][0] = -s;
    m.elements[2][2] = c;
    return m;
}

struct Matrix3x3 matrix3x3_create_rotation_z(double angle) {
    struct Matrix3x3 m = matrix3x3_create_identity();
    double c = cos(angle);
    double s = sin(angle);
    m.elements[0][0] = c;
    m.elements[0][1] = -s;
    m.elements[1][0] = s;
    m.elements[1][1] = c;
    return m;
}

int main(void) {
    struct Vector3D point = vector3d_create(