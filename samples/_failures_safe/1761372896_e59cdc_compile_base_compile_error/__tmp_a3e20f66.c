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

struct Vector3D matrix_vector_multiply(struct Matrix3x3 mat, struct Vector3D vec) {
    struct Vector3D result;
    result.x = mat.elements[0][0] * vec.x + mat.elements[0][1] * vec.y + mat.elements[0][2] * vec.z;
    result.y = mat.elements[1][0] * vec.x + mat.elements[1][1] * vec.y + mat.elements[1][2] * vec.z;
    result.z = mat.elements[2][0] * vec.x + mat.elements[2][1] * vec.y + mat.elements[2][2] * vec.z;
    return result;
}

double vector_magnitude(struct Vector3D vec) {
    return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

struct TransformResult transform_vector(struct Vector3D vec, double angle) {
    struct TransformResult result;
    result.original = vec;
    result.valid = 1;
    
    if (!isfinite(vec.x) || !isfinite(vec.y) || !isfinite(vec.z)) {
        result.valid = 0;
        return result;
    }
    
    if (fabs(vec.x) > 1e100 || fabs(vec.y) > 1e100 || fabs(vec.z) > 1e100) {
        result.valid = 0;
        return result;
    }
    
    struct Matrix3x3 rot_mat = create_rotation_matrix(angle);
    result.transformed = matrix_vector_multiply(rot_mat, vec);
    
    if (!isfinite(result.transformed.x) || !isfinite(result.transformed.y) || !isfinite(result.transformed.z)) {
        result.valid = 0;
        return result;
    }
    
    result.magnitude = vector_magnitude(result.transformed);
    if (!isfinite(result.magnitude)) {
        result.valid = 0;
    }
    
    return result;
}

int main(void) {
    double x, y, z, angle;
    char input[256];
    
    printf("Enter vector x coordinate: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    if (sscanf(input, "%lf", &x) != 1) {
        printf("Invalid x coordinate\n");
        return 1;
    }
    
    printf("Enter vector y coordinate: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    if (sscanf(input, "%lf", &y) != 1) {
        printf("Invalid y coordinate\n");
        return 1;
    }
    
    printf("Enter vector z coordinate: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    if (sscanf(input, "%lf", &z) != 1) {
        printf("Invalid z coordinate\n");
        return 1;
    }
    
    printf("Enter rotation angle in radians: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    if (sscanf(input, "%lf", &angle) != 1) {
        printf("Invalid angle\n");
        return 1;
    }
    
    if (!isfinite(angle)) {
        printf("Invalid angle value\n");
        return 1;
    }
    
    struct Vector3D input_vec = create_vector(x, y, z);
    struct TransformResult result = transform_vector(input_vec, angle);
    
    if (!result.valid) {
        printf("Transformation failed: invalid input or numerical error\n");
        return 1;
    }
    
    printf("Original vector: (%.6f, %.6f, %.6f)\n", 
           result.original.x, result.original.y, result.original.z);
    printf("Transformed vector: (%.6f, %.6f, %.6f)\n", 
           result.transformed.x, result.transformed.y,