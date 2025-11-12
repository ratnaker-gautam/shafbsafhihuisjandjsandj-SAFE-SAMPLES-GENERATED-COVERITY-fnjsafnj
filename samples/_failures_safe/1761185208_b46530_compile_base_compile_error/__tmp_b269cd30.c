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
    double bounding_radius;
};

int vector3d_validate(const struct Vector3D *v) {
    if (v == NULL) return 0;
    if (!isfinite(v->x) || !isfinite(v->y) || !isfinite(v->z)) return 0;
    return 1;
}

int matrix3x3_validate(const struct Matrix3x3 *m) {
    if (m == NULL) return 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (!isfinite(m->elements[i][j])) return 0;
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

int pointcloud_init(struct PointCloud *pc, size_t initial_capacity) {
    if (pc == NULL || initial_capacity == 0) return 0;
    pc->points = malloc(initial_capacity * sizeof(struct Vector3D));
    if (pc->points == NULL) return 0;
    pc->count = 0;
    pc->capacity = initial_capacity;
    return 1;
}

void pointcloud_free(struct PointCloud *pc) {
    if (pc != NULL) {
        free(pc->points);
        pc->points = NULL;
        pc->count = 0;
        pc->capacity = 0;
    }
}

int pointcloud_add_point(struct PointCloud *pc, const struct Vector3D *point) {
    if (pc == NULL || !vector3d_validate(point)) return 0;
    if (pc->count >= pc->capacity) {
        size_t new_capacity = pc->capacity * 2;
        if (new_capacity <= pc->capacity) return 0;
        struct Vector3D *new_points = realloc(pc->points, new_capacity * sizeof(struct Vector3D));
        if (new_points == NULL) return 0;
        pc->points = new_points;
        pc->capacity = new_capacity;
    }
    pc->points[pc->count] = *point;
    pc->count++;
    return 1;
}

struct Vector3D vector3d_transform(const struct Vector3D *v, const struct Transform *t) {
    struct Vector3D result = {0.0, 0.0, 0.0};
    if (!vector3d_validate(v) || !transform_validate(t)) return result;
    for (int i = 0; i < 3; i++) {
        double sum = 0.0;
        for (int j = 0; j < 3; j++) {
            sum += t->rotation.elements[i][j] * (j == 0 ? v->x : (j == 1 ? v->y : v->z));
        }
        if (i == 0) result.x = sum * t->scale + t->translation.x;
        else if (i == 1) result.y = sum * t->scale + t->translation.y;
        else result.z = sum * t->scale + t->translation.z;
    }
    return result;
}

int pointcloud_transform(struct PointCloud *dest, const struct PointCloud *src, const struct Transform *t) {
    if (dest == NULL || src == NULL || !transform_validate(t)) return 0;
    if (!pointcloud_init(dest, src->count)) return 0;
    for (size_t i = 0; i < src->count; i++) {
        struct Vector3D transformed = vector3d_transform(&src->points[i], t);
        if (!pointcloud_add_point(dest, &transformed)) {
            pointcloud_free(dest);
            return 0;
        }
    }
    return 1;
}

int statistics_compute(struct Statistics *stats, const struct PointCloud *pc) {
    if (stats == NULL || pc == NULL || pc->count == 0) return 0;
    stats->mean_x = 0.0;
    stats->mean_y = 0.0;
    stats->mean_z = 0.0;
    for (size_t i = 0; i < pc->count; i++) {
        stats->mean_x += pc->points[i].x;
        stats->mean_y += pc->points[i].y;
        stats->mean_z += pc->points[i].z;
    }
    stats->mean_x /= pc->count;
    stats->mean_y /= pc->count;
    stats->mean_z /= pc->count;