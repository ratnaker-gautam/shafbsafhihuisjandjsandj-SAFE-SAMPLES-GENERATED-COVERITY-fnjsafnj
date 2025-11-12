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

struct TransformResult {
    struct Vector3D original;
    struct Vector3D transformed;
    double magnitude_original;
    double magnitude_transformed;
};

struct Vector3D vector3d_create(double x, double y, double z) {
    struct Vector3D v;
    v.x = x;
    v.y = y;
    v.z = z;
    return v;
}

struct Matrix3x3 matrix3x3_create_identity(void) {
    struct Matrix3x3 m;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            m.elements[i][j] = (i == j) ? 1.0 : 0.0;
        }
    }
    return m;
}

struct Matrix3x3 matrix3x3_create_rotation_x(double angle) {
    struct Matrix3x3 m = matrix3x3_create_identity();
    double cos_a = cos(angle);
    double sin_a = sin(angle);
    m.elements[1][1] = cos_a;
    m.elements[1][2] = -sin_a;
    m.elements[2][1] = sin_a;
    m.elements[2][2] = cos_a;
    return m;
}

struct Matrix3x3 matrix3x3_create_rotation_y(double angle) {
    struct Matrix3x3 m = matrix3x3_create_identity();
    double cos_a = cos(angle);
    double sin_a = sin(angle);
    m.elements[0][0] = cos_a;
    m.elements[0][2] = sin_a;
    m.elements[2][0] = -sin_a;
    m.elements[2][2] = cos_a;
    return m;
}

struct Matrix3x3 matrix3x3_create_rotation_z(double angle) {
    struct Matrix3x3 m = matrix3x3_create_identity();
    double cos_a = cos(angle);
    double sin_a = sin(angle);
    m.elements[0][0] = cos_a;
    m.elements[0][1] = -sin_a;
    m.elements[1][0] = sin_a;
    m.elements[1][1] = cos_a;
    return m;
}

struct Vector3D matrix3x3_multiply_vector(const struct Matrix3x3 *m, const struct Vector3D *v) {
    struct Vector3D result;
    result.x = m->elements[0][0] * v->x + m->elements[0][1] * v->y + m->elements[0][2] * v->z;
    result.y = m->elements[1][0] * v->x + m->elements[1][1] * v->y + m->elements[1][2] * v->z;
    result.z = m->elements[2][0] * v->x + m->elements[2][1] * v->y + m->elements[2][2] * v->z;
    return result;
}

double vector3d_magnitude(const struct Vector3D *v) {
    return sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
}

struct TransformResult transform_vector(const struct Vector3D *v, const struct Matrix3x3 *m) {
    struct TransformResult result;
    result.original = *v;
    result.transformed = matrix3x3_multiply_vector(m, v);
    result.magnitude_original = vector3d_magnitude(v);
    result.magnitude_transformed = vector3d_magnitude(&result.transformed);
    return result;
}

int read_double(const char *prompt, double *value) {
    char buffer[256];
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    char *endptr;
    *value = strtod(buffer, &endptr);
    if (endptr == buffer) {
        return 0;
    }
    return 1;
}

int main(void) {
    struct Vector3D vectors[3];
    double angles[3];
    
    printf("3D Vector Transformation Demo\n");
    printf("Enter 3 vectors (x y z):\n");
    
    for (int i = 0; i < 3; i++) {
        double x, y, z;
        printf("Vector %d:\n", i + 1);
        if (!read_double("  X: ", &x)) {
            printf("Invalid input for X\n");
            return 1;
        }
        if (!read_double("  Y: ", &y)) {
            printf("Invalid input for Y\n");
            return 1;
        }
        if (!read_double("  Z: ", &z)) {
            printf("Invalid input for Z\n");
            return 1;
        }
        vectors[i] = vector3d_create(x, y, z);
    }
    
    printf("\nEnter rotation angles (in radians) for X, Y, Z axes:\n");
    const char *axis_names[] = {"X: ", "Y: ", "Z: "};
    for (int i = 0; i < 3; i++) {