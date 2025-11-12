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

struct Vector3D vector3d_create(double x, double y, double z) {
    struct Vector3D v;
    v.x = x;
    v.y = y;
    v.z = z;
    return v;
}

struct Matrix3x3 matrix3x3_create_rotation_x(double angle) {
    struct Matrix3x3 m;
    double c = cos(angle);
    double s = sin(angle);
    m.elements[0][0] = 1.0; m.elements[0][1] = 0.0; m.elements[0][2] = 0.0;
    m.elements[1][0] = 0.0; m.elements[1][1] = c;   m.elements[1][2] = -s;
    m.elements[2][0] = 0.0; m.elements[2][1] = s;   m.elements[2][2] = c;
    return m;
}

double vector3d_magnitude(struct Vector3D v) {
    double sum = v.x * v.x + v.y * v.y + v.z * v.z;
    if (sum < 0.0) return 0.0;
    return sqrt(sum);
}

struct Vector3D matrix3x3_multiply_vector(struct Matrix3x3 m, struct Vector3D v) {
    struct Vector3D result;
    result.x = m.elements[0][0] * v.x + m.elements[0][1] * v.y + m.elements[0][2] * v.z;
    result.y = m.elements[1][0] * v.x + m.elements[1][1] * v.y + m.elements[1][2] * v.z;
    result.z = m.elements[2][0] * v.x + m.elements[2][1] * v.y + m.elements[2][2] * v.z;
    return result;
}

int validate_vector_input(const char *input, double *x, double *y, double *z) {
    if (input == NULL) return 0;
    int count = sscanf(input, "%lf %lf %lf", x, y, z);
    return count == 3;
}

int validate_angle_input(const char *input, double *angle) {
    if (input == NULL) return 0;
    int count = sscanf(input, "%lf", angle);
    return count == 1;
}

void print_vector(struct Vector3D v) {
    printf("(%.3f, %.3f, %.3f)", v.x, v.y, v.z);
}

void print_matrix(struct Matrix3x3 m) {
    printf("[[%.3f, %.3f, %.3f]\n", m.elements[0][0], m.elements[0][1], m.elements[0][2]);
    printf(" [%.3f, %.3f, %.3f]\n", m.elements[1][0], m.elements[1][1], m.elements[1][2]);
    printf(" [%.3f, %.3f, %.3f]]", m.elements[2][0], m.elements[2][1], m.elements[2][2]);
}

int main(void) {
    char input_buffer[256];
    double x, y, z, angle;
    struct Vector3D original_vector;
    struct Matrix3x3 rotation_matrix;
    struct TransformResult result;
    
    printf("Enter 3D vector coordinates (x y z): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading vector input.\n");
        return 1;
    }
    
    if (!validate_vector_input(input_buffer, &x, &y, &z)) {
        printf("Invalid vector input. Please enter three numbers.\n");
        return 1;
    }
    
    printf("Enter rotation angle in radians: ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading angle input.\n");
        return 1;
    }
    
    if (!validate_angle_input(input_buffer, &angle)) {
        printf("Invalid angle input. Please enter a number.\n");
        return 1;
    }
    
    original_vector = vector3d_create(x, y, z);
    rotation_matrix = matrix3x3_create_rotation_x(angle);
    
    result.original = original_vector;
    result.transformed = matrix3x3_multiply_vector(rotation_matrix, original_vector);
    result.magnitude_original = vector3d_magnitude(original_vector);
    result.magnitude_transformed = vector3d_magnitude(result.transformed);
    
    printf("\nOriginal vector: ");
    print_vector(result.original);
    printf("\nMagnitude: %.3f\n", result.magnitude_original);
    
    printf("\nRotation matrix (X-axis):\n");
    print_matrix(rotation_matrix);
    printf("\n");
    
    printf("\nTransformed vector: ");
    print_vector(result.transformed);
    printf("\nMagnitude: %.3f\n", result.magnitude_transformed);