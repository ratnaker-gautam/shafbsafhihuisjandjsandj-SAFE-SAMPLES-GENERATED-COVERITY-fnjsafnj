//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: numeric_computation
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

double vector_magnitude(struct Vector3D v) {
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

struct Vector3D matrix_vector_multiply(struct Matrix3x3 m, struct Vector3D v) {
    struct Vector3D result;
    result.x = m.elements[0][0] * v.x + m.elements[0][1] * v.y + m.elements[0][2] * v.z;
    result.y = m.elements[1][0] * v.x + m.elements[1][1] * v.y + m.elements[1][2] * v.z;
    result.z = m.elements[2][0] * v.x + m.elements[2][1] * v.y + m.elements[2][2] * v.z;
    return result;
}

struct Transform transform_compose(struct Transform a, struct Transform b) {
    struct Transform result;
    result.rotation = matrix_multiply(a.rotation, b.rotation);
    result.translation = vector_add(matrix_vector_multiply(a.rotation, b.translation), a.translation);
    return result;
}

struct Vector3D transform_apply(struct Transform t, struct Vector3D v) {
    return vector_add(matrix_vector_multiply(t.rotation, v), t.translation);
}

int read_double(double *value) {
    char buffer[64];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    if (sscanf(buffer, "%lf", value) != 1) {
        return 0;
    }
    return 1;
}

int main(void) {
    struct Vector3D point;
    struct Transform transform1, transform2, combined;
    
    printf("Enter a 3D point (x y z): ");
    if (!read_double(&point.x) || !read_double(&point.y) || !read_double(&point.z)) {
        printf("Invalid input for point.\n");
        return 1;
    }
    
    printf("Enter first transform rotation matrix (9 values, row-major):\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (!read_double(&transform1.rotation.elements[i][j])) {
                printf("Invalid input for rotation matrix.\n");
                return 1;
            }
        }
    }
    
    printf("Enter first transform translation vector (x y z): ");
    if (!read_double(&transform1.translation.x) || !read_double(&transform1.translation.y) || !read_double(&transform1.translation.z)) {
        printf("Invalid input for translation vector.\n");
        return 1;
    }
    
    printf("Enter second transform rotation matrix (9 values, row-major):\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (!read_double(&transform2.rotation.elements[i][j])) {
                printf("Invalid input for rotation matrix.\n");
                return 1;
            }
        }
    }
    
    printf("Enter second transform translation vector (x y z): ");
    if (!read_double(&transform2.translation.x) || !read_double(&transform2.translation.y) || !read_double(&transform2.translation.z)) {
        printf("Invalid input for