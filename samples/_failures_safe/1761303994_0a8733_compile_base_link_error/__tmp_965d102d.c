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

struct TransformResult apply_transform(struct Vector3D vec, double angle) {
    struct TransformResult result;
    result.original = vec;
    result.valid = 1;
    
    double mag = vector_magnitude(vec);
    if (mag > 1e6 || mag < 1e-6) {
        result.valid = 0;
        return result;
    }
    
    struct Matrix3x3 rot_mat = create_rotation_matrix(angle);
    result.transformed = matrix_vector_multiply(rot_mat, vec);
    result.magnitude = vector_magnitude(result.transformed);
    
    return result;
}

int main(void) {
    double x, y, z, angle;
    int read_count;
    
    printf("Enter vector coordinates (x y z): ");
    read_count = scanf("%lf %lf %lf", &x, &y, &z);
    if (read_count != 3) {
        printf("Invalid input for vector coordinates\n");
        return 1;
    }
    
    if (!isfinite(x) || !isfinite(y) || !isfinite(z)) {
        printf("Vector coordinates must be finite numbers\n");
        return 1;
    }
    
    printf("Enter rotation angle in radians: ");
    read_count = scanf("%lf", &angle);
    if (read_count != 1) {
        printf("Invalid input for rotation angle\n");
        return 1;
    }
    
    if (!isfinite(angle)) {
        printf("Rotation angle must be a finite number\n");
        return 1;
    }
    
    struct Vector3D input_vector = create_vector(x, y, z);
    struct TransformResult result = apply_transform(input_vector, angle);
    
    if (!result.valid) {
        printf("Invalid vector magnitude for transformation\n");
        return 1;
    }
    
    printf("Original vector: (%.3f, %.3f, %.3f)\n", 
           result.original.x, result.original.y, result.original.z);
    printf("Transformed vector: (%.3f, %.3f, %.3f)\n", 
           result.transformed.x, result.transformed.y, result.transformed.z);
    printf("Magnitude of transformed vector: %.3f\n", result.magnitude);
    
    return 0;
}