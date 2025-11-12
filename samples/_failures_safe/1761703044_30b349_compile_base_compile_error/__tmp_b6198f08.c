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
    if (!vector3d_validate(a) || !vector3d_validate(b)) return result;
    result.x = a->x + b->x;
    result.y = a->y + b->y;
    result.z = a->z + b->z;
    return result;
}

struct Vector3D matrix3x3_multiply_vector(const struct Matrix3x3 *m, const struct Vector3D *v) {
    struct Vector3D result = {0.0, 0.0, 0.0};
    if (!matrix3x3_validate(m) || !vector3d_validate(v)) return result;
    result.x = m->elements[0][0] * v->x + m->elements[0][1] * v->y + m->elements[0][2] * v->z;
    result.y = m->elements[1][0] * v->x + m->elements[1][1] * v->y + m->elements[1][2] * v->z;
    result.z = m->elements[2][0] * v->x + m->elements[2][1] * v->y + m->elements[2][2] * v->z;
    return result;
}

struct Vector3D transform_point(const struct Transform *t, const struct Vector3D *p) {
    struct Vector3D result = {0.0, 0.0, 0.0};
    if (!transform_validate(t) || !vector3d_validate(p)) return result;
    struct Vector3D rotated = matrix3x3_multiply_vector(&t->rotation, p);
    struct Vector3D scaled = {rotated.x * t->scale, rotated.y * t->scale, rotated.z * t->scale};
    result = vector3d_add(&scaled, &t->translation);
    return result;
}

int pointcloud_init(struct PointCloud *pc, size_t initial_capacity) {
    if (pc == NULL || initial_capacity == 0 || initial_capacity > SIZE_MAX / sizeof(struct Vector3D)) return 0;
    pc->points = malloc(initial_capacity * sizeof(struct Vector3D));
    if (pc->points == NULL) return 0;
    pc->count = 0;
    pc->capacity = initial_capacity;
    return 1;
}

int pointcloud_add_point(struct PointCloud *pc, const struct Vector3D *point) {
    if (pc == NULL || !vector3d_validate(point)) return 0;
    if (pc->count >= pc->capacity) {
        size_t new_capacity;
        if (pc->capacity > SIZE_MAX / 2 / sizeof(struct Vector3D)) return 0;
        new_capacity = pc->capacity * 2;
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

int pointcloud_transform(const struct PointCloud *input, const struct Transform *t, struct PointCloud *output) {
    if (input == NULL || !transform_validate(t) || output == NULL) return 0;
    if (input->count == 0) return 1;
    for (size_t i = 0; i < input->count;