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

struct PointCloud {
    struct Vector3D* points;
    size_t count;
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

struct Vector3D vector_cross(struct Vector3D a, struct Vector3D b) {
    struct Vector3D result;
    result.x = a.y * b.z - a.z * b.y;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;
    return result;
}

double vector_dot(struct Vector3D a, struct Vector3D b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
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

struct Vector3D matrix_vector_multiply(struct Matrix3x3 m, struct Vector3D v) {
    struct Vector3D result;
    result.x = m.elements[0][0] * v.x + m.elements[0][1] * v.y + m.elements[0][2] * v.z;
    result.y = m.elements[1][0] * v.x + m.elements[1][1] * v.y + m.elements[1][2] * v.z;
    result.z = m.elements[2][0] * v.x + m.elements[2][1] * v.y + m.elements[2][2] * v.z;
    return result;
}

struct Transform transform_identity(void) {
    struct Transform t;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            t.rotation.elements[i][j] = (i == j) ? 1.0 : 0.0;
        }
    }
    t.translation.x = 0.0;
    t.translation.y = 0.0;
    t.translation.z = 0.0;
    return t;
}

struct Vector3D transform_point(struct Transform t, struct Vector3D p) {
    struct Vector3D rotated = matrix_vector_multiply(t.rotation, p);
    return vector_add(rotated, t.translation);
}

struct PointCloud transform_pointcloud(struct Transform t, struct PointCloud cloud) {
    struct PointCloud result;
    result.count = cloud.count;
    result.points = malloc(sizeof(struct Vector3D) * cloud.count);
    if (result.points == NULL) {
        result.count = 0;
        return result;
    }
    for (size_t i = 0; i < cloud.count; i++) {
        result.points[i] = transform_point(t, cloud.points[i]);
    }
    return result;
}

void free_pointcloud(struct PointCloud* cloud) {
    if (cloud->points != NULL) {
        free(cloud->points);
        cloud->points = NULL;
    }
    cloud->count = 0;
}

struct Transform create_rotation_transform(double angle, struct Vector3D axis) {
    struct Transform t = transform_identity();
    struct Vector3D norm_axis = vector_normalize(axis);
    double c = cos(angle);
    double s = sin(angle);
    double x = norm_axis.x;
    double y = norm_axis.y;
    double z = norm_axis.z;
    t.rotation.elements[0][0] = c + x*x*(1-c);
    t.rotation.elements[0][1] = x*y*(1-c) - z*s;
    t.rotation.elements[0][2] = x*z*(1-c) + y*s;
    t.rotation.elements[1][0] = y*x*(1-c) + z*s;
    t.rotation.elements[1][1] = c + y*y*(1-c);
    t.rotation.elements[1][2] = y*z*(1-c) - x*s;
    t.rotation.elements[2][0] = z*x*(1-c) - y*s;
    t.rotation.elements[2][1] = z*y*(1-c) + x*s;
    t.rotation.elements[2][2] = c + z*z*(1-c);
    return t;