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

struct Vector3D matrix3x3_multiply_vector(struct Matrix3x3 m, struct Vector3D v) {
    struct Vector3D result;
    result.x = m.elements[0][0] * v.x + m.elements[0][1] * v.y + m.elements[0][2] * v.z;
    result.y = m.elements[1][0] * v.x + m.elements[1][1] * v.y + m.elements[1][2] * v.z;
    result.z = m.elements[2][0] * v.x + m.elements[2][1] * v.y + m.elements[2][2] * v.z;
    return result;
}

double vector3d_magnitude(struct Vector3D v) {
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

struct TransformResult compute_transform(struct Vector3D v, double angle) {
    struct TransformResult result;
    result.original = v;
    struct Matrix3x3 rotation = matrix3x3_create_rotation_x(angle);
    result.transformed = matrix3x3_multiply_vector(rotation, v);
    result.magnitude_original = vector3d_magnitude(v);
    result.magnitude_transformed = vector3d_magnitude(result.transformed);
    return result;
}

int main() {
    double x, y, z, angle;
    printf("Enter vector coordinates (x y z): ");
    if (scanf("%lf %lf %lf", &x, &y, &z) != 3) {
        printf("Invalid input for vector coordinates.\n");
        return 1;
    }
    
    printf("Enter rotation angle in radians: ");
    if (scanf("%lf", &angle) != 1) {
        printf("Invalid input for angle.\n");
        return 1;
    }
    
    if (!isfinite(x) || !isfinite(y) || !isfinite(z) || !isfinite(angle)) {
        printf("Input values must be finite numbers.\n");
        return 1;
    }
    
    struct Vector3D input_vector = vector3d_create(x, y, z);
    struct TransformResult result = compute_transform(input_vector, angle);
    
    printf("\nOriginal vector: (%.3f, %.3f, %.3f)\n", result.original.x, result.original.y, result.original.z);
    printf("Original magnitude: %.3f\n", result.magnitude_original);
    printf("Transformed vector: (%.3f, %.3f, %.3f)\n", result.transformed.x, result.transformed.y, result.transformed.z);
    printf("Transformed magnitude: %.3f\n", result.magnitude_transformed);
    
    double magnitude_diff = fabs(result.magnitude_original - result.magnitude_transformed);
    if (magnitude_diff > 1e-10) {
        printf("Warning: Magnitude changed by %.6f (should be zero for rotation)\n", magnitude_diff);
    } else {
        printf("Rotation preserved magnitude correctly.\n");
    }
    
    return 0;
}