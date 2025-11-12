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
    double cos_a = cos(angle);
    double sin_a = sin(angle);
    m.elements[1][1] = cos_a;
    m.elements[1][2] = -sin_a;
    m.elements[2][1] = sin_a;
    m.elements[2][2] = cos_a;
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

int validate_double_input(const char* input, double* value) {
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
        
        vectors[i] = vector3d_create(x, y, z);
    }
    
    printf("\nEnter 3 rotation angles in radians (one per line):\n");
    for (int i = 0; i < 3; i++) {
        printf("Angle %d: ", i + 1);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Input error\n");
            return 1;
        }
        
        if (!validate_double_input(buffer, &angles[i])) {
            printf("Invalid angle format\n");
            return 1;
        }
        
        if (!isfinite(angles[i])) {
            printf("Angle must be a finite number\n");
            return 1;
        }
    }
    
    printf("\nResults:\n");
    printf("Vector | Original | Transformed | Orig Mag | Trans Mag\n");
    printf("-------|----------|-------------|----------|-----------\n");
    
    for (int i = 0; i < 3; i++) {
        struct Matrix3x3 rotation = matrix3x3_create_rotation_x(angles[i]);
        struct TransformResult result = apply_transform(vectors[i], rotation);
        
        printf("%6d | %5.2f,%5.2f,%5.2f | %5.2f,%