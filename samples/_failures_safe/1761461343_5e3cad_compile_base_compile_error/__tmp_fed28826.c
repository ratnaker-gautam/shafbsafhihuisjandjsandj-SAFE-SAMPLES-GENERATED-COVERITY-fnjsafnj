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
    double dot_product;
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
    if (sum < 0.0 || sum > DBL_MAX) {
        return 0.0;
    }
    return sqrt(sum);
}

double dot_product(struct Vector3D a, struct Vector3D b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

struct Vector3D matrix_vector_multiply(struct Matrix3x3 mat, struct Vector3D vec) {
    struct Vector3D result;
    result.x = mat.elements[0][0] * vec.x + mat.elements[0][1] * vec.y + mat.elements[0][2] * vec.z;
    result.y = mat.elements[1][0] * vec.x + mat.elements[1][1] * vec.y + mat.elements[1][2] * vec.z;
    result.z = mat.elements[2][0] * vec.x + mat.elements[2][1] * vec.y + mat.elements[2][2] * vec.z;
    return result;
}

int validate_vector_input(double x, double y, double z) {
    if (x < -1e100 || x > 1e100) return 0;
    if (y < -1e100 || y > 1e100) return 0;
    if (z < -1e100 || z > 1e100) return 0;
    return 1;
}

int validate_angle(double angle) {
    if (angle < -100.0 || angle > 100.0) return 0;
    return 1;
}

struct TransformResult perform_transformation(struct Vector3D vec, double angle) {
    struct TransformResult result;
    result.original = vec;
    struct Matrix3x3 rot_mat = create_rotation_matrix(angle);
    result.transformed = matrix_vector_multiply(rot_mat, vec);
    result.magnitude = vector_magnitude(vec);
    struct Vector3D unit_x = create_vector(1.0, 0.0, 0.0);
    result.dot_product = dot_product(vec, unit_x);
    return result;
}

void print_vector(struct Vector3D vec) {
    printf("(%.3f, %.3f, %.3f)", vec.x, vec.y, vec.z);
}

int main(void) {
    double input_x, input_y, input_z, input_angle;
    
    printf("Enter vector x coordinate: ");
    if (scanf("%lf", &input_x) != 1) {
        printf("Invalid input for x coordinate.\n");
        return 1;
    }
    
    printf("Enter vector y coordinate: ");
    if (scanf("%lf", &input_y) != 1) {
        printf("Invalid input for y coordinate.\n");
        return 1;
    }
    
    printf("Enter vector z coordinate: ");
    if (scanf("%lf", &input_z) != 1) {
        printf("Invalid input for z coordinate.\n");
        return 1;
    }
    
    printf("Enter rotation angle in radians: ");
    if (scanf("%lf", &input_angle) != 1) {
        printf("Invalid input for angle.\n");
        return 1;
    }
    
    if (!validate_vector_input(input_x, input_y, input_z)) {
        printf("Vector coordinates out of valid range.\n");
        return 1;
    }
    
    if (!validate_angle(input_angle)) {
        printf("Angle out of valid range.\n");
        return 1;
    }
    
    struct Vector3D input_vector = create_vector(input_x, input_y, input_z);
    struct TransformResult result = perform_transformation(input_vector, input_angle);
    
    printf("\nTransformation Results:\n");
    printf("Original vector: ");
    print_vector(result.original);
    printf("\n");
    
    printf("Transformed vector: ");
    print_vector(result.transformed);
    printf("\n");
    
    printf("Vector magnitude: %.3f\n", result.magn