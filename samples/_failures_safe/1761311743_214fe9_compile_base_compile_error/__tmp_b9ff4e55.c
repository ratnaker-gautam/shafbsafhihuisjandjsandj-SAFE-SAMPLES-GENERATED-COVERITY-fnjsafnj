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

struct Vector3D vector_add(struct Vector3D a, struct Vector3D b) {
    struct Vector3D result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    return result;
}

struct Vector3D vector_subtract(struct Vector3D a, struct Vector3D b) {
    struct Vector3D result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    return result;
}

double vector_dot_product(struct Vector3D a, struct Vector3D b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

struct Vector3D vector_cross_product(struct Vector3D a, struct Vector3D b) {
    struct Vector3D result;
    result.x = a.y * b.z - a.z * b.y;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;
    return result;
}

double vector_magnitude(struct Vector3D v) {
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

struct Matrix3x3 matrix_create_rotation_x(double angle) {
    struct Matrix3x3 m;
    double c = cos(angle);
    double s = sin(angle);
    m.elements[0][0] = 1.0; m.elements[0][1] = 0.0; m.elements[0][2] = 0.0;
    m.elements[1][0] = 0.0; m.elements[1][1] = c;   m.elements[1][2] = -s;
    m.elements[2][0] = 0.0; m.elements[2][1] = s;   m.elements[2][2] = c;
    return m;
}

struct Vector3D matrix_vector_multiply(struct Matrix3x3 m, struct Vector3D v) {
    struct Vector3D result;
    result.x = m.elements[0][0] * v.x + m.elements[0][1] * v.y + m.elements[0][2] * v.z;
    result.y = m.elements[1][0] * v.x + m.elements[1][1] * v.y + m.elements[1][2] * v.z;
    result.z = m.elements[2][0] * v.x + m.elements[2][1] * v.y + m.elements[2][2] * v.z;
    return result;
}

int validate_vector_input(double x, double y, double z) {
    if (x != x || y != y || z != z) return 0;
    if (x > 1e100 || x < -1e100) return 0;
    if (y > 1e100 || y < -1e100) return 0;
    if (z > 1e100 || z < -1e100) return 0;
    return 1;
}

int validate_angle(double angle) {
    if (angle != angle) return 0;
    if (angle > 100.0 || angle < -100.0) return 0;
    return 1;
}

int main(void) {
    struct Vector3D vec1, vec2;
    double angle;
    
    printf("Enter first vector (x y z): ");
    if (scanf("%lf %lf %lf", &vec1.x, &vec1.y, &vec1.z) != 3) {
        printf("Invalid input for first vector\n");
        return 1;
    }
    
    if (!validate_vector_input(vec1.x, vec1.y, vec1.z)) {
        printf("Invalid vector values\n");
        return 1;
    }
    
    printf("Enter second vector (x y z): ");
    if (scanf("%lf %lf %lf", &vec2.x, &vec2.y, &vec2.z) != 3) {
        printf("Invalid input for second vector\n");
        return 1;
    }
    
    if (!validate_vector_input(vec2.x, vec2.y, vec2.z)) {
        printf("Invalid vector values\n");
        return 1;
    }
    
    printf("Enter rotation angle in radians: ");
    if (scanf("%lf", &angle) != 1) {
        printf("Invalid input for angle\n");
        return 1;
    }
    
    if (!validate_angle(angle)) {
        printf("Invalid angle value\n");
        return 1;
    }
    
    struct Vector3D sum = vector_add(vec1, vec2);
    struct Vector3D diff = vector_subtract(vec1, vec2);
    double dot = vector_dot_product(vec1, vec2);
    struct Vector3D cross = vector_cross_product(vec1, vec2);
    double mag1 = vector_magnitude(vec1);
    double mag2 = vector_magnitude(vec2);
    
    struct Matrix3x3 rot_x =