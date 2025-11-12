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

double polynomial_evaluate(struct Polynomial poly, double x) {
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
    result.error_bound = tolerance;
    
    if (max_iterations <= 0 || tolerance <= 0.0) {
        result.value = NAN;
        return result;
    }
    
    for (int i = 0; i < max_iterations; i++) {
        double fx = polynomial_evaluate(poly, result.value);
        
        struct Polynomial derivative;
        derivative.degree = poly.degree - 1;
        for (int j = 0; j <= derivative.degree; j++) {
            derivative.coefficients[j] = poly.coefficients[j + 1] * (j + 1);
        }
        
        double fpx = polynomial_evaluate(derivative, result.value);
        
        if (fabs(fpx) < 1e-15) {
            result.value = NAN;
            return result;
        }
        
        double delta = fx / fpx;
        result.value -= delta;
        result.iterations++;
        
        if (fabs(delta) < tolerance) {
            break;
        }
    }
    
    return result;
}

int main(void) {
    struct Polynomial poly;
    poly.degree = 3;
    poly.coefficients[0] = 2.0;
    poly.coefficients[1] = -3.0;
    poly.coefficients[2] = 1.0;
    poly.coefficients[3] = 1.0;
    
    printf("Polynomial: ");
    for (int i = poly.degree; i >= 0; i--) {
        if (i == poly.degree) {
            printf("%.1fx^%d", poly.coefficients[i], i);
        } else if (i > 1) {
            printf(" + %.1fx^%d", poly.coefficients[i], i);
        } else if (i == 1) {
            printf(" + %.1fx", poly.coefficients[i]);
        } else {
            printf(" + %.1f", poly.coefficients[i]);
        }
    }
    printf("\n");
    
    struct NumericalResult root = newton_raphson(poly, 1.5, 1e-10, 100);
    
    if (!isnan(root.value)) {
        printf("Found root: %.10f\n", root.value);
        printf("Iterations: %d\n", root.iterations);
        printf("Error bound: %.2e\n", root.error_bound);
        
        double verification = polynomial_evaluate(poly, root.value);
        printf("Verification f(root) = %.2e\n", verification);
    } else {
        printf("Failed to find root\n");
    }
    
    struct Vector3D vec1 = create_vector(1.0, 2.0, 3.0);
    struct Vector3D vec2 = create_vector(4.0, 5.0, 6.0);
    struct Matrix3x3 mat = create_identity_matrix();
    
    printf("\nVector 1: (%.1f, %.1f, %.1f)\n", vec1.x, vec1.y, vec1.z);
    printf("Vector 2: (