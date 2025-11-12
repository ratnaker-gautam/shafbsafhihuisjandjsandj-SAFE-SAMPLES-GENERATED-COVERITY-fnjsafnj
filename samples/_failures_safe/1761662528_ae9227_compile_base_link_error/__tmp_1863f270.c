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
    struct Vector3D vec;
    vec.x = x;
    vec.y = y;
    vec.z = z;
    return vec;
}

struct Matrix3x3 matrix3x3_create_rotation_x(double angle) {
    struct Matrix3x3 mat;
    double cos_a = cos(angle);
    double sin_a = sin(angle);
    mat.elements[0][0] = 1.0;
    mat.elements[0][1] = 0.0;
    mat.elements[0][2] = 0.0;
    mat.elements[1][0] = 0.0;
    mat.elements[1][1] = cos_a;
    mat.elements[1][2] = -sin_a;
    mat.elements[2][0] = 0.0;
    mat.elements[2][1] = sin_a;
    mat.elements[2][2] = cos_a;
    return mat;
}

double vector3d_magnitude(struct Vector3D vec) {
    return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

struct Vector3D matrix3x3_multiply_vector(struct Matrix3x3 mat, struct Vector3D vec) {
    struct Vector3D result;
    result.x = mat.elements[0][0] * vec.x + mat.elements[0][1] * vec.y + mat.elements[0][2] * vec.z;
    result.y = mat.elements[1][0] * vec.x + mat.elements[1][1] * vec.y + mat.elements[1][2] * vec.z;
    result.z = mat.elements[2][0] * vec.x + mat.elements[2][1] * vec.y + mat.elements[2][2] * vec.z;
    return result;
}

struct TransformResult compute_transform(struct Vector3D vec, double angle) {
    struct TransformResult result;
    result.original = vec;
    result.magnitude_original = vector3d_magnitude(vec);
    
    struct Matrix3x3 rotation = matrix3x3_create_rotation_x(angle);
    result.transformed = matrix3x3_multiply_vector(rotation, vec);
    result.magnitude_transformed = vector3d_magnitude(result.transformed);
    
    return result;
}

int read_double(const char* prompt, double* value) {
    char buffer[256];
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    char* endptr;
    *value = strtod(buffer, &endptr);
    if (endptr == buffer) {
        return 0;
    }
    return 1;
}

int main(void) {
    struct Vector3D vectors[3];
    double angles[3];
    
    printf("3D Vector Rotation Analysis\n");
    printf("Enter 3 vectors and rotation angles:\n");
    
    for (int i = 0; i < 3; i++) {
        double x, y, z, angle;
        printf("\nVector %d:\n", i + 1);
        
        if (!read_double("  X: ", &x)) {
            printf("Invalid input for X\n");
            return 1;
        }
        if (!read_double("  Y: ", &y)) {
            printf("Invalid input for Y\n");
            return 1;
        }
        if (!read_double("  Z: ", &z)) {
            printf("Invalid input for Z\n");
            return 1;
        }
        if (!read_double("  Rotation angle (radians): ", &angle)) {
            printf("Invalid input for angle\n");
            return 1;
        }
        
        vectors[i] = vector3d_create(x, y, z);
        angles[i] = angle;
    }
    
    printf("\nResults:\n");
    printf("Vec | Original (x,y,z) | Transformed (x,y,z) | Orig Mag | Trans Mag\n");
    printf("----|-------------------|---------------------|----------|----------\n");
    
    for (int i = 0; i < 3; i++) {
        struct TransformResult result = compute_transform(vectors[i], angles[i]);
        printf("%3d | %6.2f %6.2f %6.2f | %6.2f %6.2f %6.2f | %8.3f | %8.3f\n",
               i + 1,
               result.original.x, result.original.y, result.original.z,
               result.transformed.x, result.transformed.y, result.transformed.z,
               result.magnitude_original, result.magnitude_transformed);
    }
    
    return 0;
}