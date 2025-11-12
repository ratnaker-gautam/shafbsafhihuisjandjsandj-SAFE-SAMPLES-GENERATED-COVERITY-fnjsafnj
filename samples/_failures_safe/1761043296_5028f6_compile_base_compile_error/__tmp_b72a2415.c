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
    result.x = t->scale * (t->rotation.elements[0][0] * v->x + t->rotation.elements[0][1] * v->y + t->rotation.elements[0][2] * v->z) + t->translation.x;
    result.y = t->scale * (t->rotation.elements[1][0] * v->x + t->rotation.elements[1][1] * v->y + t->rotation.elements[1][2] * v->z) + t->translation.y;
    result.z = t->scale * (t->rotation.elements[2][0] * v->x + t->rotation.elements[2][1] * v->y + t->rotation.elements[2][2] * v->z) + t->translation.z;
    return result;
}

int pointcloud_transform(struct PointCloud *pc, const struct Transform *t) {
    if (pc == NULL || !transform_validate(t)) return 0;
    for (size_t i = 0; i < pc->count; i++) {
        if (!vector3d_validate(&pc->points[i])) return 0;
        pc->points[i] = vector3d_transform(&pc->points[i], t);
    }
    return 1;
}

int statistics_compute(const struct PointCloud *pc, struct Statistics *stats) {
    if (pc == NULL || stats == NULL || pc->count == 0) return 0;
    stats->mean_x = 0.0;
    stats->mean_y = 0.0;
    stats->mean_z = 0.0;
    for (size_t i = 0; i < pc->count; i++) {
        if (!vector3d_validate(&pc->points[i])) return 0;
        stats->mean_x += pc->points[i].x;
        stats->mean_y += pc->points[i].y;
        stats->mean_z += pc->points[i].z;
    }
    stats->mean_x /= pc->count;
    stats->mean_y /= pc