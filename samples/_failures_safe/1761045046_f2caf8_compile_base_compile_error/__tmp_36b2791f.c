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
    struct Vector3D scale;
};

struct Geometry {
    struct Vector3D vertices[8];
    int vertex_count;
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
    if (len > 1e-10) {
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

struct Matrix3x3 create_rotation_matrix(double angle, struct Vector3D axis) {
    struct Matrix3x3 result;
    struct Vector3D norm_axis = vector_normalize(axis);
    double c = cos(angle);
    double s = sin(angle);
    double t = 1.0 - c;
    
    result.elements[0][0] = t * norm_axis.x * norm_axis.x + c;
    result.elements[0][1] = t * norm_axis.x * norm_axis.y - s * norm_axis.z;
    result.elements[0][2] = t * norm_axis.x * norm_axis.z + s * norm_axis.y;
    
    result.elements[1][0] = t * norm_axis.x * norm_axis.y + s * norm_axis.z;
    result.elements[1][1] = t * norm_axis.y * norm_axis.y + c;
    result.elements[1][2] = t * norm_axis.y * norm_axis.z - s * norm_axis.x;
    
    result.elements[2][0] = t * norm_axis.x * norm_axis.z - s * norm_axis.y;
    result.elements[2][1] = t * norm_axis.y * norm_axis.z + s * norm_axis.x;
    result.elements[2][2] = t * norm_axis.z * norm_axis.z + c;
    
    return result;
}

struct Vector3D transform_point(struct Transform t, struct Vector3D point) {
    struct Vector3D scaled;
    scaled.x = point.x * t.scale.x;
    scaled.y = point.y * t.scale.y;
    scaled.z = point.z * t.scale.z;
    
    struct Vector3D rotated = matrix_vector_multiply(t.rotation, scaled);