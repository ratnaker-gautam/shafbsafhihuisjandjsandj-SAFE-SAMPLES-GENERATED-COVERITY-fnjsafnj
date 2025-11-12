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
    return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

double vector_dot_product(struct Vector3D a, struct Vector3D b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

struct Vector3D matrix_vector_multiply(struct Matrix3x3 mat, struct Vector3D vec) {
    struct Vector3D result;
    result.x = mat.elements[0][0] * vec.x + mat.elements[0][1] * vec.y + mat.elements[0][2] * vec.z;
    result.y = mat.elements[1][0] * vec.x + mat.elements[1][1] * vec.y + mat.elements[1][2] * vec.z;
    result.z = mat.elements[2][0] * vec.x + mat.elements[2][1] * vec.y + mat.elements[2][2] * vec.z;
    return result;
}

struct TransformResult perform_transform(struct Vector3D vec, struct Matrix3x3 mat) {
    struct TransformResult result;
    result.original = vec;
    result.transformed = matrix_vector_multiply(mat, vec);
    result.magnitude = vector_magnitude(result.transformed);
    result.dot_product = vector_dot_product(vec, result.transformed);
    return result;
}

int main() {
    double input_x, input_y, input_z, angle;
    printf("Enter vector coordinates (x y z): ");
    if (scanf("%lf %lf %lf", &input_x, &input_y, &input_z) != 3) {
        printf("Invalid input for vector coordinates.\n");
        return 1;
    }
    
    printf("Enter rotation angle in radians: ");
    if (scanf("%lf", &angle) != 1) {
        printf("Invalid input for angle.\n");
        return 1;
    }
    
    if (!isfinite(input_x) || !isfinite(input_y) || !isfinite(input_z) || !isfinite(angle)) {
        printf("Input contains non-finite values.\n");
        return 1;
    }
    
    struct Vector3D original_vector = create_vector(input_x, input_y, input_z);
    struct Matrix3x3 rotation_matrix = create_rotation_matrix(angle);
    struct TransformResult result = perform_transform(original_vector, rotation_matrix);
    
    printf("Original vector: (%.3f, %.3f, %.3f)\n", original_vector.x, original_vector.y, original_vector.z);
    printf("Transformed vector: (%.3f, %.3f, %.3f)\n", result.transformed.x, result.transformed.y, result.transformed.z);
    printf("Magnitude of transformed vector: %.3f\n", result.magnitude);
    printf("Dot product (original • transformed): %.3f\n", result.dot_product);
    
    return 0;
}