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

struct TransformResult apply_transform(struct Vector3D vec, double angle) {
    struct TransformResult res;
    res.original = vec;
    res.magnitude_original = vector_magnitude(vec);
    
    struct Matrix3x3 rot_mat = create_rotation_matrix(angle);
    res.transformed = matrix_vector_multiply(rot_mat, vec);
    res.magnitude_transformed = vector_magnitude(res.transformed);
    
    return res;
}

int read_double(const char* prompt, double* value) {
    printf("%s", prompt);
    if (scanf("%lf", value) != 1) {
        return 0;
    }
    return 1;
}

int main(void) {
    struct Vector3D input_vector;
    double rotation_angle;
    
    printf("3D Vector Rotation Calculator\n");
    printf("Enter vector components:\n");
    
    if (!read_double("X: ", &input_vector.x)) {
        printf("Invalid input for X component\n");
        return 1;
    }
    
    if (!read_double("Y: ", &input_vector.y)) {
        printf("Invalid input for Y component\n");
        return 1;
    }
    
    if (!read_double("Z: ", &input_vector.z)) {
        printf("Invalid input for Z component\n");
        return 1;
    }
    
    if (!read_double("Rotation angle (radians): ", &rotation_angle)) {
        printf("Invalid input for rotation angle\n");
        return 1;
    }
    
    if (isnan(rotation_angle) || isinf(rotation_angle)) {
        printf("Invalid rotation angle\n");
        return 1;
    }
    
    struct TransformResult result = apply_transform(input_vector, rotation_angle);
    
    printf("\nResults:\n");
    printf("Original vector: (%.3f, %.3f, %.3f)\n", 
           result.original.x, result.original.y, result.original.z);
    printf("Original magnitude: %.3f\n", result.magnitude_original);
    printf("Transformed vector: (%.3f, %.3f, %.3f)\n", 
           result.transformed.x, result.transformed.y, result.transformed.z);
    printf("Transformed magnitude: %.3f\n", result.magnitude_transformed);
    
    double mag_diff = fabs(result.magnitude_original - result.magnitude_transformed);
    if (mag_diff > 1e-10) {
        printf("Warning: Magnitude changed by %.10f (should be preserved)\n", mag_diff);
    } else {
        printf("Magnitude preserved (rotation matrix is orthogonal)\n");
    }
    
    return 0;
}