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

int validate_double_input(const char* prompt, double* value) {
    char buffer[256];
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    char* endptr;
    *value = strtod(buffer, &endptr);
    if (endptr == buffer) {
        return 0;
    }
    return 1;
}

struct Vector3D create_vector(double x, double y, double z) {
    struct Vector3D vec;
    vec.x = x;
    vec.y = y;
    vec.z = z;
    return vec;
}

struct Vector3D add_vectors(struct Vector3D a, struct Vector3D b) {
    struct Vector3D result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    return result;
}

struct Vector3D subtract_vectors(struct Vector3D a, struct Vector3D b) {
    struct Vector3D result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    return result;
}

struct Vector3D scale_vector(struct Vector3D v, double scalar) {
    struct Vector3D result;
    result.x = v.x * scalar;
    result.y = v.y * scalar;
    result.z = v.z * scalar;
    return result;
}

double dot_product(struct Vector3D a, struct Vector3D b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

struct Vector3D cross_product(struct Vector3D a, struct Vector3D b) {
    struct Vector3D result;
    result.x = a.y * b.z - a.z * b.y;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;
    return result;
}

double vector_length(struct Vector3D v) {
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

struct Matrix3x3 create_identity_matrix(void) {
    struct Matrix3x3 mat;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            mat.elements[i][j] = (i == j) ? 1.0 : 0.0;
        }
    }
    return mat;
}

struct Matrix3x3 multiply_matrices(struct Matrix3x3 a, struct Matrix3x3 b) {
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

struct Vector3D transform_vector(struct Matrix3x3 mat, struct Vector3D vec) {
    struct Vector3D result;
    result.x = mat.elements[0][0] * vec.x + mat.elements[0][1] * vec.y + mat.elements[0][2] * vec.z;
    result.y = mat.elements[1][0] * vec.x + mat.elements[1][1] * vec.y + mat.elements[1][2] * vec.z;
    result.z = mat.elements[2][0] * vec.x + mat.elements[2][1] * vec.y + mat.elements[2][2] * vec.z;
    return result;
}

struct Transform create_transform(struct Matrix3x3 rot, struct Vector3D trans, double scale) {
    struct Transform tf;
    tf.rotation = rot;
    tf.translation = trans;
    tf.scale = scale;
    return tf;
}

struct Vector3D apply_transform(struct Transform tf, struct Vector3D point) {
    struct Vector3D scaled = scale_vector(point, tf.scale);
    struct Vector3D rotated = transform_vector(tf.rotation, scaled);
    return add_vectors(rotated, tf.translation);
}

struct BoundingBox compute_bounds(struct Vector3D* vertices, uint32_t count) {
    struct BoundingBox bounds;
    if (count == 0 || vertices == NULL) {
        bounds.min = create_vector(0.0, 0.0, 0.0);
        bounds.max = create_vector(0.0, 0.0, 0.0);
        return bounds;
    }
    bounds.min = vertices[0];
    bounds.max = vertices[0];
    for (uint32_t i = 1; i < count; i++) {