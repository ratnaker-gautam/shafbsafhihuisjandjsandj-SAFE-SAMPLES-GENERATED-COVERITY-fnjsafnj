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

struct GeometricTransform create_transform(struct Vector3D axis, double angle, struct Vector3D translation, double scale) {
    struct GeometricTransform transform;
    transform.scale = scale;
    transform.translation = translation;
    
    double c = cos(angle);
    double s = sin(angle);
    double t = 1.0 - c;
    struct Vector3D n = vector_normalize(axis);
    
    transform.rotation.elements[0][0] = t * n.x * n.x + c;
    transform.rotation.elements[0][1] = t * n.x * n.y - s * n.z;
    transform.rotation.elements[0][2] = t * n.x * n.z + s * n.y;
    
    transform.rotation.elements[1][0] = t * n.x * n.y + s * n.z;
    transform.rotation.elements[1][1] = t * n.y * n.y + c;
    transform.rotation.elements[1][2] = t * n.y * n.z - s * n.x;
    
    transform.rotation.elements[2][0] = t * n.x * n.z - s * n.y;
    transform.rotation.elements[2][1] = t * n.y * n.z + s * n.x;
    transform.rotation.elements[2][2] = t * n.z * n.z + c;
    
    return transform;
}

struct Vector3D apply_transform(struct GeometricTransform transform, struct Vector3D point) {
    struct Vector3D scaled = vector_scale(point, transform.scale);
    struct Vector3D rotated = matrix_vector_multiply(transform.rotation, scaled);
    return vector_add(rotated, transform.translation);
}

struct BoundingBox calculate_bounding_box(struct Vector3D* points, int count