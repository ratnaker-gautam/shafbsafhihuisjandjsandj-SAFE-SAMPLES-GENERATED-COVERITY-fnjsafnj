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

struct Vector3D matrix_vector_multiply(struct Matrix3x3 mat, struct Vector3D vec) {
    struct Vector3D result;
    result.x = mat.elements[0][0] * vec.x + mat.elements[0][1] * vec.y + mat.elements[0][2] * vec.z;
    result.y = mat.elements[1][0] * vec.x + mat.elements[1][1] * vec.y + mat.elements[1][2] * vec.z;
    result.z = mat.elements[2][0] * vec.x + mat.elements[2][1] * vec.y + mat.elements[2][2] * vec.z;
    return result;
}

struct TransformResult apply_transform(struct Vector3D vec, double angle) {
    struct TransformResult result;
    result.original = vec;
    
    struct Matrix3x3 rotation = create_rotation_matrix(angle);
    result.transformed = matrix_vector_multiply(rotation, vec);
    
    result.magnitude_original = vector_magnitude(vec);
    result.magnitude_transformed = vector_magnitude(result.transformed);
    
    return result;
}

void print_vector(struct Vector3D vec) {
    printf("(%.3f, %.3f, %.3f)", vec.x, vec.y, vec.z);
}

void print_transform_result(struct TransformResult result) {
    printf("Original vector: ");
    print_vector(result.original);
    printf(" (magnitude: %.3f)\n", result.magnitude_original);
    
    printf("Transformed vector: ");
    print_vector(result.transformed);
    printf(" (magnitude: %.3f)\n", result.magnitude_transformed);
}

int main() {
    struct Vector3D vectors[5];
    double angles[5];
    
    printf("Vector Transformation Demo\n");
    printf("Enter 5 vectors and rotation angles:\n");
    
    for (int i = 0; i < 5; i++) {
        double x, y, z, angle;
        
        printf("\nVector %d - Enter x, y, z: ", i + 1);
        if (scanf("%lf %lf %lf", &x, &y, &z) != 3) {
            printf("Invalid input for vector coordinates\n");
            return 1;
        }
        
        printf("Rotation angle (radians): ");
        if (scanf("%lf", &angle) != 1) {
            printf("Invalid input for angle\n");
            return 1;
        }
        
        if (angle < -6.28318530718 || angle > 6.28318530718) {
            printf("Warning: Angle outside [-2π, 2π] range\n");
        }
        
        vectors[i] = create_vector(x, y, z);
        angles[i] = angle;
    }
    
    printf("\n=== Transformation Results ===\n");
    
    for (int i = 0; i < 5; i++) {
        printf("\nResult %d:\n", i + 1);
        struct TransformResult result = apply_transform(vectors[i], angles[i]);
        print_transform_result(result);
        
        double mag_diff = fabs(result.magnitude_original - result.magnitude_transformed);
        if (mag_diff > 1e-10) {
            printf("Warning: Magnitude changed by %.6f (should be preserved)\n", mag_diff);
        }
    }
    
    return 0;
}