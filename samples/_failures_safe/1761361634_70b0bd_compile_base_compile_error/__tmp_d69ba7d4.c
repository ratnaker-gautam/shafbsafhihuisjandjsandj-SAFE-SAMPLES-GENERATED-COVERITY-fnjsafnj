//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>
#include <float.h>
#include <ctype.h>

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
    int valid;
};

struct Vector3D vector3d_create(double x, double y, double z) {
    struct Vector3D v;
    v.x = x;
    v.y = y;
    v.z = z;
    return v;
}

struct Matrix3x3 matrix3x3_create(double elements[3][3]) {
    struct Matrix3x3 m;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            m.elements[i][j] = elements[i][j];
        }
    }
    return m;
}

int is_finite(double value) {
    return value == value && value <= DBL_MAX && value >= -DBL_MAX;
}

struct Vector3D matrix_vector_multiply(struct Matrix3x3 m, struct Vector3D v) {
    struct Vector3D result = {0, 0, 0};
    for (int i = 0; i < 3; i++) {
        double sum = 0.0;
        for (int j = 0; j < 3; j++) {
            if (!is_finite(m.elements[i][j]) || !is_finite(v.x) || !is_finite(v.y) || !is_finite(v.z)) {
                result.x = NAN;
                result.y = NAN;
                result.z = NAN;
                return result;
            }
            double component = (j == 0 ? v.x : (j == 1 ? v.y : v.z));
            double product = m.elements[i][j] * component;
            if (!is_finite(product)) {
                result.x = NAN;
                result.y = NAN;
                result.z = NAN;
                return result;
            }
            sum += product;
            if (!is_finite(sum)) {
                result.x = NAN;
                result.y = NAN;
                result.z = NAN;
                return result;
            }
        }
        if (i == 0) result.x = sum;
        else if (i == 1) result.y = sum;
        else result.z = sum;
    }
    return result;
}

double vector_magnitude(struct Vector3D v) {
    if (!is_finite(v.x) || !is_finite(v.y) || !is_finite(v.z)) {
        return NAN;
    }
    double x2 = v.x * v.x;
    double y2 = v.y * v.y;
    double z2 = v.z * v.z;
    if (!is_finite(x2) || !is_finite(y2) || !is_finite(z2)) {
        return NAN;
    }
    double sum = x2 + y2 + z2;
    if (!is_finite(sum) || sum < 0) {
        return NAN;
    }
    return sqrt(sum);
}

struct TransformResult transform_vector(struct Matrix3x3 m, struct Vector3D v) {
    struct TransformResult result;
    result.original = v;
    result.transformed = matrix_vector_multiply(m, v);
    result.magnitude = vector_magnitude(result.transformed);
    result.valid = is_finite(result.transformed.x) && is_finite(result.transformed.y) && 
                   is_finite(result.transformed.z) && is_finite(result.magnitude);
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
    while (*endptr != '\0' && *endptr != '\n') {
        if (!isspace((unsigned char)*endptr)) {
            return 0;
        }
        endptr++;
    }
    return is_finite(*value);
}

int main(void) {
    double matrix_elements[3][3];
    printf("Enter 3x3 matrix elements (row-wise):\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            char prompt[32];
            if (snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j) >= (int)sizeof(prompt)) {
                printf("Prompt buffer overflow.\n");
                return 1;
            }
            if (!read_double(prompt, &matrix_elements[i][j])) {
                printf("Invalid matrix element input.\n");
                return 1;
            }
        }
    }

    struct Vector3D input_vector;
    printf("Enter vector components:\n");
    if (!read_double("X: ", &input_vector.x) ||
        !read_double("Y: ", &input_vector.y) ||
        !read_double("Z: ", &input_vector.z)) {
        printf("Invalid vector component input.\n");
        return 1;
    }

    struct Matrix