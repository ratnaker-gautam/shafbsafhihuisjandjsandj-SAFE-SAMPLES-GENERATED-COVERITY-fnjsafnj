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

struct BoundingBox {
    struct Vector3D min;
    struct Vector3D max;
};

struct Mesh {
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

struct Vector3D transform_vector(const struct Vector3D* v, const struct Transform* t) {
    struct Vector3D result = {0.0, 0.0, 0.0};
    if (!validate_vector(v) || !validate_transform(t)) return result;
    struct Vector3D rotated = vector_multiply_matrix(v, &t->rotation);
    struct Vector3D scaled = {rotated.x * t->scale, rotated.y * t->scale, rotated.z * t->scale};
    result = vector_add(&scaled, &t->translation);
    return result;
}

int initialize_mesh(struct Mesh* mesh, uint32_t vertex_count) {
    if (mesh == NULL || vertex_count == 0 || vertex_count > 1000000) return 0;
    mesh->vertices = malloc(vertex_count * sizeof(struct Vector3D));
    if (mesh->vertices == NULL) return 0;
    mesh->vertex_count = vertex_count;
    mesh->bounds.min.x = mesh->bounds.min.y = mesh->bounds.min.z = 0.0;
    mesh->bounds.max.x = mesh->bounds.max.y = mesh->bounds.max.z = 0.0;
    return 1;
}

void compute_bounds(struct Mesh* mesh) {
    if (mesh == NULL || mesh->vertices == NULL || mesh->vertex_count == 0) return;
    if (!validate_vector(&mesh->vertices[0])) return;
    mesh->bounds.min = mesh->vertices[0];
    mesh->bounds.max = mesh->vertices[0];
    for (uint32_t i = 1; i < mesh->vertex_count; i++) {
        if (!validate_vector(&mesh->vertices[i])) continue;
        if (mesh->vertices[i].x < mesh->bounds.min.x) mesh->bounds.min.x = mesh->vertices[i].x;
        if (mesh->vertices[i].y < mesh->bounds.min.y) mesh->bounds.min.y = mesh->vertices[i].y;
        if (mesh->vertices[i].z < mesh->bounds.min.z) mesh->bounds.min.z = mesh->vertices[i].z;
        if (mesh->vertices[i].x > mesh->bounds.max.x) mesh->bounds.max.x = mesh->vertices[i].x;
        if (mesh->vertices[i].y > mesh->bounds.max.y) mesh->bounds.max.y = mesh->vertices[i].y;
        if (mesh->vertices[i].z > mesh->bounds.max.z) mesh->bounds.max.z = mesh->vertices[i].z;
    }
}

void apply_transform_to_mesh(struct Mesh* mesh, const