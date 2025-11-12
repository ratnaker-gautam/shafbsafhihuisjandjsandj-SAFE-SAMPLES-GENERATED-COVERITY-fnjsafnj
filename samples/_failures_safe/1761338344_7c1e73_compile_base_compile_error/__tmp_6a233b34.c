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
    struct Vector3D scaling;
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

struct Vector3D vector_multiply(struct Vector3D v, double scalar) {
    struct Vector3D result;
    result.x = v.x * scalar;
    result.y = v.y * scalar;
    result.z = v.z * scalar;
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

struct GeometricTransform create_transform(struct Vector3D rotation_angles, struct Vector3D translation, struct Vector3D scaling) {
    struct GeometricTransform transform;
    
    double cx = cos(rotation_angles.x);
    double sx = sin(rotation_angles.x);
    double cy = cos(rotation_angles.y);
    double sy = sin(rotation_angles.y);
    double cz = cos(rotation_angles.z);
    double sz = sin(rotation_angles.z);
    
    struct Matrix3x3 rx = {
        {{1.0, 0.0, 0.0},
         {0.0, cx, -sx},
         {0.0, sx, cx}}
    };
    
    struct Matrix3x3 ry = {
        {{cy, 0.0, sy},
         {0.0, 1.0, 0.0},
         {-sy, 0.0, cy}}
    };
    
    struct Matrix3x3 rz = {
        {{cz, -sz, 0.0},
         {sz, cz, 0.0},
         {0.0, 0.0, 1.0}}
    };
    
    struct Matrix3x3 rotation = matrix_multiply(rz, matrix_multiply(ry, rx));
    
    struct Matrix3x3 scale_matrix = {
        {{scaling.x, 0.0, 0.0},
         {0.0, scaling.y, 0.0},
         {0.0, 0.0, scaling.z}}
    };
    
    transform.rotation = matrix_multiply(rotation, scale_matrix);
    transform.translation = translation;
    transform.scaling = scaling;
    
    return transform;
}

struct Vector3D apply_transform(struct GeometricTransform transform, struct Vector3D point) {
    struct Vector3D scaled