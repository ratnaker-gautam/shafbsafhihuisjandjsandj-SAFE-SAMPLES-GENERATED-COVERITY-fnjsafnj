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

int validate_input(double x, double y, double z, double angle) {
    if (x < -1e6 || x > 1e6) return 0;
    if (y < -1e6 || y > 1e6) return 0;
    if (z < -1e6 || z > 1e6) return 0;
    if (angle < -6.28318530718 || angle > 6.28318530718) return 0;
    return 1;
}

void print_vector(struct Vector3D vec) {
    printf("(%.3f, %.3f, %.3f)", vec.x, vec.y, vec.z);
}

int main() {
    double input_x, input_y, input_z, input_angle;
    
    printf("Enter vector coordinates (x y z): ");
    if (scanf("%lf %lf %lf", &input_x, &input_y, &input_z) != 3) {
        printf("Invalid input format\n");
        return 1;
    }
    
    printf("Enter rotation angle in radians: ");
    if (scanf("%lf", &input_angle) != 1) {
        printf("Invalid angle input\n");
        return 1;
    }
    
    if (!validate_input(input_x, input_y, input_z, input_angle)) {
        printf("Input values out of valid range\n");
        return 1;
    }
    
    struct Vector3D original_vector = create_vector(input_x, input_y, input_z);
    struct Matrix3x3 rotation_matrix = create_rotation_matrix(input_angle);
    struct Vector3D transformed_vector = matrix_vector_multiply(rotation_matrix, original_vector);
    
    struct TransformResult result;
    result.original = original_vector;
    result.transformed = transformed_vector;
    result.magnitude_original = vector_magnitude(original_vector);
    result.magnitude_transformed = vector_magnitude(transformed_vector);
    
    printf("\nOriginal vector: ");
    print_vector(result.original);
    printf(" (magnitude: %.3f)\n", result.magnitude_original);
    
    printf("Transformed vector: ");
    print_vector(result.transformed);
    printf(" (magnitude: %.3f)\n", result.magnitude_transformed);
    
    printf("Magnitude preserved: %s\n", 
           fabs(result.magnitude_original - result.magnitude_transformed) < 1e-9 ? "Yes" : "No");
    
    return 0;
}