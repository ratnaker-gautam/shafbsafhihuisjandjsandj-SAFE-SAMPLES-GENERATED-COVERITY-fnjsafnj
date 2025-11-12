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
    struct Vector3D input_vector;
    double angle_x, angle_y, angle_z;
    
    printf("3D Vector Rotation Transform\n");
    printf("============================\n\n");
    
    printf("Enter vector coordinates:\n");
    if (!read_double("X: ", &input_vector.x)) {
        printf("Invalid input for X coordinate\n");
        return 1;
    }
    if (!read_double("Y: ", &input_vector.y)) {
        printf("Invalid input for Y coordinate\n");
        return 1;
    }
    if (!read_double("Z: ", &input_vector.z)) {
        printf("Invalid input for Z coordinate\n");
        return 1;
    }
    
    printf("\nEnter rotation angles (in radians):\n");
    if (!read_double("Rotation around X-axis: ", &angle_x)) {
        printf("Invalid input for X rotation angle\n");
        return 1;
    }
    if (!read_double("Rotation around Y-axis: ", &angle_y)) {
        printf("Invalid input for Y rotation angle\n");
        return 1;
    }
    if (!read_double("Rotation around Z-axis: ", &angle_z)) {
        printf("Invalid input for Z rotation angle\n");
        return 1;
    }
    
    struct Matrix3x3 rotation_matrix = create_rotation_matrix(angle_x, angle_y, angle_z);
    struct TransformResult result = apply_transform(input_vector, rotation_matrix);
    
    printf("\nResults:\n");
    printf("Original vector: (%.6f, %.6f, %.6f)\n", 
           result.original.x, result.original.y, result.original.z);
    printf("Original magnitude: %.6f\n", result.magnitude_original);
    printf("Transformed vector: (%.6f, %.6f, %.6f)\n", 
           result.transformed.x, result.transformed.y, result.transformed.z);
    printf("Transformed magnitude: %.6f\n", result.magnitude_transformed);
    
    double magnitude_diff = fabs(result.magnitude_original - result.magnitude_transformed);
    printf("Magnitude difference: %.6f\n