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

double dot_product(struct Vector3D a, struct Vector3D b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

struct Vector3D matrix_vector_multiply(struct Matrix3x3 mat, struct Vector3D vec) {
    struct Vector3D result;
    result.x = mat.elements[0][0] * vec.x + mat.elements[0][1] * vec.y + mat.elements[0][2] * vec.z;
    result.y = mat.elements[1][0] * vec.x + mat.elements[1][1] * vec.y + mat.elements[1][2] * vec.z;
    result.z = mat.elements[2][0] * vec.x + mat.elements[2][1] * vec.y + mat.elements[2][2] * vec.z;
    return result;
}

struct TransformResult perform_transformation(struct Vector3D vec, struct Matrix3x3 mat) {
    struct TransformResult res;
    res.original = vec;
    res.transformed = matrix_vector_multiply(mat, vec);
    res.magnitude = vector_magnitude(res.transformed);
    res.dot_product = dot_product(vec, res.transformed);
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
    if (endptr == buffer || *endptr != '\n') {
        return 0;
    }
    return 1;
}

int main(void) {
    struct Vector3D input_vector;
    double rotation_angle;
    
    printf("3D Vector Transformation Calculator\n");
    printf("===================================\n");
    
    if (!read_double("Enter vector X coordinate: ", &input_vector.x)) {
        printf("Error: Invalid X coordinate input.\n");
        return 1;
    }
    
    if (!read_double("Enter vector Y coordinate: ", &input_vector.y)) {
        printf("Error: Invalid Y coordinate input.\n");
        return 1;
    }
    
    if (!read_double("Enter vector Z coordinate: ", &input_vector.z)) {
        printf("Error: Invalid Z coordinate input.\n");
        return 1;
    }
    
    if (!read_double("Enter rotation angle (radians): ", &rotation_angle)) {
        printf("Error: Invalid rotation angle input.\n");
        return 1;
    }
    
    if (rotation_angle < -6.28318530718 || rotation_angle > 6.28318530718) {
        printf("Error: Rotation angle must be between -2π and 2π radians.\n");
        return 1;
    }
    
    struct Matrix3x3 rotation_mat = create_rotation_matrix(rotation_angle);
    struct TransformResult result = perform_transformation(input_vector, rotation_mat);
    
    printf("\nTransformation Results:\n");
    printf("Original vector: (%.6f, %.6f, %.6f)\n", 
           result.original.x, result.original.y, result.original.z);
    printf("Transformed vector: (%.6f, %.6f, %.6f)\n", 
           result.transformed.x, result.transformed.y, result.transformed.z);
    printf("Magnitude of transformed vector: %.6f\n", result.magnitude);
    printf("Dot product (original • transformed): %.6f\n", result.dot_product);
    
    return 0;
}