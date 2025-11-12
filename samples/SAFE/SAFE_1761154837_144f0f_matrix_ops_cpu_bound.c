//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MATRIX_SIZE 8

typedef struct {
    double data[MATRIX_SIZE][MATRIX_SIZE];
} Matrix;

void matrix_multiply(const Matrix *a, const Matrix *b, Matrix *result) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            result->data[i][j] = 0.0;
            for (int k = 0; k < MATRIX_SIZE; k++) {
                result->data[i][j] += a->data[i][k] * b->data[k][j];
            }
        }
    }
}

void matrix_transpose(const Matrix *input, Matrix *result) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            result->data[j][i] = input->data[i][j];
        }
    }
}

double matrix_determinant(const Matrix *m) {
    Matrix temp = *m;
    double det = 1.0;
    
    for (int i = 0; i < MATRIX_SIZE; i++) {
        int pivot = i;
        for (int j = i + 1; j < MATRIX_SIZE; j++) {
            if (fabs(temp.data[j][i]) > fabs(temp.data[pivot][i])) {
                pivot = j;
            }
        }
        
        if (pivot != i) {
            for (int k = 0; k < MATRIX_SIZE; k++) {
                double swap = temp.data[i][k];
                temp.data[i][k] = temp.data[pivot][k];
                temp.data[pivot][k] = swap;
            }
            det = -det;
        }
        
        if (fabs(temp.data[i][i]) < 1e-12) {
            return 0.0;
        }
        
        det *= temp.data[i][i];
        
        for (int j = i + 1; j < MATRIX_SIZE; j++) {
            double factor = temp.data[j][i] / temp.data[i][i];
            for (int k = i + 1; k < MATRIX_SIZE; k++) {
                temp.data[j][k] -= factor * temp.data[i][k];
            }
        }
    }
    
    return det;
}

void initialize_matrix(Matrix *m) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            m->data[i][j] = (double)((i * MATRIX_SIZE + j) % 10 + 1);
        }
    }
}

void print_matrix(const Matrix *m) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            printf("%8.2f", m->data[i][j]);
        }
        printf("\n");
    }
}

int main() {
    Matrix a, b, result_mult, result_trans;
    
    initialize_matrix(&a);
    initialize_matrix(&b);
    
    printf("Matrix A:\n");
    print_matrix(&a);
    
    printf("\nMatrix B:\n");
    print_matrix(&b);
    
    matrix_multiply(&a, &b, &result_mult);
    printf("\nMatrix A * B:\n");
    print_matrix(&result_mult);
    
    matrix_transpose(&a, &result_trans);
    printf("\nTranspose of A:\n");
    print_matrix(&result_trans);
    
    double det_a = matrix_determinant(&a);
    double det_b = matrix_determinant(&b);
    
    printf("\nDeterminant of A: %.6f\n", det_a);
    printf("Determinant of B: %.6f\n", det_b);
    printf("Determinant of A*B: %.6f\n", det_a * det_b);
    
    return 0;
}