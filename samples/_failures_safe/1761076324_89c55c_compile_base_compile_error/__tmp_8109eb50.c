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

struct Vector3D vector_add(struct Vector3D a, struct Vector3D b) {
    struct Vector3D result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    return result;
}

struct Vector3D vector_scale(struct Vector3D v, double scalar) {
    struct Vector3D result;
    result.x = v.x * scalar;
    result.y = v.y * scalar;
    result.z = v.z * scalar;
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

double vector_length(struct Vector3D v) {
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

struct Matrix3x3 matrix_multiply(struct Matrix3x3 a, struct Matrix3x3 b) {
    struct Matrix3x3 result;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result.elements[i][j] = 0.0;
            for (int k = 0; k < 3; k++) {
                result.elements[i][j] += a.elements[i][k] * b.elements[k][j];
            }
        }
    }
    return result;
}

struct Vector3D transform_point(struct Transform t, struct Vector3D point) {
    struct Vector3D rotated;
    rotated.x = t.rotation.elements[0][0] * point.x + t.rotation.elements[0][1] * point.y + t.rotation.elements[0][2] * point.z;
    rotated.y = t.rotation.elements[1][0] * point.x + t.rotation.elements[1][1] * point.y + t.rotation.elements[1][2] * point.z;
    rotated.z = t.rotation.elements[2][0] * point.x + t.rotation.elements[2][1] * point.y + t.rotation.elements[2][2] * point.z;
    
    struct Vector3D scaled = vector_scale(rotated, t.scale);
    return vector_add(scaled, t.translation);
}

void compute_bounds(struct Mesh* mesh) {
    if (mesh->vertex_count == 0) {
        mesh->bounds.min.x = mesh->bounds.min.y = mesh->bounds.min.z = 0.0;
        mesh->bounds.max.x = mesh->bounds.max.y = mesh->bounds.max.z = 0.0;
        return;
    }
    
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

int main(void) {
    struct Mesh mesh;
    printf("Enter number of vertices (1-1000): ");
    char buffer[256];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL)