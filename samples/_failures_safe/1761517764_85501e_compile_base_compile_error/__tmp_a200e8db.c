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

struct TransformResult apply_transform(struct Vector3D vec, struct Matrix3x3 mat) {
    struct TransformResult res;
    res.original = vec;
    res.transformed = matrix_vector_multiply(mat, vec);
    res.magnitude_original = vector_magnitude(vec);
    res.magnitude_transformed = vector_magnitude(res.transformed);
    return res;
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
    
    printf("3D Vector Transformation Analysis\n");
    printf("Enter 3 vectors (x y z coordinates):\n");
    
    for (int i = 0; i < 3; i++) {
        double x, y, z;
        printf("Vector %d:\n", i + 1);
        if (!read_double("  X: ", &x)) {
            printf("Invalid input for X coordinate\n");
            return 1;
        }
        if (!read_double("  Y: ", &y)) {
            printf("Invalid input for Y coordinate\n");
            return 1;
        }
        if (!read_double("  Z: ", &z)) {
            printf("Invalid input for Z coordinate\n");
            return 1;
        }
        vectors[i] = create_vector(x, y, z);
    }
    
    printf("\nEnter rotation angles in radians (x y z):\n");
    if (!read_double("  Angle X: ", &angles[0])) {
        printf("Invalid input for angle X\n");
        return 1;
    }
    if (!read_double("  Angle Y: ", &angles[1])) {
        printf("Invalid input for angle Y\n");
        return 1;
    }
    if (!read_double("  Angle Z: ", &angles[2])) {
        printf("Invalid input for angle Z\n");
        return 1;
    }
    
    struct Matrix3x3 rotation_matrix = create_rotation_matrix(angles[0], angles[1], angles[2]);
    
    printf("\nTransformation Results:\n");
    printf("Rotation Matrix:\n");
    for (int i = 0; i < 3; i++) {
        printf("  [");
        for (int j = 0; j < 3; j++) {
            printf("%8.4f", rotation_matrix.elements[i][j]);
            if (j < 2) printf(" ");
        }
        printf("]\n");
    }
    
    printf("\nVector Analysis:\n");
    for (int i = 0; i < 3; i++)