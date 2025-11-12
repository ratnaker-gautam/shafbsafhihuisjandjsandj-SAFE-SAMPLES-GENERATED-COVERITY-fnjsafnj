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
    return 1;
}

int main(void) {
    struct Vector3D vectors[5];
    int vector_count = 0;
    
    printf("Enter 3D vectors (x y z). Enter non-numeric to stop.\n");
    
    for (int i = 0; i < 5; i++) {
        double x, y, z;
        printf("Vector %d:\n", i + 1);
        if (!read_double("  X: ", &x)) break;
        if (!read_double("  Y: ", &y)) break;
        if (!read_double("  Z: ", &z)) break;
        
        vectors[vector_count] = vector3d_create(x, y, z);
        vector_count++;
    }
    
    if (vector_count == 0) {
        printf("No valid vectors entered.\n");
        return 1;
    }
    
    double angle;
    printf("Enter rotation angle in radians: ");
    if (scanf("%lf", &angle) != 1) {
        printf("Invalid angle.\n");
        return 1;
    }
    
    struct Matrix3x3 rotation_matrix = matrix3x3_create_rotation_x(angle);
    
    printf("\nVector Transform Results:\n");
    printf("Index | Original (x,y,z) | Transformed (x,y,z) | Orig Mag | Trans Mag\n");
    printf("------|------------------|---------------------|----------|----------\n");
    
    for (int i = 0; i < vector_count; i++) {
        struct TransformResult result = apply_transform(vectors[i], rotation_matrix);
        printf("%5d | %6.2f %6.2f %6.2f | %6.2f %6.2f %6.2f | %8.2f | %8.2f\n",
               i + 1,
               result.original.x, result.original.y, result.original.z,
               result.transformed.x, result.transformed.y, result.transformed.z,
               result.magnitude_original, result.magnitude_transformed);
    }
    
    return 0;
}