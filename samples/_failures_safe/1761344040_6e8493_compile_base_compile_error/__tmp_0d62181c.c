//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

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
    double determinant;
    int valid;
};

struct Vector3D create_vector(double x, double y, double z) {
    struct Vector3D vec;
    vec.x = x;
    vec.y = y;
    vec.z = z;
    return vec;
}

struct Matrix3x3 create_matrix(double a11, double a12, double a13,
                              double a21, double a22, double a23,
                              double a31, double a32, double a33) {
    struct Matrix3x3 mat;
    mat.elements[0][0] = a11;
    mat.elements[0][1] = a12;
    mat.elements[0][2] = a13;
    mat.elements[1][0] = a21;
    mat.elements[1][1] = a22;
    mat.elements[1][2] = a23;
    mat.elements[2][0] = a31;
    mat.elements[2][1] = a32;
    mat.elements[2][2] = a33;
    return mat;
}

double calculate_determinant(struct Matrix3x3 mat) {
    double det = mat.elements[0][0] * (mat.elements[1][1] * mat.elements[2][2] - mat.elements[1][2] * mat.elements[2][1])
               - mat.elements[0][1] * (mat.elements[1][0] * mat.elements[2][2] - mat.elements[1][2] * mat.elements[2][0])
               + mat.elements[0][2] * (mat.elements[1][0] * mat.elements[2][1] - mat.elements[1][1] * mat.elements[2][0]);
    return det;
}

struct Vector3D transform_vector(struct Vector3D vec, struct Matrix3x3 mat) {
    struct Vector3D result;
    result.x = mat.elements[0][0] * vec.x + mat.elements[0][1] * vec.y + mat.elements[0][2] * vec.z;
    result.y = mat.elements[1][0] * vec.x + mat.elements[1][1] * vec.y + mat.elements[1][2] * vec.z;
    result.z = mat.elements[2][0] * vec.x + mat.elements[2][1] * vec.y + mat.elements[2][2] * vec.z;
    return result;
}

struct TransformResult perform_transformation(struct Vector3D vec, struct Matrix3x3 mat) {
    struct TransformResult res;
    res.original = vec;
    res.determinant = calculate_determinant(mat);
    
    if (fabs(res.determinant) < 1e-12) {
        res.valid = 0;
        res.transformed = create_vector(0.0, 0.0, 0.0);
    } else {
        res.valid = 1;
        res.transformed = transform_vector(vec, mat);
    }
    
    return res;
}

int read_double(const char* prompt, double* value) {
    char buffer[256];
    char* endptr;
    
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    
    *value = strtod(buffer, &endptr);
    if (endptr == buffer) {
        return 0;
    }
    
    return 1;
}

int main(void) {
    struct Vector3D input_vector;
    struct Matrix3x3 transformation_matrix;
    struct TransformResult result;
    
    printf("3D Vector Transformation Calculator\n");
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
    
    printf("\nEnter transformation matrix elements:\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            if (!read_double(prompt, &transformation_matrix.elements[i][j])) {
                printf("Invalid input for matrix element\n");
                return 1;
            }
        }
    }
    
    result = perform_transformation(input_vector, transformation_matrix);
    
    printf("\nResults:\n");
    printf("Original vector: (%.6f, %.6f, %.6f)\n", 
           result.original.x, result.original.y, result.original.z);
    printf("Matrix determinant: %.6f\n", result.determinant);
    
    if (result.valid) {
        printf("Trans