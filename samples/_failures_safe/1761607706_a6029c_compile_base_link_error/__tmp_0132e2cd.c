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

struct Matrix3x3 matrix3x3_create_identity(void) {
    struct Matrix3x3 m;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            m.elements[i][j] = (i == j) ? 1.0 : 0.0;
        }
    }
    return m;
}

struct Matrix3x3 matrix3x3_create_rotation_x(double angle) {
    struct Matrix3x3 m = matrix3x3_create_identity();
    double c = cos(angle);
    double s = sin(angle);
    m.elements[1][1] = c;
    m.elements[1][2] = -s;
    m.elements[2][1] = s;
    m.elements[2][2] = c;
    return m;
}

double vector3d_magnitude(struct Vector3D v) {
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

struct Vector3D matrix3x3_multiply_vector(struct Matrix3x3 m, struct Vector3D v) {
    struct Vector3D result;
    result.x = m.elements[0][0] * v.x + m.elements[0][1] * v.y + m.elements[0][2] * v.z;
    result.y = m.elements[1][0] * v.x + m.elements[1][1] * v.y + m.elements[1][2] * v.z;
    result.z = m.elements[2][0] * v.x + m.elements[2][1] * v.y + m.elements[2][2] * v.z;
    return result;
}

struct TransformResult apply_transform(struct Vector3D v, struct Matrix3x3 m) {
    struct TransformResult result;
    result.original = v;
    result.transformed = matrix3x3_multiply_vector(m, v);
    result.magnitude_original = vector3d_magnitude(v);
    result.magnitude_transformed = vector3d_magnitude(result.transformed);
    return result;
}

int validate_vector_input(double x, double y, double z) {
    if (isnan(x) || isnan(y) || isnan(z)) return 0;
    if (isinf(x) || isinf(y) || isinf(z)) return 0;
    return 1;
}

int validate_angle(double angle) {
    if (isnan(angle) || isinf(angle)) return 0;
    return 1;
}

int main(void) {
    double input_x, input_y, input_z, input_angle;
    
    printf("Enter vector x coordinate: ");
    if (scanf("%lf", &input_x) != 1) {
        printf("Invalid input for x coordinate.\n");
        return 1;
    }
    
    printf("Enter vector y coordinate: ");
    if (scanf("%lf", &input_y) != 1) {
        printf("Invalid input for y coordinate.\n");
        return 1;
    }
    
    printf("Enter vector z coordinate: ");
    if (scanf("%lf", &input_z) != 1) {
        printf("Invalid input for z coordinate.\n");
        return 1;
    }
    
    if (!validate_vector_input(input_x, input_y, input_z)) {
        printf("Invalid vector coordinates.\n");
        return 1;
    }
    
    printf("Enter rotation angle in radians: ");
    if (scanf("%lf", &input_angle) != 1) {
        printf("Invalid input for angle.\n");
        return 1;
    }
    
    if (!validate_angle(input_angle)) {
        printf("Invalid angle value.\n");
        return 1;
    }
    
    struct Vector3D original_vector = vector3d_create(input_x, input_y, input_z);
    struct Matrix3x3 rotation_matrix = matrix3x3_create_rotation_x(input_angle);
    struct TransformResult result = apply_transform(original_vector, rotation_matrix);
    
    printf("\nOriginal vector: (%.6f, %.6f, %.6f)\n", 
           result.original.x, result.original.y, result.original.z);
    printf("Original magnitude: %.6f\n", result.magnitude_original);
    
    printf("Transformed vector: (%.6f, %.6f, %.6f)\n", 
           result.transformed.x, result.transformed.y, result.transformed.z);
    printf("Transformed magnitude: %.6f\n", result.magnitude_transformed);
    
    double magnitude_change = fabs(result.magnitude_transformed - result.magnitude_original);
    printf("Magnitude change: %.6f\n", magnitude_change);
    
    return 0;
}