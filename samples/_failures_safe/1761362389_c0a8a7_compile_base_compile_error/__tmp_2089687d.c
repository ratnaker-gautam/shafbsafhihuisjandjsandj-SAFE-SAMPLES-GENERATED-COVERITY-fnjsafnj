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

struct Transform transform_combine(struct Transform a, struct Transform b) {
    struct Transform result;
    result.rotation = matrix_multiply(a.rotation, b.rotation);
    result.translation = vector_add(matrix_vector_multiply(a.rotation, b.translation), a.translation);
    return result;
}

struct Vector3D transform_point(struct Transform t, struct Vector3D p) {
    return vector_add(matrix_vector_multiply(t.rotation, p), t.translation);
}

struct PointCloud create_point_cloud(size_t count) {
    struct PointCloud cloud;
    cloud.count = count;
    cloud.points = NULL;
    if (count > 0 && count <= 1000000) {
        cloud.points = malloc(count * sizeof(struct Vector3D));
    }
    return cloud;
}

void free_point_cloud(struct PointCloud* cloud) {
    if (cloud && cloud->points) {
        free(cloud->points);
        cloud->points = NULL;
        cloud->count = 0;
    }
}

int read_point_cloud(struct PointCloud* cloud) {
    if (!cloud || !cloud->points) {
        return 0;
    }
    for (size_t i = 0; i < cloud->count; i++) {
        printf("Enter point %zu (x y z): ", i + 1);
        if (scanf("%lf %lf %lf", &cloud->points[i].x, &cloud->points[i].y, &cloud->points[i].z) != 3) {
            return 0;
        }
    }
    return 1;
}

void apply_transform_to_point_cloud(struct PointCloud* cloud, struct Transform t) {
    if (!cloud || !cloud->points) {
        return;
    }
    for (size_t i = 0; i < cloud->count; i++) {
        cloud->points[i] = transform_point(t, cloud->points[i]);
    }
}

void print_point_cloud(struct PointCloud* cloud) {
    if (!cloud || !cloud->points) {
        return;
    }
    printf("Transformed points:\n");
    for (size_t i = 0; i < cloud->count; i++) {