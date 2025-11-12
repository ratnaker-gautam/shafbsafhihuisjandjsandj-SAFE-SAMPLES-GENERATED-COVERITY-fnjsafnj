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

struct Polynomial create_polynomial(double coeffs[], int deg) {
    struct Polynomial poly;
    if (deg < 0 || deg > 9) {
        poly.degree = 0;
        poly.coefficients[0] = 0.0;
        return poly;
    }
    poly.degree = deg;
    for (int i = 0; i <= deg; i++) {
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
    result.error_bound = tolerance * 2.0;
    
    if (max_iterations <= 0 || tolerance <= 0.0) {
        result.error_bound = INFINITY;
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
            result.value = x;
            result.iterations = i + 1;
            result.error_bound = INFINITY;
            return result;
        }
        
        double delta = fx / fpx;
        double x_new = x - delta;
        
        if (fabs(delta) < tolerance) {
            result.value = x_new;
            result.iterations = i + 1;
            result.error_bound = fabs(delta);
            return result;
        }
        
        x = x_new;
        result.iterations = i + 1;
    }
    
    result.value = x;
    result.error_bound = tolerance * 2.0;
    return result;
}

int main(void) {
    double poly_coeffs[] = {2.0, -3.0, 1.0};
    struct Polynomial poly = create_polynomial(poly_coeffs, 2);
    
    printf("Polynomial: %.1fx^2 + %.1fx + %.1f\n", poly.coefficients[2], poly.coefficients[1], poly.coefficients[0]);
    
    struct NumericalResult root1 = newton_raphson(poly, 5.0, 1e-10, 100);
    printf("Root near x=5: %.10f (iterations: %d, error: %.2e)\n", root1.value, root1.iterations, root1.error_bound);
    
    struct NumericalResult root2 = newton_raphson(poly, -2.0,