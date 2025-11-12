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

struct Vector3D vector_add(struct Vector3D a, struct Vector3D b) {
    struct Vector3D result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    return result;
}

struct Vector3D vector_scale(struct Vector3D v, double scalar) {
    struct Vector3D result;
    result.x = v.x * scalar;
    result.y = v.y * scalar;
    result.z = v.z * scalar;
    return result;
}

double vector_dot(struct Vector3D a, struct Vector3D b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

struct Matrix3x3 matrix_multiply(struct Matrix3x3 a, struct Matrix3x3 b) {
    struct Matrix3x3 result;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result.elements[i][j] = 0.0;
            for (int k = 0; k < 3; k++) {
                result.elements[i][j] += a.elements[i][k] * b.elements[k][j];
            }
        }
    }
    return result;
}

double polynomial_evaluate(struct Polynomial p, double x) {
    double result = 0.0;
    double power = 1.0;
    for (int i = 0; i <= p.degree; i++) {
        result += p.coefficients[i] * power;
        power *= x;
    }
    return result;
}

struct NumericalResult newton_raphson(struct Polynomial p, double initial_guess, uint8_t max_iterations) {
    struct NumericalResult result;
    result.value = initial_guess;
    result.iterations = 0;
    result.error_code = 0;
    
    if (p.degree == 0) {
        result.error_code = -1;
        return result;
    }
    
    struct Polynomial derivative;
    derivative.degree = p.degree - 1;
    for (int i = 0; i <= derivative.degree; i++) {
        derivative.coefficients[i] = p.coefficients[i+1] * (i+1);
    }
    
    for (int i = 0; i < max_iterations; i++) {
        double fx = polynomial_evaluate(p, result.value);
        double dfx = polynomial_evaluate(derivative, result.value);
        
        if (fabs(dfx) < 1e-15) {
            result.error_code = -2;
            return result;
        }
        
        double delta = fx / dfx;
        result.value -= delta;
        result.iterations++;
        
        if (fabs(delta) < 1e-10) {
            break;
        }
    }
    
    return result;
}

int main() {
    struct Vector3D vec1 = {1.0, 2.0, 3.0};
    struct Vector3D vec2 = {4.0, 5.0, 6.0};
    
    struct Vector3D sum = vector_add(vec1, vec2);
    struct Vector3D scaled = vector_scale(vec1, 2.5);
    double dot_product = vector_dot(vec1, vec2);
    
    struct Matrix3x3 mat1 = {{{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}, {7.0, 8.0, 9.0}}};
    struct Matrix3x3 mat2 = {{{9.0, 8.0, 7.0}, {6.0, 5.0, 4.0}, {3.0, 2.0, 1.0}}};
    
    struct Matrix3x3 product = matrix_multiply(mat1, mat2);
    
    struct Polynomial poly;
    poly.degree = 3;
    poly.coefficients[0] = 2.0;
    poly.coefficients[1] = -3.0;
    poly.coefficients[2] = 1.0;
    poly.coefficients[3] = 4.0;
    
    double poly_value = polynomial_evaluate(poly, 2.0);
    struct NumericalResult root = newton_raphson(poly, 1.0, 50);
    
    printf("Vector sum: (%.2f, %.2f, %.2f)\n", sum.x, sum.y, sum.z);
    printf("Scaled vector: (%.2f, %.2f, %.2f)\n", scaled.x, scaled.y, scaled.z);
    printf("Dot product: %.2f\n", dot_product);
    
    printf("Matrix product:\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%.2f ", product.elements[i][j]);
        }
        printf("\n");
    }