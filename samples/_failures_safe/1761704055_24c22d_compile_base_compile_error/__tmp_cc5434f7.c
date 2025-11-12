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
    if (endptr == buffer || *endptr != '\n') {
        return 0;
    }
    return 1;
}

struct Vector3D vector_add(struct Vector3D a, struct Vector3D b) {
    struct Vector3D result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    return result;
}

struct Vector3D vector_scale(struct Vector3D v, double scalar) {
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
    return sqrt(vector_dot(v, v));
}

struct Vector3D vector_normalize(struct Vector3D v) {
    double len = vector_length(v);
    if (len > 0.0) {
        return vector_scale(v, 1.0 / len);
    }
    return v;
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

struct Vector3D matrix_transform(struct Matrix3x3 m, struct Vector3D v) {
    struct Vector3D result;
    result.x = m.elements[0][0] * v.x + m.elements[0][1] * v.y + m.elements[0][2] * v.z;
    result.y = m.elements[1][0] * v.x + m.elements[1][1] * v.y + m.elements[1][2] * v.z;
    result.z = m.elements[2][0] * v.x + m.elements[2][1] * v.y + m.elements[2][2] * v.z;
    return result;
}

struct Transform transform_combine(struct Transform a, struct Transform b) {
    struct Transform result;
    result.rotation = matrix_multiply(a.rotation, b.rotation);
    result.translation = vector_add(matrix_transform(a.rotation, b.translation), a.translation);
    result.scale = a.scale * b.scale;
    return result;
}

struct Vector3D transform_apply(struct Transform t, struct Vector3D v) {
    struct Vector3D scaled = vector_scale(v, t.scale);
    struct Vector3D rotated = matrix_transform(t.rotation, scaled);
    return vector_add(rotated, t.translation);
}

void update_bounds(struct BoundingBox* bounds, struct Vector3D point) {
    if (point.x < bounds->min.x) bounds->min.x = point.x;
    if (point.y < bounds->min.y) bounds->min.y = point.y;
    if (point.z < bounds->min.z) bounds->min.z = point.z;
    if (point.x > bounds->max.x) bounds->max.x = point.x;
    if (point.y > bounds->max.y) bounds->max.y = point.y;
    if (point.z > bounds->max.z) bounds->max.z = point.z;
}

int initialize_mesh(struct Mesh* mesh, uint32_t vertex_count) {
    if (vertex_count == 0 || vertex_count > 1000000) {
        return 0;
    }
    mesh->vertices = malloc(vertex_count * sizeof(struct Vector3D));
    if (mesh->vertices == NULL) {
        return 0;
    }
    mesh->vertex_count = vertex_count;
    mesh->bounds.min.x = mesh->bounds.min.y = mesh->bounds.min.z = HUGE_VAL;
    mesh->bounds