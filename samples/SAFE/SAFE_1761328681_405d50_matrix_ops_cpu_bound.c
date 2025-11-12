//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 100

typedef struct {
    int rows;
    int cols;
    double data[MAX_DIM][MAX_DIM];
} Matrix;

int validate_matrix_dimensions(int rows, int cols) {
    if (rows <= 0 || cols <= 0 || rows > MAX_DIM || cols > MAX_DIM) {
        return 0;
    }
    return 1;
}

void matrix_multiply(const Matrix *a, const Matrix *b, Matrix *result) {
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            result->data[i][j] = 0.0;
            for (int k = 0; k < a->cols; k++) {
                result->data[i][j] += a->data[i][k] * b->data[k][j];
            }
        }
    }
}

void matrix_transpose(const Matrix *src, Matrix *dst) {
    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            dst->data[j][i] = src->data[i][j];
        }
    }
}

double matrix_trace(const Matrix *m) {
    double trace = 0.0;
    int min_dim = (m->rows < m->cols) ? m->rows : m->cols;
    for (int i = 0; i < min_dim; i++) {
        trace += m->data[i][i];
    }
    return trace;
}

void print_matrix(const Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%8.2f", m->data[i][j]);
        }
        printf("\n");
    }
}

int read_matrix(Matrix *m) {
    printf("Enter number of rows and columns: ");
    if (scanf("%d %d", &m->rows, &m->cols) != 2) {
        return 0;
    }
    if (!validate_matrix_dimensions(m->rows, m->cols)) {
        return 0;
    }
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            if (scanf("%lf", &m->data[i][j]) != 1) {
                return 0;
            }
        }
    }
    return 1;
}

int main(void) {
    Matrix a, b, result;
    
    printf("Matrix Operations Program\n");
    printf("Matrix A:\n");
    if (!read_matrix(&a)) {
        printf("Invalid input for matrix A\n");
        return 1;
    }
    
    printf("Matrix B:\n");
    if (!read_matrix(&b)) {
        printf("Invalid input for matrix B\n");
        return 1;
    }
    
    if (a.cols != b.rows) {
        printf("Matrix dimensions incompatible for multiplication\n");
        return 1;
    }
    
    result.rows = a.rows;
    result.cols = b.cols;
    
    printf("\nMatrix A:\n");
    print_matrix(&a);
    
    printf("\nMatrix B:\n");
    print_matrix(&b);
    
    printf("\nMatrix Multiplication (A * B):\n");
    matrix_multiply(&a, &b, &result);
    print_matrix(&result);
    
    if (a.rows == a.cols) {
        printf("\nTrace of A: %.2f\n", matrix_trace(&a));
    }
    
    if (b.rows == b.cols) {
        printf("Trace of B: %.2f\n", matrix_trace(&b));
    }
    
    if (result.rows == result.cols) {
        printf("Trace of Result: %.2f\n", matrix_trace(&result));
    }
    
    Matrix transposed;
    transposed.rows = a.cols;
    transposed.cols = a.rows;
    printf("\nTranspose of A:\n");
    matrix_transpose(&a, &transposed);
    print_matrix(&transposed);
    
    transposed.rows = b.cols;
    transposed.cols = b.rows;
    printf("\nTranspose of B:\n");
    matrix_transpose(&b, &transposed);
    print_matrix(&transposed);
    
    return 0;
}