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
    int degree;
};

struct NumericalResult {
    double value;
    int iterations;
    double error_bound;
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

struct Polynomial create_polynomial(double coeffs[], int degree) {
    struct Polynomial poly;
    if (degree < 0 || degree > 9) {
        poly.degree = 0;
        poly.coefficients[0] = 0.0;
        return poly;
    }
    poly.degree = degree;
    for (int i = 0; i <= degree; i++) {
        poly.coefficients[i] = coeffs[i];
    }
    return poly;
}

double evaluate_polynomial(struct Polynomial poly, double x) {
    double result = 0.0;
    double power = 1.0;
    for (int i = 0; i <= poly.degree; i++) {
        result += poly.coefficients[i] * power;
        power *= x;
    }
    return result;
}

struct NumericalResult newton_raphson(struct Polynomial poly, double initial_guess, double tolerance, int max_iterations) {
    struct NumericalResult result;
    result.value = initial_guess;
    result.iterations = 0;
    result.error_bound = 0.0;
    
    if (max_iterations <= 0 || tolerance <= 0.0) {
        return result;
    }
    
    double x = initial_guess;
    for (int i = 0; i < max_iterations; i++) {
        double fx = evaluate_polynomial(poly, x);
        
        struct Polynomial derivative;
        if (poly.degree > 0) {
            double deriv_coeffs[9];
            for (int j = 0; j < poly.degree; j++) {
                deriv_coeffs[j] = poly.coefficients[j + 1] * (j + 1);
            }
            derivative = create_polynomial(deriv_coeffs, poly.degree - 1);
        } else {
            double zero_coeff[1] = {0.0};
            derivative = create_polynomial(zero_coeff, 0);
        }
        
        double fpx = evaluate_polynomial(derivative, x);
        
        if (fabs(fpx) < 1e-15) {
            break;
        }
        
        double delta = fx / fpx;
        x = x - delta;
        
        result.iterations = i + 1;
        result.value = x;
        result.error_bound = fabs(delta);
        
        if (fabs(delta) < tolerance) {
            break;
        }
    }
    
    return result;
}

int main(void) {
    printf("Numeric Computation Demo\n");
    printf("=======================\n\n");
    
    struct Vector3D vec1 = create_vector(1.0, 2.0, 3.0);
    struct Vector3D vec2 = create_vector(4.0, 5.0, 6.0);
    
    printf("Vector 1: (%.2f, %.2f, %.2f)\n", vec1.x, vec1.y, vec1.z);
    printf("Vector 2: (%.2f, %.2f, %.2f)\n", vec2.x, vec2.y, vec2.z);
    
    double dot = vector_dot_product(vec1, vec2);
    printf("Dot product: %.2f\n\n", dot);
    
    struct Matrix3x3 mat = create_identity_matrix();
    mat.elements[0][1] = 0.5;
    mat.elements[1][2] = -0.3;
    
    struct Vector3D transformed = matrix_vector_multiply(mat, vec1);
    printf("Matrix-vector multiplication result: (%.2f, %.2f, %.2f