//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: numeric_computation
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
};

struct PointCloud {
    struct Vector3D *points;
    size_t count;
    size_t capacity;
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

struct Transform transform_compose(struct Transform a, struct Transform b) {
    struct Transform result;
    result.rotation = matrix_multiply(a.rotation, b.rotation);
    result.translation = vector_add(matrix_vector_multiply(a.rotation, b.translation), a.translation);
    return result;
}

struct Vector3D transform_point(struct Transform t, struct Vector3D p) {
    return vector_add(matrix_vector_multiply(t.rotation, p), t.translation);
}

int pointcloud_init(struct PointCloud *cloud, size_t initial_capacity) {
    if (initial_capacity == 0 || initial_capacity > SIZE_MAX / sizeof(struct Vector3D)) {
        return 0;
    }
    cloud->points = malloc(initial_capacity * sizeof(struct Vector3D));
    if (cloud->points == NULL) {
        return 0;
    }
    cloud->count = 0;
    cloud->capacity = initial_capacity;
    return 1;
}

void pointcloud_free(struct PointCloud *cloud) {
    free(cloud->points);
    cloud->points = NULL;
    cloud->count = 0;
    cloud->capacity = 0;
}

int pointcloud_add_point(struct PointCloud *cloud, struct Vector3D point) {
    if (cloud->count >= cloud->capacity) {
        size_t new_capacity;
        if (cloud->capacity > SIZE_MAX / 2 / sizeof(struct Vector3D)) {
            return 0;
        }
        new_capacity = cloud->capacity * 2;
        struct Vector3D *new_points = realloc(cloud->points, new_capacity * sizeof(struct Vector3D));
        if (new_points == NULL) {
            return 0;
        }
        cloud->points = new_points;
        cloud->capacity = new_capacity;
    }
    cloud->points[cloud->count] = point