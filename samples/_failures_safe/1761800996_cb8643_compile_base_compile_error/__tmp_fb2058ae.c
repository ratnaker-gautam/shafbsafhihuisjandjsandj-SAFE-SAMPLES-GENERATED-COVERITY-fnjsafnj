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

struct Mesh {
    struct Vector3D* vertices;
    uint32_t vertex_count;
    struct BoundingBox bounds;
};

int validate_double(double value) {
    if (value != value) return 0;
    if (value * 0 != 0) return 0;
    return 1;
}

struct Vector3D vector_add(struct Vector3D a, struct Vector3D b) {
    struct Vector3D result;
    if (!validate_double(a.x) || !validate_double(b.x) ||
        !validate_double(a.y) || !validate_double(b.y) ||
        !validate_double(a.z) || !validate_double(b.z)) {
        result.x = result.y = result.z = 0.0;
        return result;
    }
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    return result;
}

struct Vector3D vector_scale(struct Vector3D v, double scalar) {
    struct Vector3D result;
    if (!validate_double(v.x) || !validate_double(v.y) || 
        !validate_double(v.z) || !validate_double(scalar)) {
        result.x = result.y = result.z = 0.0;
        return result;
    }
    result.x = v.x * scalar;
    result.y = v.y * scalar;
    result.z = v.z * scalar;
    return result;
}

double vector_dot(struct Vector3D a, struct Vector3D b) {
    if (!validate_double(a.x) || !validate_double(b.x) ||
        !validate_double(a.y) || !validate_double(b.y) ||
        !validate_double(a.z) || !validate_double(b.z)) {
        return 0.0;
    }
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

struct Vector3D vector_cross(struct Vector3D a, struct Vector3D b) {
    struct Vector3D result;
    if (!validate_double(a.x) || !validate_double(b.x) ||
        !validate_double(a.y) || !validate_double(b.y) ||
        !validate_double(a.z) || !validate_double(b.z)) {
        result.x = result.y = result.z = 0.0;
        return result;
    }
    result.x = a.y * b.z - a.z * b.y;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;
    return result;
}

struct Matrix3x3 matrix_multiply(struct Matrix3x3 a, struct Matrix3x3 b) {
    struct Matrix3x3 result;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result.elements[i][j] = 0.0;
            for (int k = 0; k < 3; k++) {
                if (!validate_double(a.elements[i][k]) || 
                    !validate_double(b.elements[k][j])) {
                    result.elements[i][j] = 0.0;
                    continue;
                }
                result.elements[i][j] += a.elements[i][k] * b.elements[k][j];
            }
        }
    }
    return result;
}

struct Vector3D transform_point(struct Transform t, struct Vector3D point) {
    struct Vector3D result;
    if (!validate_double(t.scale)) {
        result.x = result.y = result.z = 0.0;
        return result;
    }
    
    struct Vector3D scaled = vector_scale(point, t.scale);
    
    struct Vector3D rotated;
    rotated.x = t.rotation.elements[0][0] * scaled.x + 
                t.rotation.elements[0][1] * scaled.y + 
                t.rotation.elements[0][2] * scaled.z;
    rotated.y = t.rotation.elements[1][0] * scaled.x + 
                t.rotation.elements[1][1] * scaled.y + 
                t.rotation.elements[1][2] * scaled.z;
    rotated.z = t.rotation.elements[2][0] * scaled.x + 
                t.rotation.elements[2][1] * scaled.y + 
                t.rotation.elements[2][2] * scaled.z;
    
    result = vector_add(rotated, t.translation);
    return result;
}

void compute_bounds(struct Mesh* mesh) {
    if (mesh == NULL || mesh->vertices == NULL || mesh->vertex_count == 0) {
        return;
    }
    
    mesh->bounds.min = mesh->vertices[0];
    mesh->bounds.max = mesh->vertices[0];
    
    for (uint32_t i = 1; i < mesh->vertex_count; i++) {
        if (mesh->vertices[i].x < mesh->bounds.min.x) mesh->bounds.min.x