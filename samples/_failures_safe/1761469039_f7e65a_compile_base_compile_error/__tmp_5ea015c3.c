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

struct Matrix3x3 create_rotation_matrix(double angle) {
    struct Matrix3x3 mat;
    double c = cos(angle);
    double s = sin(angle);
    mat.elements[0][0] = c;
    mat.elements[0][1] = -s;
    mat.elements[0][2] = 0.0;
    mat.elements[1][0] = s;
    mat.elements[1][1] = c;
    mat.elements[1][2] = 0.0;
    mat.elements[2][0] = 0.0;
    mat.elements[2][1] = 0.0;
    mat.elements[2][2] = 1.0;
    return mat;
}

double vector_magnitude(struct Vector3D vec) {
    double sum = vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
    if (sum < 0.0) return 0.0;
    return sqrt(sum);
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

int validate_input(const char* input, double* value) {
    char* endptr;
    *value = strtod(input, &endptr);
    if (endptr == input) {
        return 0;
    }
    while (*endptr != '\0') {
        if (*endptr != ' ' && *endptr != '\t' && *endptr != '\n' && *endptr != '\r') {
            return 0;
        }
        endptr++;
    }
    return 1;
}

int main(void) {
    struct Vector3D vectors[3];
    double angles[3];
    char buffer[256];
    
    printf("Enter 3 vectors (x y z for each):\n");
    for (int i = 0; i < 3; i++) {
        printf("Vector %d: ", i + 1);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Input error\n");
            return 1;
        }
        
        double x, y, z;
        int parsed = sscanf(buffer, "%lf %lf %lf", &x, &y, &z);
        if (parsed != 3) {
            printf("Invalid vector format\n");
            return 1;
        }
        
        if (!isfinite(x) || !isfinite(y) || !isfinite(z)) {
            printf("Vector components must be finite numbers\n");
            return 1;
        }
        
        vectors[i] = create_vector(x, y, z);
    }
    
    printf("\nEnter 3 rotation angles in radians:\n");
    for (int i = 0; i < 3; i++) {
        printf("Angle %d: ", i + 1);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Input error\n");
            return 1;
        }
        
        if (!validate_input(buffer, &angles[i])) {
            printf("Invalid angle format\n");
            return 1;
        }
        
        if (!isfinite(angles[i])) {
            printf("Angle must be a finite number\n");
            return 1;
        }
    }
    
    printf("\nTransformation Results:\n");
    printf("======================\n");
    
    for (int i = 0; i < 3; i++) {
        struct Matrix3x3 rot_mat = create_rotation_matrix(angles[i]);
        struct TransformResult result = apply_transform(vectors[i], rot_mat);
        
        printf("Vector %d:\n", i + 1);
        printf("  Original: (%.3f, %.3f, %.3f) magnitude: %.3f\n",
               result.original.x, result.original.y, result.original.z,
               result.magnitude_original);
        printf("  Transformed: (%.3f, %.3f, %.3f) magnitude: %.3f