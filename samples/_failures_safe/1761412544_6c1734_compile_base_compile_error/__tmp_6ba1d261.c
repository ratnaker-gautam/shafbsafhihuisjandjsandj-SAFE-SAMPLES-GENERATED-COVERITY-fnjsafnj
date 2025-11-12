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

struct Matrix3x3 matrix3x3_multiply(struct Matrix3x3 a, struct Matrix3x3 b) {
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

struct Vector3D matrix3x3_multiply_vector(struct Matrix3x3 m, struct Vector3D v) {
    struct Vector3D result;
    result.x = m.elements[0][0] * v.x + m.elements[0][1] * v.y + m.elements[0][2] * v.z;
    result.y = m.elements[1][0] * v.x + m.elements[1][1] * v.y + m.elements[1][2] * v.z;
    result.z = m.elements[2][0] * v.x + m.elements[2][1] * v.y + m.elements[2][2] * v.z;
    return result;
}

struct Vector3D transform_apply(struct Transform t, struct Vector3D point) {
    struct Vector3D rotated = matrix3x3_multiply_vector(t.rotation, point);
    return vector3d_add(rotated, t.translation);
}

struct PointCloud pointcloud_create(size_t count) {
    struct PointCloud cloud;
    cloud.count = count;
    cloud.points = NULL;
    if (count > 0 && count <= 1000000) {
        cloud.points = (struct Vector3D*)malloc(count * sizeof(struct Vector3D));
    }
    return cloud;
}

void pointcloud_destroy(struct PointCloud* cloud) {
    if (cloud->points != NULL) {
        free(cloud->points);
        cloud->points = NULL;
    }
    cloud->count = 0;
}

int pointcloud_transform(struct PointCloud* cloud, struct Transform transform) {
    if (cloud == NULL || cloud->points == NULL) {
        return 0;
    }
    for (size_t i = 0; i < cloud->count; i++) {
        cloud->points[i] = transform_apply(transform, cloud->points[i]);
    }
    return 1;
}

struct Vector3D pointcloud_centroid(struct PointCloud cloud) {
    struct Vector3D centroid = {0.0, 0.0, 0.0};
    if (cloud.points == NULL || cloud.count == 0) {
        return centroid;
    }
    for (size_t i = 0; i < cloud.count; i++) {
        centroid.x += cloud.points[i].x;
        centroid.y += cloud.points[i].y;
        centroid.z += cloud.points[i].z;
    }
    double inv_count = 1.0 / (double)cloud.count;
    centroid.x *= inv_count;
    centroid.y *= inv_count;
    centroid.z *= inv_count;
    return centroid;
}

int main(void) {
    printf("Enter number of points in cloud (1-1000): ");
    int num_points;
    if (scanf("%d", &num_points) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    if (num_points < 1 || num_points > 1000) {
        printf("Number of points must be between 1 and 1000\n");
        return 1;
    }
    
    struct PointCloud cloud = pointcloud