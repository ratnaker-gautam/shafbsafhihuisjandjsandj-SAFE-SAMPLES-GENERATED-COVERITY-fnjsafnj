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

struct Vector3D create_vector(double x, double y, double z) {
    struct Vector3D vec;
    vec.x = x;
    vec.y = y;
    vec.z = z;
    return vec;
}

struct Matrix3x3 create_rotation_matrix(double angle) {
    struct Matrix3x3 mat;
    double cos_a = cos(angle);
    double sin_a = sin(angle);
    
    mat.elements[0][0] = cos_a;
    mat.elements[0][1] = -sin_a;
    mat.elements[0][2] = 0.0;
    mat.elements[1][0] = sin_a;
    mat.elements[1][1] = cos_a;
    mat.elements[1][2] = 0.0;
    mat.elements[2][0] = 0.0;
    mat.elements[2][1] = 0.0;
    mat.elements[2][2] = 1.0;
    
    return mat;
}

double vector_magnitude(struct Vector3D vec) {
    return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

struct Vector3D matrix_vector_multiply(struct Matrix3x3 mat, struct Vector3D vec) {
    struct Vector3D result;
    result.x = mat.elements[0][0] * vec.x + mat.elements[0][1] * vec.y + mat.elements[0][2] * vec.z;
    result.y = mat.elements[1][0] * vec.x + mat.elements[1][1] * vec.y + mat.elements[1][2] * vec.z;
    result.z = mat.elements[2][0] * vec.x + mat.elements[2][1] * vec.y + mat.elements[2][2] * vec.z;
    return result;
}

struct TransformResult apply_transform(struct Vector3D vec, struct Matrix3x3 mat) {
    struct TransformResult res;
    res.original = vec;
    res.transformed = matrix_vector_multiply(mat, vec);
    res.magnitude_original = vector_magnitude(vec);
    res.magnitude_transformed = vector_magnitude(res.transformed);
    return res;
}

int read_double(const char* prompt, double* value) {
    char buffer[100];
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
    struct Vector3D vectors[3];
    double angle;
    
    printf("Vector Transformation Calculator\n");
    printf("Enter rotation angle in radians: ");
    if (!read_double("", &angle)) {
        printf("Invalid angle input.\n");
        return 1;
    }
    
    printf("Enter 3 vectors (x y z coordinates):\n");
    for (int i = 0; i < 3; i++) {
        double x, y, z;
        printf("Vector %d:\n", i + 1);
        if (!read_double("  x: ", &x)) {
            printf("Invalid x coordinate.\n");
            return 1;
        }
        if (!read_double("  y: ", &y)) {
            printf("Invalid y coordinate.\n");
            return 1;
        }
        if (!read_double("  z: ", &z)) {
            printf("Invalid z coordinate.\n");
            return 1;
        }
        vectors[i] = create_vector(x, y, z);
    }
    
    struct Matrix3x3 rotation_mat = create_rotation_matrix(angle);
    struct TransformResult results[3];
    
    printf("\nTransformation Results:\n");
    printf("Rotation Matrix (Z-axis):\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%8.3f ", rotation_mat.elements[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    
    for (int i = 0; i < 3; i++) {
        results[i] = apply_transform(vectors[i], rotation_mat);
        printf("Vector %d:\n", i + 1);
        printf("  Original: (%6.2f, %6.2f, %6.2f) Magnitude: %6.2f\n",
               results[i].original.x, results[i].original.y, results[i].original.z,
               results[i].magnitude_original);
        printf("  Transformed: (%6.2f, %6.2f, %6.2f) Magnitude: %6.2f\n",
               results[i].transformed.x, results[i].transformed.y, results[i].transformed.z,
               results[i].magnitude_transformed);
        printf("\n");
    }
    
    return 0;
}