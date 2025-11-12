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

int is_valid_vector(struct Vector3D vec) {
    return !isnan(vec.x) && !isnan(vec.y) && !isnan(vec.z) &&
           !isinf(vec.x) && !isinf(vec.y) && !isinf(vec.z);
}

struct TransformResult perform_transformation(struct Vector3D vec, double angle) {
    struct TransformResult result;
    result.original = vec;
    result.valid = 0;
    
    if (!is_valid_vector(vec)) {
        return result;
    }
    
    if (isnan(angle) || isinf(angle)) {
        return result;
    }
    
    struct Matrix3x3 rotation_mat = create_rotation_matrix(angle);
    result.transformed = matrix_vector_multiply(rotation_mat, vec);
    
    if (!is_valid_vector(result.transformed)) {
        return result;
    }
    
    result.magnitude = vector_magnitude(result.transformed);
    if (isnan(result.magnitude) || isinf(result.magnitude)) {
        return result;
    }
    
    result.valid = 1;
    return result;
}

void print_vector(struct Vector3D vec) {
    printf("(%.3f, %.3f, %.3f)", vec.x, vec.y, vec.z);
}

int main() {
    struct Vector3D test_vectors[5];
    double angles[5];
    const double PI = 3.14159265358979323846;
    
    test_vectors[0] = create_vector(1.0, 0.0, 0.0);
    test_vectors[1] = create_vector(0.0, 1.0, 0.0);
    test_vectors[2] = create_vector(1.0, 1.0, 1.0);
    test_vectors[3] = create_vector(2.5, -1.5, 0.8);
    test_vectors[4] = create_vector(-1.0, -2.0, 3.0);
    
    angles[0] = 0.0;
    angles[1] = PI / 4.0;
    angles[2] = PI / 2.0;
    angles[3] = PI;
    angles[4] = 3.0 * PI / 2.0;
    
    printf("Vector Transformation Results:\n");
    printf("==============================\n");
    
    for (int i = 0; i < 5; i++) {
        struct TransformResult result = perform_transformation(test_vectors[i], angles[i]);
        
        printf("Test %d:\n", i + 1);
        printf("Original: ");
        print_vector(result.original);
        printf("\n");
        
        if (result.valid) {
            printf("Transformed: ");
            print_vector(result.transformed);
            printf("\n");
            printf("Magnitude: %.3f\n", result.magnitude);
            printf("Angle: %.3f radians\n", angles[i]);
        } else {
            printf("Transformation failed - invalid input or numerical error\n");
        }
        printf("\n");
    }
    
    printf("Enter a vector (x y z): ");
    struct Vector3D user_vec;
    if (scanf("%lf %lf %lf", &user_vec.x, &user_vec.y, &user_vec.z) !=