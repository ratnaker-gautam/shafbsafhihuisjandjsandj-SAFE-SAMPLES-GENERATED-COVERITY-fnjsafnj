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

struct BoundingBox {
    struct Vector3D min;
    struct Vector3D max;
};

struct Geometry {
    struct Vector3D* vertices;
    uint32_t vertex_count;
    struct BoundingBox bounds;
};

int validate_vector(const struct Vector3D* v) {
    if (v == NULL) return 0;
    if (!isfinite(v->x) || !isfinite(v->y) || !isfinite(v->z)) return 0;
    return 1;
}

int validate_matrix(const struct Matrix3x3* m) {
    if (m == NULL) return 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (!isfinite(m->elements[i][j])) return 0;
        }
    }
    return 1;
}

int validate_transform(const struct Transform* t) {
    if (t == NULL) return 0;
    if (!validate_matrix(&t->rotation)) return 0;
    if (!validate_vector(&t->translation)) return 0;
    if (!isfinite(t->scale) || t->scale <= 0.0) return 0;
    return 1;
}

struct Vector3D vector_add(const struct Vector3D* a, const struct Vector3D* b) {
    struct Vector3D result = {0.0, 0.0, 0.0};
    if (!validate_vector(a) || !validate_vector(b)) return result;
    result.x = a->x + b->x;
    result.y = a->y + b->y;
    result.z = a->z + b->z;
    return result;
}

struct Vector3D vector_multiply_matrix(const struct Vector3D* v, const struct Matrix3x3* m) {
    struct Vector3D result = {0.0, 0.0, 0.0};
    if (!validate_vector(v) || !validate_matrix(m)) return result;
    result.x = v->x * m->elements[0][0] + v->y * m->elements[1][0] + v->z * m->elements[2][0];
    result.y = v->x * m->elements[0][1] + v->y * m->elements[1][1] + v->z * m->elements[2][1];
    result.z = v->x * m->elements[0][2] + v->y * m->elements[1][2] + v->z * m->elements[2][2];
    return result;
}

struct Vector3D transform_point(const struct Vector3D* point, const struct Transform* transform) {
    struct Vector3D result = {0.0, 0.0, 0.0};
    if (!validate_vector(point) || !validate_transform(transform)) return result;
    struct Vector3D rotated = vector_multiply_matrix(point, &transform->rotation);
    rotated.x *= transform->scale;
    rotated.y *= transform->scale;
    rotated.z *= transform->scale;
    result = vector_add(&rotated, &transform->translation);
    return result;
}

void compute_bounds(struct Geometry* geometry) {
    if (geometry == NULL || geometry->vertices == NULL || geometry->vertex_count == 0) return;
    geometry->bounds.min = geometry->vertices[0];
    geometry->bounds.max = geometry->vertices[0];
    for (uint32_t i = 1; i < geometry->vertex_count; i++) {
        if (!validate_vector(&geometry->vertices[i])) continue;
        if (geometry->vertices[i].x < geometry->bounds.min.x) geometry->bounds.min.x = geometry->vertices[i].x;
        if (geometry->vertices[i].y < geometry->bounds.min.y) geometry->bounds.min.y = geometry->vertices[i].y;
        if (geometry->vertices[i].z < geometry->bounds.min.z) geometry->bounds.min.z = geometry->vertices[i].z;
        if (geometry->vertices[i].x > geometry->bounds.max.x) geometry->bounds.max.x = geometry->vertices[i].x;
        if (geometry->vertices[i].y > geometry->bounds.max.y) geometry->bounds.max.y = geometry->vertices[i].y;
        if (geometry->vertices[i].z > geometry->bounds.max.z) geometry->bounds.max.z = geometry->vertices[i].z;
    }
}

double calculate_volume(const struct BoundingBox* box) {
    if (box == NULL || !validate_vector(&box->min) || !validate_vector(&box->max)) return 0.0;
    double width = box->max.x - box->min.x;
    double height = box->max.y - box->min.y;
    double depth = box->max.z - box->min.z;
    if (width <= 0.0 || height <= 0.0 || depth <= 0.0) return 0.0;
    return width * height * depth;
}

int main(void) {
    struct Geometry cube;
    cube.vertex_count = 8;
    cube.vertices = malloc(sizeof(struct Vector3D) * cube.vertex_count);
    if (