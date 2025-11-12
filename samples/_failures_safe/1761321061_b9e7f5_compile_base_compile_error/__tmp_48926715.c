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

struct Matrix3x3 create_rotation_matrix(double angle_x, double angle_y, double angle_z) {
    struct Matrix3x3 mat;
    double cx = cos(angle_x);
    double sx = sin(angle_x);
    double cy = cos(angle_y);
    double sy = sin(angle_y);
    double cz = cos(angle_z);
    double sz = sin(angle_z);
    
    mat.elements[0][0] = cy * cz;
    mat.elements[0][1] = -cy * sz;
    mat.elements[0][2] = sy;
    
    mat.elements[1][0] = sx * sy * cz + cx * sz;
    mat.elements[1][1] = -sx * sy * sz + cx * cz;
    mat.elements[1][2] = -sx * cy;
    
    mat.elements[2][0] = -cx * sy * cz + sx * sz;
    mat.elements[2][1] = cx * sy * sz + sx * cz;
    mat.elements[2][2] = cx * cy;
    
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

int validate_angles(double angle_x, double angle_y, double angle_z) {
    if (angle_x < -6.28318530718 || angle_x > 6.28318530718) return 0;
    if (angle_y < -6.28318530718 || angle_y > 6.28318530718) return 0;
    if (angle_z < -6.28318530718 || angle_z > 6.28318530718) return 0;
    return 1;
}

int validate_vector(struct Vector3D vec) {
    if (vec.x < -1e6 || vec.x > 1e6) return 0;
    if (vec.y < -1e6 || vec.y > 1e6) return 0;
    if (vec.z < -1e6 || vec.z > 1e6) return 0;
    return 1;
}

struct TransformResult perform_transformation(struct Vector3D vec, double angle_x, double angle_y, double angle_z) {
    struct TransformResult result;
    result.original = vec;
    result.magnitude_original = vector_magnitude(vec);
    
    struct Matrix3x3 rotation = create_rotation_matrix(angle_x, angle_y, angle_z);
    result.transformed = matrix_vector_multiply(rotation, vec);
    result.magnitude_transformed = vector_magnitude(result.transformed);
    
    return result;
}

int main() {
    struct Vector3D input_vector;
    double angle_x, angle_y, angle_z;
    
    printf("Enter vector coordinates (x y z): ");
    if (scanf("%lf %lf %lf", &input_vector.x, &input_vector.y, &input_vector.z) != 3) {
        printf("Invalid vector input\n");
        return 1;
    }
    
    if (!validate_vector(input_vector)) {
        printf("Vector values out of range\n");
        return 1;
    }
    
    printf("Enter rotation angles in radians (x y z): ");
    if (scanf("%lf %lf %lf", &angle_x, &angle_y, &angle_z) != 3) {
        printf("Invalid angle input\n");
        return 1;
    }
    
    if (!validate_angles(angle_x, angle_y, angle_z)) {
        printf("Angle values out of range (-2π to 2π)\n");
        return 1;
    }
    
    struct TransformResult result = perform_transformation(input_vector, angle_x, angle_y, angle_z);
    
    printf("\nOriginal vector: (%.3f, %.3f, %.3f)\n", result.original.x, result.original.y, result.original.z);
    printf("Original magnitude: %.3f\n", result.magnitude_original);
    printf("Transformed vector: (%.3f, %.3f, %.3f)\n", result.transformed.x, result.transformed.y, result.transformed.z);
    printf("Transformed magnitude: %.3