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

struct Vector3D vector_add(struct Vector3D a, struct Vector3D b) {
    struct Vector3D result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    return result;
}

struct Vector3D vector_subtract(struct Vector3D a, struct Vector3D b) {
    struct Vector3D result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    return result;
}

struct Vector3D vector_scale(struct Vector3D v, double s) {
    struct Vector3D result;
    result.x = v.x * s;
    result.y = v.y * s;
    result.z = v.z * s;
    return result;
}

double vector_dot(struct Vector3D a, struct Vector3D b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

struct Vector3D vector_cross(struct Vector3D a, struct Vector3D b) {
    struct Vector3D result;
    result.x = a.y * b.z - a.z * b.y;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;
    return result;
}

struct Vector3D matrix_vector_multiply(struct Matrix3x3 m, struct Vector3D v) {
    struct Vector3D result;
    result.x = m.elements[0][0] * v.x + m.elements[0][1] * v.y + m.elements[0][2] * v.z;
    result.y = m.elements[1][0] * v.x + m.elements[1][1] * v.y + m.elements[1][2] * v.z;
    result.z = m.elements[2][0] * v.x + m.elements[2][1] * v.y + m.elements[2][2] * v.z;
    return result;
}

struct Vector3D transform_point(struct Transform t, struct Vector3D point) {
    struct Vector3D rotated = matrix_vector_multiply(t.rotation, point);
    struct Vector3D scaled = vector_scale(rotated, t.scale);
    return vector_add(scaled, t.translation);
}

void update_bounding_box(struct BoundingBox* box, struct Vector3D point) {
    if (point.x < box->min.x) box->min.x = point.x;
    if (point.y < box->min.y) box->min.y = point.y;
    if (point.z < box->min.z) box->min.z = point.z;
    if (point.x > box->max.x) box->max.x = point.x;
    if (point.y > box->max.y) box->max.y = point.y;
    if (point.z > box->max.z) box->max.z = point.z;
}

int initialize_mesh(struct Mesh* mesh, uint32_t vertex_count) {
    if (vertex_count == 0 || vertex_count > 1000000) {
        return 0;
    }
    mesh->vertices = malloc(vertex_count * sizeof(struct Vector3D));
    if (mesh->vertices == NULL) {
        return 0;
    }
    mesh->vertex_count = vertex_count;
    mesh->bounds.min.x = INFINITY;
    mesh->bounds.min.y = INFINITY;
    mesh->bounds.min.z = INFINITY;
    mesh->bounds.max.x = -INFINITY;
    mesh->bounds.max.y = -INFINITY;
    mesh->bounds.max.z = -INFINITY;
    return 1;
}

void compute_mesh_bounds(struct Mesh* mesh) {
    if (mesh->vertices == NULL || mesh->vertex_count == 0) {
        return;
    }
    mesh->bounds.min.x = INFINITY;
    mesh->bounds.min.y = INFINITY;
    mesh->bounds.min.z = INFINITY;
    mesh->bounds.max.x = -INFINITY;
    mesh->bounds.max.y = -INFINITY;
    mesh->bounds.max.z = -INFINITY;
    for (uint32_t i = 0; i < mesh->vertex_count; i++) {
        update_bounding_box(&mesh->bounds, mesh->vertices[i]);
    }
}

double calculate_mesh_volume(struct Mesh mesh) {
    if (mesh.vertex_count < 4 || mesh.vertices == NULL) {
        return 0.0;
    }
    struct Vector3D center;
    center.x = (mesh.bounds.min.x + mesh.bounds.max.x) / 2.0;
    center.y = (mesh.bounds.min.y + mesh.bounds.max.y) / 2.0;
    center.z = (mesh.bounds.min.z + mesh.bounds.max.z) / 2.0;
    double total_volume =