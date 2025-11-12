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

struct GeometricTransform {
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

int validate_transform(const struct GeometricTransform* t) {
    if (t == NULL) return 0;
    if (!validate_matrix(&t->rotation)) return 0;
    if (!validate_vector(&t->translation)) return 0;
    if (!isfinite(t->scale) || t->scale <= 0.0) return 0;
    return 1;
}

struct Vector3D transform_point(const struct GeometricTransform* t, const struct Vector3D* p) {
    struct Vector3D result = {0.0, 0.0, 0.0};
    if (!validate_transform(t) || !validate_vector(p)) return result;
    
    result.x = t->scale * (t->rotation.elements[0][0] * p->x + t->rotation.elements[0][1] * p->y + t->rotation.elements[0][2] * p->z) + t->translation.x;
    result.y = t->scale * (t->rotation.elements[1][0] * p->x + t->rotation.elements[1][1] * p->y + t->rotation.elements[1][2] * p->z) + t->translation.y;
    result.z = t->scale * (t->rotation.elements[2][0] * p->x + t->rotation.elements[2][1] * p->y + t->rotation.elements[2][2] * p->z) + t->translation.z;
    
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
    
    mesh->bounds.min.x = mesh->bounds.max.x = mesh->vertices[0].x;
    mesh->bounds.min.y = mesh->bounds.max.y = mesh->vertices[0].y;
    mesh->bounds.min.z = mesh->bounds.max.z = mesh->vertices[0].z;
    
    for (uint32_t i = 1; i < mesh->vertex_count; i++) {
        if (mesh->vertices[i].x < mesh->bounds.min.x) mesh->bounds.min.x = mesh->vertices[i].x;
        if (mesh->vertices[i].x > mesh->bounds.max.x) mesh->bounds.max.x = mesh->vertices[i].x;
        if (mesh->vertices[i].y < mesh->bounds.min.y) mesh->bounds.min.y = mesh->vertices[i].y;
        if (mesh->vertices[i].y > mesh->bounds.max.y) mesh->bounds.max.y = mesh->vertices[i].y;
        if (mesh->vertices[i].z < mesh->bounds.min.z) mesh->bounds.min.z = mesh->vertices[i].z;
        if (mesh->vertices[i].z > mesh->bounds.max.z) mesh->bounds.max.z = mesh->vertices[i].z;
    }
}

void apply_transform(struct Mesh* mesh, const struct GeometricTransform* transform) {
    if (mesh == NULL || mesh->vertices == NULL || !validate_transform(transform)) return;
    
    for (uint32_t i = 0; i < mesh->vertex_count; i++) {
        if (!validate_vector(&mesh->vertices[i])) continue;
        mesh->vertices[i] = transform_point(transform, &mesh->vertices[i]);
    }
    
    compute_bounds(mesh);
}

double calculate_volume(const struct BoundingBox* box) {
    if (box == NULL || !validate_vector(&box->min) || !validate_vector(&box->max)) return 0.0;
    
    double width = box->max.x - box->min.x;
    double height = box->max.y