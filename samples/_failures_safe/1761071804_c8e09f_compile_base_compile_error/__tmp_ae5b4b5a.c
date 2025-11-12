//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: numeric_computation
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

struct Polynomial create_polynomial(double coeffs[], int deg) {
    struct Polynomial poly;
    if (deg < 0 || deg > 9) {
        deg = 0;
    }
    poly.degree = deg;
    for (int i = 0; i <= deg; i++) {
        poly.coefficients[i] = coeffs[i];
    }
    for (int i = deg + 1; i < 10; i++) {
        poly.coefficients[i] = 0.0;
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
    result.error_bound = tolerance * 2.0;
    
    if (max_iterations <= 0) {
        max_iterations = 100;
    }
    if (tolerance <= 0.0) {
        tolerance = 1e-10;
    }
    
    double x = initial_guess;
    for (int i = 0; i < max_iterations; i++) {
        double fx = evaluate_polynomial(poly, x);
        if (fabs(fx) < tolerance) {
            result.value = x;
            result.iterations = i + 1;
            result.error_bound = fabs(fx);
            return result;
        }
        
        double derivative_approx = (evaluate_polynomial(poly, x + tolerance) - fx) / tolerance;
        if (fabs(derivative_approx) < 1e-15) {
            break;
        }
        
        double x_new = x - fx / derivative_approx;
        if (fabs(x_new - x) < tolerance) {
            result.value = x_new;
            result.iterations = i + 1;
            result.error_bound = fabs(x_new - x);
            return result;
        }
        x = x_new;
    }
    
    result.value = x;
    result.iterations = max_iterations;
    result.error_bound = tolerance * 2.0;
    return result;
}

int main(void) {
    double coeffs[] = {2.0, -3.0, 1.0};
    struct Polynomial poly = create_polynomial(coeffs, 2);
    
    printf("Polynomial: %.1fx^2 + %.1fx + %.1f\n", poly.coefficients[2], poly.coefficients[1], poly.coefficients[0]);
    
    struct Vector3D vec1 = create_vector(1.0, 2.0, 3.0);
    struct Vector3D vec2 = create_vector(4.0, 5.0, 6.0);
    double dot = vector_dot_product(vec1, vec2);
    printf("Vector dot product: (%.1f, %.1f, %.1f) . (%.1f, %.1f, %.1f) = %.1f\n", 
           vec1.x, vec1.y, vec1.z, vec2.x, vec2.y, vec2.z, dot);
    
    struct Matrix3x3 mat = create_identity_matrix();
    struct Vector3D transformed = matrix_vector_multiply(mat, vec1);