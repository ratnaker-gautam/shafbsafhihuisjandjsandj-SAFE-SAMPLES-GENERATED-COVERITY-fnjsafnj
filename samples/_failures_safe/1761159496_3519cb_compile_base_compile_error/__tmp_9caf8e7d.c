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

struct Vector3D vector3d_add(const struct Vector3D *a, const struct Vector3D *b) {
    struct Vector3D result = {0.0, 0.0, 0.0};
    if (a == NULL || b == NULL) return result;
    result.x = a->x + b->x;
    result.y = a->y + b->y;
    result.z = a->z + b->z;
    return result;
}

struct Vector3D vector3d_multiply_matrix(const struct Vector3D *v, const struct Matrix3x3 *m) {
    struct Vector3D result = {0.0, 0.0, 0.0};
    if (v == NULL || m == NULL) return result;
    result.x = v->x * m->elements[0][0] + v->y * m->elements[1][0] + v->z * m->elements[2][0];
    result.y = v->x * m->elements[0][1] + v->y * m->elements[1][1] + v->z * m->elements[2][1];
    result.z = v->x * m->elements[0][2] + v->y * m->elements[1][2] + v->z * m->elements[2][2];
    return result;
}

struct Vector3D transform_apply(const struct Transform *t, const struct Vector3D *v) {
    struct Vector3D result = {0.0, 0.0, 0.0};
    if (!transform_validate(t) || !vector3d_validate(v)) return result;
    struct Vector3D rotated = vector3d_multiply_matrix(v, &t->rotation);
    result.x = rotated.x * t->scale + t->translation.x;
    result.y = rotated.y * t->scale + t->translation.y;
    result.z = rotated.z * t->scale + t->translation.z;
    return result;
}

int pointcloud_init(struct PointCloud *pc, size_t initial_capacity) {
    if (pc == NULL || initial_capacity == 0) return 0;
    pc->points = malloc(initial_capacity * sizeof(struct Vector3D));
    if (pc->points == NULL) return 0;
    pc->count = 0;
    pc->capacity = initial_capacity;
    return 1;
}

int pointcloud_add(struct PointCloud *pc, const struct Vector3D *point) {
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

void pointcloud_cleanup(struct PointCloud *pc) {
    if (pc != NULL) {
        free(pc->points);
        pc->points = NULL;
        pc->count = 0;
        pc->capacity = 0;
    }
}

struct Statistics pointcloud_compute_stats(const struct PointCloud *pc) {
    struct Statistics stats = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    if (pc == NULL || pc->count == 0) return stats;
    for (size_t i =