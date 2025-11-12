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
};

struct PointCloud {
    struct Vector3D *points;
    size_t count;
    size_t capacity;
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

struct Vector3D vector3d_multiply_scalar(struct Vector3D v, double scalar) {
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

double vector3d_magnitude(struct Vector3D v) {
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

struct Matrix3x3 matrix3x3_identity(void) {
    struct Matrix3x3 result = {0};
    result.elements[0][0] = 1.0;
    result.elements[1][1] = 1.0;
    result.elements[2][2] = 1.0;
    return result;
}

struct Matrix3x3 matrix3x3_multiply(struct Matrix3x3 a, struct Matrix3x3 b) {
    struct Matrix3x3 result = {0};
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                result.elements[i][j] += a.elements[i][k] * b.elements[k][j];
            }
        }
    }
    return result;
}

struct Vector3D matrix3x3_multiply_vector(struct Matrix3x3 m, struct Vector3D v) {
    struct Vector3D result;
    result.x = m.elements[0][0] * v.x + m.elements[0][1] * v.y + m.elements[0][2] * v.z;
    result.y = m.elements[1][0] * v.x + m.elements[1][1] * v.y + m.elements[1][2] * v.z;
    result.z = m.elements[2][0] * v.x + m.elements[2][1] * v.y + m.elements[2][2] * v.z;
    return result;
}

struct Transform transform_combine(struct Transform a, struct Transform b) {
    struct Transform result;
    result.rotation = matrix3x3_multiply(a.rotation, b.rotation);
    result.translation = vector3d_add(matrix3x3_multiply_vector(a.rotation, b.translation), a.translation);
    return result;
}

struct Vector3D transform_apply(struct Transform t, struct Vector3D point) {
    return vector3d_add(matrix3x3_multiply_vector(t.rotation, point), t.translation);
}

int pointcloud_init(struct PointCloud *cloud, size_t initial_capacity) {
    if (cloud == NULL || initial_capacity == 0) return 0;
    cloud->points = malloc(initial_capacity * sizeof(struct Vector3D));
    if (cloud->points == NULL) return 0;
    cloud->count = 0;
    cloud->capacity = initial_capacity;
    return 1;
}

void pointcloud_free(struct PointCloud *cloud) {
    if (cloud != NULL) {
        free(cloud->points);
        cloud->points = NULL;
        cloud->count = 0;
        cloud->capacity = 0;
    }
}

int pointcloud_add_point(struct PointCloud *cloud, struct Vector3D point) {
    if (cloud == NULL) return 0;
    if (cloud->count >= cloud->capacity) {
        size_t new_capacity = cloud->capacity * 2;
        if (new_capacity < cloud->capacity) return 0;
        struct Vector3D *new_points = realloc(cloud->points, new_capacity * sizeof(struct Vector3D));
        if (new_points == NULL) return 0;
        cloud->points = new_points;
        cloud->capacity = new_capacity;
    }
    cloud->points[cloud->count] = point;
    cloud->count++;