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

int validate_input(double x, double y, double z, double angle) {
    if (isnan(x) || isnan(y) || isnan(z) || isnan(angle)) {
        return 0;
    }
    if (isinf(x) || isinf(y) || isinf(z) || isinf(angle)) {
        return 0;
    }
    if (fabs(x) > 1e100 || fabs(y) > 1e100 || fabs(z) > 1e100 || fabs(angle) > 1e100) {
        return 0;
    }
    return 1;
}

struct TransformResult perform_transform(double x, double y, double z, double angle) {
    struct TransformResult result;
    
    if (!validate_input(x, y, z, angle)) {
        result.original = create_vector(0.0, 0.0, 0.0);
        result.transformed = create_vector(0.0, 0.0, 0.0);
        result.magnitude_original = 0.0;
        result.magnitude_transformed = 0.0;
        return result;
    }
    
    struct Vector3D original_vec = create_vector(x, y, z);
    struct Matrix3x3 rotation_mat = create_rotation_matrix(angle);
    struct Vector3D transformed_vec = matrix_vector_multiply(rotation_mat, original_vec);
    
    result.original = original_vec;
    result.transformed = transformed_vec;
    result.magnitude_original = vector_magnitude(original_vec);
    result.magnitude_transformed = vector_magnitude(transformed_vec);
    
    return result;
}

void print_vector(struct Vector3D vec, const char* label) {
    printf("%s: (%.6f, %.6f, %.6f)\n", label, vec.x, vec.y, vec.z);
}

int main(void) {
    double input_x, input_y, input_z, input_angle;
    
    printf("Enter vector coordinates (x y z): ");
    if (scanf("%lf %lf %lf", &input_x, &input_y, &input_z) != 3) {
        printf("Invalid input for vector coordinates.\n");
        return 1;
    }
    
    printf("Enter rotation angle in radians: ");
    if (scanf("%lf", &input_angle) != 1) {
        printf("Invalid input for rotation angle.\n");
        return 1;
    }
    
    struct TransformResult result = perform_transform(input_x, input_y, input_z, input_angle);
    
    printf("\nTransformation Results:\n");
    print_vector(result.original, "Original vector");
    print_vector(result.transformed, "Transformed vector");
    printf("Original magnitude: %.6f\n", result.magnitude_original);
    printf("Transformed magnitude: %.6f\n", result.magnitude_transformed);
    
    if (fabs(result.magnitude_original - result.magnitude_transformed) < 1e-10) {
        printf("Magnitude preserved (rotation successful).\n");
    } else {
        printf("Magnitude not preserved (numerical error).\n");
    }
    
    return 0;
}