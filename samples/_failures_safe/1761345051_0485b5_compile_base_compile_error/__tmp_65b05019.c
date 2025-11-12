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

struct TransformResult {
    struct Vector3D original;
    struct Vector3D transformed;
    double magnitude_original;
    double magnitude_transformed;
    double scale_factor;
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
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

struct Matrix3x3 matrix_create_rotation_x(double angle) {
    struct Matrix3x3 m;
    double c = cos(angle);
    double s = sin(angle);
    m.elements[0][0] = 1.0; m.elements[0][1] = 0.0; m.elements[0][2] = 0.0;
    m.elements[1][0] = 0.0; m.elements[1][1] = c;   m.elements[1][2] = -s;
    m.elements[2][0] = 0.0; m.elements[2][1] = s;   m.elements[2][2] = c;
    return m;
}

struct Matrix3x3 matrix_create_rotation_y(double angle) {
    struct Matrix3x3 m;
    double c = cos(angle);
    double s = sin(angle);
    m.elements[0][0] = c;   m.elements[0][1] = 0.0; m.elements[0][2] = s;
    m.elements[1][0] = 0.0; m.elements[1][1] = 1.0; m.elements[1][2] = 0.0;
    m.elements[2][0] = -s;  m.elements[2][1] = 0.0; m.elements[2][2] = c;
    return m;
}

struct Vector3D matrix_vector_multiply(struct Matrix3x3 m, struct Vector3D v) {
    struct Vector3D result;
    result.x = m.elements[0][0] * v.x + m.elements[0][1] * v.y + m.elements[0][2] * v.z;
    result.y = m.elements[1][0] * v.x + m.elements[1][1] * v.y + m.elements[1][2] * v.z;
    result.z = m.elements[2][0] * v.x + m.elements[2][1] * v.y + m.elements[2][2] * v.z;
    return result;
}

int validate_vector_input(double x, double y, double z) {
    if (x != x || y != y || z != z) return 0;
    if (x > 1e100 || x < -1e100) return 0;
    if (y > 1e100 || y < -1e100) return 0;
    if (z > 1e100 || z < -1e100) return 0;
    return 1;
}

int validate_angle(double angle) {
    if (angle != angle) return 0;
    if (angle > 100.0 || angle < -100.0) return 0;
    return 1;
}

struct TransformResult perform_vector_transformation(struct Vector3D v, double angle_x, double angle_y) {
    struct TransformResult result;
    result.original = v;
    result.magnitude_original = vector_magnitude(v);
    
    struct Matrix3x3 rot_x = matrix_create_rotation_x(angle_x);
    struct Matrix3x3 rot_y = matrix_create_rotation_y(angle_y);
    
    struct Vector3D temp = matrix_vector_multiply(rot_x, v);
    result.transformed = matrix_vector_multiply(rot_y, temp);
    
    result.magnitude_transformed = vector_magnitude(result.transformed);
    
    if (result.magnitude_original > 1e-10) {
        result.scale_factor = result.magnitude_transformed / result.magnitude_original;
    } else {
        result.scale_factor = 1.0;
    }
    
    return result;
}

void print_vector(struct