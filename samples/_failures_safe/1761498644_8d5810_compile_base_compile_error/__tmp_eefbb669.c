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
    struct TransformResult result;
    result.original = vec;
    result.magnitude_original = vector_magnitude(vec);
    
    struct Matrix3x3 rotation = create_rotation_matrix(angle);
    result.transformed = matrix_vector_multiply(rotation, vec);
    result.magnitude_transformed = vector_magnitude(result.transformed);
    
    return result;
}

void print_vector(struct Vector3D vec) {
    printf("(%.3f, %.3f, %.3f)", vec.x, vec.y, vec.z);
}

void print_transform_result(struct TransformResult result) {
    printf("Original vector: ");
    print_vector(result.original);
    printf(" (magnitude: %.3f)\n", result.magnitude_original);
    
    printf("Transformed vector: ");
    print_vector(result.transformed);
    printf(" (magnitude: %.3f)\n", result.magnitude_transformed);
}

int main() {
    struct Vector3D test_vectors[5];
    double angles[5];
    const double PI = 3.14159265358979323846;
    
    test_vectors[0] = create_vector(1.0, 0.0, 0.0);
    test_vectors[1] = create_vector(0.0, 1.0, 0.0);
    test_vectors[2] = create_vector(1.0, 1.0, 0.0);
    test_vectors[3] = create_vector(2.0, 3.0, 1.0);
    test_vectors[4] = create_vector(-1.0, 2.0, -0.5);
    
    angles[0] = 0.0;
    angles[1] = PI / 4.0;
    angles[2] = PI / 2.0;
    angles[3] = PI;
    angles[4] = 3.0 * PI / 2.0;
    
    printf("3D Vector Rotation Transform Results\n");
    printf("====================================\n\n");
    
    for (int i = 0; i < 5; i++) {
        printf("Test Case %d:\n", i + 1);
        printf("Rotation angle: %.3f radians\n", angles[i]);
        
        struct TransformResult result = apply_transform(test_vectors[i], angles[i]);
        print_transform_result(result);
        
        double mag_diff = fabs(result.magnitude_original - result.magnitude_transformed);
        if (mag_diff < 1e-10) {
            printf("Magnitude preserved: YES (difference: %.6f)\n", mag_diff);
        } else {
            printf("Magnitude preserved: NO (difference: %.6f)\n", mag_diff);
        }
        
        printf("\n");
    }
    
    return 0;
}