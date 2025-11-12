//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>
#include <float.h>

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
    double magnitude;
    int valid;
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
    double sum = vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
    if (sum > DBL_MAX) {
        return -1.0;
    }
    return sqrt(sum);
}

struct Vector3D matrix_vector_multiply(struct Matrix3x3 mat, struct Vector3D vec) {
    struct Vector3D result;
    result.x = mat.elements[0][0] * vec.x + mat.elements[0][1] * vec.y + mat.elements[0][2] * vec.z;
    result.y = mat.elements[1][0] * vec.x + mat.elements[1][1] * vec.y + mat.elements[1][2] * vec.z;
    result.z = mat.elements[2][0] * vec.x + mat.elements[2][1] * vec.y + mat.elements[2][2] * vec.z;
    return result;
}

int validate_vector(struct Vector3D vec) {
    if (vec.x != vec.x || vec.y != vec.y || vec.z != vec.z) {
        return 0;
    }
    if (vec.x > DBL_MAX || vec.x < -DBL_MAX) {
        return 0;
    }
    if (vec.y > DBL_MAX || vec.y < -DBL_MAX) {
        return 0;
    }
    if (vec.z > DBL_MAX || vec.z < -DBL_MAX) {
        return 0;
    }
    return 1;
}

struct TransformResult transform_vector(struct Vector3D vec, double angle) {
    struct TransformResult result;
    result.valid = 0;
    result.original = vec;
    result.transformed = create_vector(0.0, 0.0, 0.0);
    result.magnitude = 0.0;

    if (!validate_vector(vec)) {
        return result;
    }
    if (angle != angle || angle > 1000.0 || angle < -1000.0) {
        return result;
    }

    struct Matrix3x3 rot_mat = create_rotation_matrix(angle);
    struct Vector3D transformed = matrix_vector_multiply(rot_mat, vec);
    
    if (!validate_vector(transformed)) {
        return result;
    }

    double mag = vector_magnitude(transformed);
    if (mag < 0.0) {
        return result;
    }

    result.transformed = transformed;
    result.magnitude = mag;
    result.valid = 1;
    return result;
}

int main(void) {
    double x, y, z, angle;
    
    printf("Enter vector x coordinate: ");
    if (scanf("%lf", &x) != 1) {
        printf("Invalid input for x coordinate.\n");
        return 1;
    }
    
    printf("Enter vector y coordinate: ");
    if (scanf("%lf", &y) != 1) {
        printf("Invalid input for y coordinate.\n");
        return 1;
    }
    
    printf("Enter vector z coordinate: ");
    if (scanf("%lf", &z) != 1) {
        printf("Invalid input for z coordinate.\n");
        return 1;
    }
    
    printf("Enter rotation angle in radians: ");
    if (scanf("%lf", &angle) != 1) {
        printf("Invalid input for angle.\n");
        return 1;
    }

    struct Vector3D input_vector = create_vector(x, y, z);
    struct TransformResult result = transform_vector(input_vector, angle);

    if (!result.valid) {
        printf("Error: Invalid input or computation failed.\n");
        return 1;
    }

    printf("Original vector: (%f, %f, %f)\n", result.original.x, result.original.y, result.original.z);
    printf("Transformed vector: (%f, %f, %f)\n", result.transformed.x, result.transformed.y, result.transformed.z);
    printf("Magnitude of transformed vector: %f\n", result.magnitude);

    return 0;
}