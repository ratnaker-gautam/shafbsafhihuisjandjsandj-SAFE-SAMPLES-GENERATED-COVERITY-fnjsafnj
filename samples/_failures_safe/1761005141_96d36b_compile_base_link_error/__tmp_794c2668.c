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

struct TransformResult apply_transform(struct Vector3D vec, double angle) {
    struct TransformResult res;
    res.original = vec;
    res.magnitude_original = vector_magnitude(vec);
    struct Matrix3x3 rot_mat = create_rotation_matrix(angle);
    res.transformed = matrix_vector_multiply(rot_mat, vec);
    res.magnitude_transformed = vector_magnitude(res.transformed);
    return res;
}

int validate_input(double x, double y, double z, double angle) {
    if (isnan(x) || isnan(y) || isnan(z) || isnan(angle)) {
        return 0;
    }
    if (isinf(x) || isinf(y) || isinf(z) || isinf(angle)) {
        return 0;
    }
    return 1;
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
        printf("Invalid input for rotation angle.\n");
        return 1;
    }
    
    if (!validate_input(input_x, input_y, input_z, input_angle)) {
        printf("Input contains invalid numeric values.\n");
        return 1;
    }
    
    struct Vector3D input_vector = create_vector(input_x, input_y, input_z);
    struct TransformResult result = apply_transform(input_vector, input_angle);
    
    printf("\nOriginal vector: (%.6f, %.6f, %.6f)\n", 
           result.original.x, result.original.y, result.original.z);
    printf("Original magnitude: %.6f\n", result.magnitude_original);
    printf("Transformed vector: (%.6f, %.6f, %.6f)\n", 
           result.transformed.x, result.transformed.y, result.transformed.z);
    printf("Transformed magnitude: %.6f\n", result.magnitude_transformed);
    printf("Magnitude preserved: %s\n", 
           fabs(result.magnitude_original - result.magnitude_transformed) < 1e-10 ? "Yes" : "No");
    
    return 0;
}