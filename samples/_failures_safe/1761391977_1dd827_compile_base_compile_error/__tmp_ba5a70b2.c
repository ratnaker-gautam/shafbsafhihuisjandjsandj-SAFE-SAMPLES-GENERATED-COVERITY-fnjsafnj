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
    double c = cos(angle);
    double s = sin(angle);
    m.elements[1][1] = c;
    m.elements[1][2] = -s;
    m.elements[2][1] = s;
    m.elements[2][2] = c;
    return m;
}

double vector3d_magnitude(struct Vector3D v) {
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

struct Vector3D matrix3x3_multiply_vector(struct Matrix3x3 m, struct Vector3D v) {
    struct Vector3D result;
    result.x = m.elements[0][0] * v.x + m.elements[0][1] * v.y + m.elements[0][2] * v.z;
    result.y = m.elements[1][0] * v.x + m.elements[1][1] * v.y + m.elements[1][2] * v.z;
    result.z = m.elements[2][0] * v.x + m.elements[2][1] * v.y + m.elements[2][2] * v.z;
    return result;
}

struct TransformResult apply_transform(struct Vector3D v, struct Matrix3x3 m) {
    struct TransformResult result;
    result.original = v;
    result.transformed = matrix3x3_multiply_vector(m, v);
    result.magnitude_original = vector3d_magnitude(v);
    result.magnitude_transformed = vector3d_magnitude(result.transformed);
    return result;
}

int read_double(const char* prompt, double* value) {
    char buffer[256];
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    char* endptr;
    *value = strtod(buffer, &endptr);
    if (endptr == buffer) {
        return 0;
    }
    while (*endptr == ' ' || *endptr == '\t') {
        endptr++;
    }
    if (*endptr != '\n' && *endptr != '\0') {
        return 0;
    }
    return 1;
}

int main(void) {
    struct Vector3D vectors[3];
    double angles[3];
    
    printf("3D Vector Transformation Analysis\n");
    printf("Enter 3 vectors (x y z coordinates):\n");
    
    for (int i = 0; i < 3; i++) {
        double x, y, z;
        printf("Vector %d:\n", i + 1);
        if (!read_double("  X: ", &x)) {
            printf("Invalid input for X coordinate\n");
            return 1;
        }
        if (!read_double("  Y: ", &y)) {
            printf("Invalid input for Y coordinate\n");
            return 1;
        }
        if (!read_double("  Z: ", &z)) {
            printf("Invalid input for Z coordinate\n");
            return 1;
        }
        vectors[i] = vector3d_create(x, y, z);
    }
    
    printf("\nEnter rotation angles (in radians) for X, Y, Z axes:\n");
    for (int i = 0; i < 3; i++) {
        const char* axes[] = {"X", "Y", "Z"};
        if (!read_double(axes[i], &angles[i])) {
            printf("Invalid input for %s angle\n", axes[i]);
            return 1;
        }
    }
    
    struct Matrix3x3 rotation_x = matrix3x3_create_rotation_x(angles[0]);
    
    printf("\nTransformation Results:\n");
    printf("Rotation matrix (X-axis):\n");
    for (int i = 0; i < 3; i++) {
        printf("  [");
        for (int j = 0; j < 3; j++) {
            printf("%8.4f", rotation_x.elements[i][j]);
            if (j < 2) printf(" ");
        }
        printf("]\n");
    }
    
    printf("\nVector transformations:\n");
    for (int i = 0; i < 3; i++) {
        struct TransformResult result = apply_transform(vectors[i], rotation_x);
        printf("