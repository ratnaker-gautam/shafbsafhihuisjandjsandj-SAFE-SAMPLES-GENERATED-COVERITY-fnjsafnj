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

struct Geometry {
    struct Vector3D* vertices;
    size_t vertex_count;
    struct BoundingBox bounds;
};

struct Vector3D vector3d_add(struct Vector3D a, struct Vector3D b) {
    struct Vector3D result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    return result;
}

struct Vector3D vector3d_multiply(struct Vector3D v, double scalar) {
    struct Vector3D result;
    result.x = v.x * scalar;
    result.y = v.y * scalar;
    result.z = v.z * scalar;
    return result;
}

struct Vector3D vector3d_transform(struct Vector3D v, struct Transform t) {
    struct Vector3D result;
    result.x = v.x * t.scale.x * t.rotation.elements[0][0] + 
               v.y * t.scale.y * t.rotation.elements[0][1] + 
               v.z * t.scale.z * t.rotation.elements[0][2] + t.translation.x;
    result.y = v.x * t.scale.x * t.rotation.elements[1][0] + 
               v.y * t.scale.y * t.rotation.elements[1][1] + 
               v.z * t.scale.z * t.rotation.elements[1][2] + t.translation.y;
    result.z = v.x * t.scale.x * t.rotation.elements[2][0] + 
               v.y * t.scale.y * t.rotation.elements[2][1] + 
               v.z * t.scale.z * t.rotation.elements[2][2] + t.translation.z;
    return result;
}

double vector3d_dot(struct Vector3D a, struct Vector3D b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

struct Vector3D vector3d_cross(struct Vector3D a, struct Vector3D b) {
    struct Vector3D result;
    result.x = a.y * b.z - a.z * b.y;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;
    return result;
}

double vector3d_length(struct Vector3D v) {
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

struct Vector3D vector3d_normalize(struct Vector3D v) {
    double len = vector3d_length(v);
    struct Vector3D result;
    if (len > 0.0) {
        result.x = v.x / len;
        result.y = v.y / len;
        result.z = v.z / len;
    } else {
        result.x = 0.0;
        result.y = 0.0;
        result.z = 0.0;
    }
    return result;
}

struct BoundingBox boundingbox_calculate(struct Vector3D* vertices, size_t count) {
    struct BoundingBox box;
    if (count == 0) {
        box.min.x = box.min.y = box.min.z = 0.0;
        box.max.x = box.max.y = box.max.z = 0.0;
        return box;
    }
    box.min.x = box.max.x = vertices[0].x;
    box.min.y = box.max.y = vertices[0].y;
    box.min.z = box.max.z = vertices[0].z;
    for (size_t i = 1; i < count; i++) {
        if (vertices[i].x < box.min.x) box.min.x = vertices[i].x;
        if (vertices[i].x > box.max.x) box.max.x = vertices[i].x;
        if (vertices[i].y < box.min.y) box.min.y = vertices[i].y;
        if (vertices[i].y > box.max.y) box.max.y = vertices[i].y;
        if (vertices[i].z < box.min.z) box.min.z = vertices[i].z;
        if (vertices[i].z > box.max.z) box.max.z = vertices[i].z;
    }
    return box;
}

double boundingbox_volume(struct BoundingBox box) {
    double dx = box.max.x - box.min.x;
    double dy = box.max.y - box.min.y;
    double dz = box.max.z - box.min.z;
    if (dx < 0.0 || dy < 0.0 || dz < 0.0) return 0.0;
    return dx * dy * dz;
}

struct Geometry geometry_create_cube(double size) {
    struct Geometry geom;
    geom.vertex_count = 8;
    geom.vertices = malloc(sizeof(struct Vector3D) * 8);
    if (geom.vertices == NULL) {
        geom.vertex_count = 0;
        return geom;
    }
    double half = size / 2.0;
    geom.vertices[0].