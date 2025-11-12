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

int is_valid_vector(struct Vector3D vec) {
    if (vec.x != vec.x || vec.y != vec.y || vec.z != vec.z) {
        return 0;
    }
    if (fabs(vec.x) > 1e100 || fabs(vec.y) > 1e100 || fabs(vec.z) > 1e100) {
        return 0;
    }
    return 1;
}

struct TransformResult transform_vector(struct Vector3D vec, double angle) {
    struct TransformResult result;
    result.original = vec;
    result.valid = 0;
    
    if (!is_valid_vector(vec)) {
        return result;
    }
    
    if (angle != angle || fabs(angle) > 1000.0) {
        return result;
    }
    
    struct Matrix3x3 rot_mat = create_rotation_matrix(angle);
    struct Vector3D transformed = matrix_vector_multiply(rot_mat, vec);
    
    if (!is_valid_vector(transformed)) {
        return result;
    }
    
    result.transformed = transformed;
    result.magnitude = vector_magnitude(transformed);
    result.valid = 1;
    
    return result;
}

void print_vector(struct Vector3D vec) {
    printf("(%.3f, %.3f, %.3f)", vec.x, vec.y, vec.z);
}

int main() {
    struct Vector3D test_vectors[5];
    double test_angles[5];
    const double PI = 3.14159265358979323846;
    
    test_vectors[0] = create_vector(1.0, 0.0, 0.0);
    test_vectors[1] = create_vector(0.0, 1.0, 0.0);
    test_vectors[2] = create_vector(1.0, 1.0, 1.0);
    test_vectors[3] = create_vector(2.5, -1.5, 0.8);
    test_vectors[4] = create_vector(-1.0, -2.0, 3.0);
    
    test_angles[0] = 0.0;
    test_angles[1] = PI / 2.0;
    test_angles[2] = PI;
    test_angles[3] = -PI / 4.0;
    test_angles[4] = 2.0 * PI / 3.0;
    
    printf("3D Vector Transformation Results:\n");
    printf("================================\n");
    
    for (int i = 0; i < 5; i++) {
        struct TransformResult result = transform_vector(test_vectors[i], test_angles[i]);
        
        printf("Test %d:\n", i + 1);
        printf("Original vector: ");
        print_vector(result.original);
        printf("\n");
        
        if (result.valid) {
            printf("Rotation angle: %.3f radians\n", test_angles[i]);
            printf("Transformed vector: ");
            print_vector(result.transformed);
            printf("\n");
            printf("Magnitude: %.3f\n", result.magnitude);
        } else {
            printf("Transformation failed - invalid input\n");
        }
        printf("\n");
    }
    
    return 0;
}