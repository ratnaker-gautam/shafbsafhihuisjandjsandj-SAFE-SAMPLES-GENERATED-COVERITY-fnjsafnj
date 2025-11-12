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

struct PointCloud {
    struct Vector3D *points;
    size_t count;
    size_t capacity;
};

struct Statistics {
    double mean_x;
    double mean_y;
    double mean_z;
    double variance_x;
    double variance_y;
    double variance_z;
    struct Vector3D centroid;
};

int vector3d_validate(const struct Vector3D *vec) {
    if (vec == NULL) return 0;
    if (!isfinite(vec->x) || !isfinite(vec->y) || !isfinite(vec->z)) return 0;
    return 1;
}

int matrix3x3_validate(const struct Matrix3x3 *mat) {
    if (mat == NULL) return 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (!isfinite(mat->elements[i][j])) return 0;
        }
    }
    return 1;
}

int transform_validate(const struct Transform *t) {
    if (t == NULL) return 0;
    if (!matrix3x3_validate(&t->rotation)) return 0;
    if (!vector3d_validate(&t->translation)) return 0;
    if (!isfinite(t->scale) || t->scale <= 0.0) return 0;
    return 1;
}

struct Vector3D vector3d_add(const struct Vector3D *a, const struct Vector3D *b) {
    struct Vector3D result = {0.0, 0.0, 0.0};
    if (a == NULL || b == NULL) return result;
    result.x = a->x + b->x;
    result.y = a->y + b->y;
    result.z = a->z + b->z;
    return result;
}

struct Vector3D vector3d_transform(const struct Vector3D *point, const struct Transform *transform) {
    struct Vector3D result = {0.0, 0.0, 0.0};
    if (!vector3d_validate(point) || !transform_validate(transform)) return result;
    
    double tx = transform->rotation.elements[0][0] * point->x +
                transform->rotation.elements[0][1] * point->y +
                transform->rotation.elements[0][2] * point->z;
    double ty = transform->rotation.elements[1][0] * point->x +
                transform->rotation.elements[1][1] * point->y +
                transform->rotation.elements[1][2] * point->z;
    double tz = transform->rotation.elements[2][0] * point->x +
                transform->rotation.elements[2][1] * point->y +
                transform->rotation.elements[2][2] * point->z;
    
    result.x = tx * transform->scale + transform->translation.x;
    result.y = ty * transform->scale + transform->translation.y;
    result.z = tz * transform->scale + transform->translation.z;
    
    return result;
}

int pointcloud_init(struct PointCloud *cloud, size_t initial_capacity) {
    if (cloud == NULL || initial_capacity == 0) return 0;
    cloud->points = malloc(initial_capacity * sizeof(struct Vector3D));
    if (cloud->points == NULL) return 0;
    cloud->count = 0;
    cloud->capacity = initial_capacity;
    return 1;
}

int pointcloud_add_point(struct PointCloud *cloud, const struct Vector3D *point) {
    if (cloud == NULL || !vector3d_validate(point)) return 0;
    if (cloud->count >= cloud->capacity) {
        size_t new_capacity = cloud->capacity * 2;
        if (new_capacity <= cloud->capacity) return 0;
        struct Vector3D *new_points = realloc(cloud->points, new_capacity * sizeof(struct Vector3D));
        if (new_points == NULL) return 0;
        cloud->points = new_points;
        cloud->capacity = new_capacity;
    }
    cloud->points[cloud->count] = *point;
    cloud->count++;
    return 1;
}

void pointcloud_cleanup(struct PointCloud *cloud) {
    if (cloud != NULL) {
        free(cloud->points);
        cloud->points = NULL;
        cloud->count = 0;
        cloud->capacity = 0;
    }
}

struct Statistics pointcloud_compute_stats(const struct PointCloud *cloud) {
    struct Statistics stats = {0};
    if (cloud == NULL || cloud->points == NULL || cloud->count == 0) return stats;
    
    for (size_t i = 0; i < cloud->count; i++) {
        stats.mean_x += cloud->points[i].x;
        stats.mean_y += cloud->points[i].y;
        stats.mean_z += cloud->points[i].z;
    }
    stats.mean_x /= cloud->count;
    stats.mean_y /= cloud->count;
    stats.mean_z /= cloud->count;
    
    stats.cent