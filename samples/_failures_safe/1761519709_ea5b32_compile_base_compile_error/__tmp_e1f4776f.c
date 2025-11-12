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

int validate_vector(struct Vector3D vec) {
    if (isnan(vec.x) || isnan(vec.y) || isnan(vec.z)) return 0;
    if (isinf(vec.x) || isinf(vec.y) || isinf(vec.z)) return 0;
    return 1;
}

int validate_angle(double angle) {
    if (isnan(angle) || isinf(angle)) return 0;
    return 1;
}

struct TransformResult perform_transform(struct Vector3D vec, double angle) {
    struct TransformResult result;
    result.original = vec;
    result.magnitude_original = vector_magnitude(vec);
    
    struct Matrix3x3 rot_mat = create_rotation_matrix(angle);
    result.transformed = matrix_vector_multiply(rot_mat, vec);
    result.magnitude_transformed = vector_magnitude(result.transformed);
    
    return result;
}

void print_vector(struct Vector3D vec, const char* label) {
    printf("%s: (%.3f, %.3f, %.3f)\n", label, vec.x, vec.y, vec.z);
}

void print_result(struct TransformResult res) {
    printf("=== Vector Transformation Results ===\n");
    print_vector(res.original, "Original vector");
    print_vector(res.transformed, "Transformed vector");
    printf("Original magnitude: %.3f\n", res.magnitude_original);
    printf("Transformed magnitude: %.3f\n", res.magnitude_transformed);
    printf("Magnitude preserved: %s\n", 
           fabs(res.magnitude_original - res.magnitude_transformed) < 1e-10 ? "Yes" : "No");
    printf("\n");
}

int main() {
    struct Vector3D test_vectors[4];
    double angles[4] = {0.0, 3.14159265358979323846/4, 3.14159265358979323846/2, 3.14159265358979323846};
    
    test_vectors[0] = create_vector(1.0, 0.0, 0.0);
    test_vectors[1] = create_vector(0.0, 1.0, 0.0);
    test_vectors[2] = create_vector(1.0, 1.0, 0.0);
    test_vectors[3] = create_vector(2.0, 3.0, 1.0);
    
    for (int i = 0; i < 4; i++) {
        if (!validate_vector(test_vectors[i]) || !validate_angle(angles[i])) {
            printf("Error: Invalid input data for test case %d\n", i);
            continue;
        }
        
        struct TransformResult result = perform_transform(test_vectors[i], angles[i]);
        
        if (!validate_vector(result.transformed)) {
            printf("Error: Transformation produced invalid result for test case %d\n", i);
            continue;
        }
        
        print_result(result);
    }
    
    printf("Enter vector coordinates (x y z): ");
    struct Vector3D user_vec;
    if (scanf("%lf %lf %lf", &user_vec.x, &user_vec.y, &user_vec.z) != 3) {
        printf("Error: Invalid input format\n