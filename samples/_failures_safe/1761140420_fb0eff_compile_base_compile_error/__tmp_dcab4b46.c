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

struct Transform transform_compose(struct Transform a, struct Transform b) {
    struct Transform result;
    result.rotation = matrix_multiply(a.rotation, b.rotation);
    result.translation = vector_add(matrix_vector_multiply(a.rotation, b.translation), a.translation);
    return result;
}

struct Vector3D transform_apply(struct Transform t, struct Vector3D v) {
    return vector_add(matrix_vector_multiply(t.rotation, v), t.translation);
}

struct Matrix3x3 rotation_matrix_x(double angle) {
    struct Matrix3x3 result = {{{1.0, 0.0, 0.0},
                               {0.0, cos(angle), -sin(angle)},
                               {0.0, sin(angle), cos(angle)}}};
    return result;
}

struct Matrix3x3 rotation_matrix_y(double angle) {
    struct Matrix3x3 result = {{{cos(angle), 0.0, sin(angle)},
                               {0.0, 1.0, 0.0},
                               {-sin(angle), 0.0, cos(angle)}}};
    return result;
}

struct Matrix3x3 rotation_matrix_z(double angle) {
    struct Matrix3x3 result = {{{cos(angle), -sin(angle), 0.0},
                               {sin(angle), cos(angle), 0.0},
                               {0.0, 0.0, 1.0}}};
    return result;
}

int main(void) {
    struct Vector3D point = {1.0, 2.0, 3.0};
    struct Vector3D vector1 = {4.0, 5.0, 6.0};
    struct Vector3D vector2 = {7.0, 8.0, 9.0};
    
    printf("Original point: (%f, %f, %f)\n", point.x, point.y, point.z);
    printf("Vector 1: (%f, %f, %f)\n", vector1.x, vector1.y, vector1.z);
    printf("Vector 2: (%f, %f, %f)\n", vector2.x, vector2.y, vector2.z);
    
    struct Vector3D sum