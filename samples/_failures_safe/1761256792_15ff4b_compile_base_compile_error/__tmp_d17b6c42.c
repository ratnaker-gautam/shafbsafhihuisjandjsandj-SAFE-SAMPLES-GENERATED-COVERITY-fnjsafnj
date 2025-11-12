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
    struct Vector3D scale;
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

int validate_double_input(const char* prompt, double* value) {
    char buffer[256];
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    char* endptr;
    *value = strtod(buffer, &endptr);
    if (endptr == buffer || *endptr != '\n') {
        return 0;
    }
    return 1;
}

void vector3d_add(struct Vector3D* result, const struct Vector3D* a, const struct Vector3D* b) {
    result->x = a->x + b->x;
    result->y = a->y + b->y;
    result->z = a->z + b->z;
}

void vector3d_multiply(struct Vector3D* result, const struct Vector3D* a, double scalar) {
    result->x = a->x * scalar;
    result->y = a->y * scalar;
    result->z = a->z * scalar;
}

double vector3d_dot(const struct Vector3D* a, const struct Vector3D* b) {
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

void vector3d_cross(struct Vector3D* result, const struct Vector3D* a, const struct Vector3D* b) {
    result->x = a->y * b->z - a->z * b->y;
    result->y = a->z * b->x - a->x * b->z;
    result->z = a->x * b->y - a->y * b->x;
}

void matrix3x3_multiply_vector(struct Vector3D* result, const struct Matrix3x3* m, const struct Vector3D* v) {
    result->x = m->elements[0][0] * v->x + m->elements[0][1] * v->y + m->elements[0][2] * v->z;
    result->y = m->elements[1][0] * v->x + m->elements[1][1] * v->y + m->elements[1][2] * v->z;
    result->z = m->elements[2][0] * v->x + m->elements[2][1] * v->y + m->elements[2][2] * v->z;
}

void transform_vector(struct Vector3D* result, const struct Transform* t, const struct Vector3D* v) {
    struct Vector3D scaled, rotated;
    scaled.x = v->x * t->scale.x;
    scaled.y = v->y * t->scale.y;
    scaled.z = v->z * t->scale.z;
    matrix3x3_multiply_vector(&rotated, &t->rotation, &scaled);
    vector3d_add(result, &rotated, &t->translation);
}

void compute_bounding_box(struct BoundingBox* box, const struct Mesh* mesh) {
    if (mesh->vertex_count == 0) {
        box->min.x = box->min.y = box->min.z = 0.0;
        box->max.x = box->max.y = box->max.z = 0.0;
        return;
    }
    box->min.x = box->max.x = mesh->vertices[0].x;
    box->min.y = box->max.y = mesh->vertices[0].y;
    box->min.z = box->max.z = mesh->vertices[0].z;
    for (uint32_t i = 1; i < mesh->vertex_count; i++) {
        if (mesh->vertices[i].x < box->min.x) box->min.x = mesh->vertices[i].x;
        if (mesh->vertices[i].x > box->max.x) box->max.x = mesh->vertices[i].x;
        if (mesh->vertices[i].y < box->min.y) box->min.y = mesh->vertices[i].y;
        if (mesh->vertices[i].y > box->max.y) box->max.y = mesh->vertices[i].y;
        if (mesh->vertices[i].z < box->min.z) box->min.z = mesh->vertices[i].z;
        if (mesh->vertices[i].z > box->max.z) box->max.z = mesh->vertices[i].z;
    }
}

double calculate_mesh_volume(const struct Mesh* mesh) {
    if (mesh->vertex_count < 4) return 0.0;
    double volume = 0.0;
    struct Vector3D center = {0.0, 0.0, 0.0};
    for (uint32_t i = 0; i < mesh->vertex_count; i++) {
        center.x += mesh->vertices[i].x;
        center.y += mesh->vertices