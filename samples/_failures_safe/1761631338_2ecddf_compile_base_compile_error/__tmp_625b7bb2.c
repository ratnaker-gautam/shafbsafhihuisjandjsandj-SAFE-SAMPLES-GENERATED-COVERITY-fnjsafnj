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

struct Polynomial {
    double coefficients[10];
    uint8_t degree;
};

struct NumericalResult {
    double value;
    uint8_t precision;
    uint8_t iterations;
    int8_t error_code;
};

struct Vector3D create_vector(double x, double y, double z) {
    struct Vector3D vec;
    vec.x = x;
    vec.y = y;
    vec.z = z;
    return vec;
}

struct Matrix3x3 create_identity_matrix(void) {
    struct Matrix3x3 mat;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            mat.elements[i][j] = (i == j) ? 1.0 : 0.0;
        }
    }
    return mat;
}

struct Vector3D matrix_vector_multiply(struct Matrix3x3 mat, struct Vector3D vec) {
    struct Vector3D result;
    result.x = mat.elements[0][0] * vec.x + mat.elements[0][1] * vec.y + mat.elements[0][2] * vec.z;
    result.y = mat.elements[1][0] * vec.x + mat.elements[1][1] * vec.y + mat.elements[1][2] * vec.z;
    result.z = mat.elements[2][0] * vec.x + mat.elements[2][1] * vec.y + mat.elements[2][2] * vec.z;
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

double polynomial_evaluate(struct Polynomial poly, double x) {
    double result = 0.0;
    double power = 1.0;
    for (int i = 0; i <= poly.degree; i++) {
        result += poly.coefficients[i] * power;
        power *= x;
    }
    return result;
}

struct NumericalResult polynomial_root_bisection(struct Polynomial poly, double a, double b, uint8_t max_iterations) {
    struct NumericalResult result;
    result.error_code = 0;
    result.iterations = 0;
    
    if (max_iterations == 0 || max_iterations > 100) {
        result.error_code = -1;
        return result;
    }
    
    double fa = polynomial_evaluate(poly, a);
    double fb = polynomial_evaluate(poly, b);
    
    if (fa * fb > 0) {
        result.error_code = -2;
        return result;
    }
    
    for (uint8_t i = 0; i < max_iterations; i++) {
        double mid = (a + b) / 2.0;
        double fmid = polynomial_evaluate(poly, mid);
        
        if (fabs(fmid) < 1e-10) {
            result.value = mid;
            result.iterations = i + 1;
            result.precision = max_iterations;
            return result;
        }
        
        if (fa * fmid < 0) {
            b = mid;
            fb = fmid;
        } else {
            a = mid;
            fa = fmid;
        }
        
        result.iterations = i + 1;
    }
    
    result.value = (a + b) / 2.0;
    result.precision = max_iterations;
    return result;
}

int main(void) {
    struct Vector3D v1 = create_vector(1.0, 2.0, 3.0);
    struct Vector3D v2 = create_vector(4.0, 5.0, 6.0);
    
    printf("Vector 1: (%.2f, %.2f, %.2f)\n", v1.x, v1.y, v1.z);
    printf("Vector 2: (%.2f, %.2f, %.2f)\n", v2.x, v2.y, v2.z);
    
    double dot = vector_dot_product(v1, v2);
    printf("Dot product: %.2f\n", dot);
    
    struct Vector3D cross = vector_cross_product(v1, v2);
    printf("Cross product: (%.2f, %.2f, %.2f)\n", cross.x, cross.y, cross.z);
    
    struct Matrix3x3 identity = create_identity_matrix();
    struct Vector3D transformed = matrix_vector_multiply(identity, v1);
    printf("Identity matrix applied to v1: (%.2f, %.2f, %.2f)\n", transformed.x, transformed.y, transformed.z);
    
    struct Polynomial poly;
    poly.degree = 2;
    poly.coefficients[0] = 1.0;
    poly.coefficients[1] = -3.0