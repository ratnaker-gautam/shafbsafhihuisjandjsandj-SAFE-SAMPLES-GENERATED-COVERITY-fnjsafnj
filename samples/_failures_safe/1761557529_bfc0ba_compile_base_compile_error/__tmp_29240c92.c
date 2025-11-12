//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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
};

struct BoundingBox {
    struct Vector3D min;
    struct Vector3D max;
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

struct Vector3D vector_cross(struct Vector3D a, struct Vector3D b) {
    struct Vector3D result;
    result.x = a.y * b.z - a.z * b.y;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;
    return result;
}

double vector_dot(struct Vector3D a, struct Vector3D b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
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
    return vector_add(rotated, t.translation);
}

struct BoundingBox transform_bounding_box(struct Transform t, struct BoundingBox box) {
    struct Vector3D corners[8];
    corners[0] = (struct Vector3D){box.min.x, box.min.y, box.min.z};
    corners[1] = (struct Vector3D){box.min.x, box.min.y, box.max.z};
    corners[2] = (struct Vector3D){box.min.x, box.max.y, box.min.z};
    corners[3] = (struct Vector3D){box.min.x, box.max.y, box.max.z};
    corners[4] = (struct Vector3D){box.max.x, box.min.y, box.min.z};
    corners[5] = (struct Vector3D){box.max.x, box.min.y, box.max.z};
    corners[6] = (struct Vector3D){box.max.x, box.max.y, box.min.z};
    corners[7] = (struct Vector3D){box.max.x, box.max.y, box.max.z};
    
    struct BoundingBox result;
    struct Vector3D first = transform_point(t, corners[0]);
    result.min.x = result.max.x = first.x;
    result.min.y = result.max.y = first.y;
    result.min.z = result.max.z = first.z;
    
    for (int i = 1; i < 8; i++) {
        struct Vector3D transformed = transform_point(t, corners[i]);
        if (transformed.x < result.min.x) result.min.x = transformed.x;
        if (transformed.y < result.min.y) result.min.y = transformed.y;
        if (transformed.z < result.min.z) result.min.z = transformed.z;
        if (transformed.x > result.max.x) result.max.x = transformed.x;
        if (transformed.y > result.max.y) result.max.y = transformed.y;
        if (transformed.z > result.max.z) result.max.z = transformed.z;
    }
    
    return result;
}

double calculate_volume(struct BoundingBox box) {
    double dx = box.max.x - box.min.x;
    double dy = box.max.y - box.min.y;
    double dz = box.max.z - box.min.z;
    if (dx <= 0.0 || dy <= 0.0 || dz <= 0.0) return 0.0;
    return dx * dy * dz;
}

int main(void) {
    struct BoundingBox original_box;
    printf("Enter original bounding box (min_x min_y min_z max_x max_y max_z): ");
    if (scanf("%lf %lf %lf %lf %lf %lf", 
              &original_box.min.x, &original_box.min.y, &original_box.min.z,
              &original_box.max.x, &original_box.max.y, &original_box.max.z) != 6) {
        printf("Invalid input format\n");
        return 1;
    }
    
    if (original_box.min.x >= original_box.max.x || 
        original_box.min.y >= original_box.max.y || 
        original_box.min.z >= original_box.max.z) {
        printf("Invalid bounding box: