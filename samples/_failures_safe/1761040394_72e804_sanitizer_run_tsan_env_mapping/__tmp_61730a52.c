//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: matrix_ops
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
    if (rows <= 0 || cols <= 0) return 0;
    if (rows > MAX_DIM || cols > MAX_DIM) return 0;
    return 1;
}

int read_matrix(Matrix *m) {
    printf("Enter matrix dimensions (rows cols): ");
    if (scanf("%d %d", &m->rows, &m->cols) != 2) return 0;
    if (!validate_matrix_dimensions(m->rows, m->cols)) return 0;
    
    printf("Enter %d x %d matrix elements:\n", m->rows, m->cols);
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            if (scanf("%lf", &m->data[i][j]) != 1) return 0;
        }
    }
    return 1;
}

void print_matrix(const Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%8.2f ", m->data[i][j]);
        }
        printf("\n");
    }
}

int matrix_multiply(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->cols != b->rows) return 0;
    if (!validate_matrix_dimensions(a->rows, b->cols)) return 0;
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            result->data[i][j] = 0.0;
            for (int k = 0; k < a->cols; k++) {
                result->data[i][j] += a->data[i][k] * b->data[k][j];
            }
        }
    }
    return 1;
}

void matrix_transpose(const Matrix *src, Matrix *dst) {
    dst->rows = src->cols;
    dst->cols = src->rows;
    
    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            dst->data[j][i] = src->data[i][j];
        }
    }
}

double matrix_trace(const Matrix *m) {
    if (m->rows != m->cols) return 0.0;
    
    double trace = 0.0;
    for (int i = 0; i < m->rows; i++) {
        trace += m->data[i][i];
    }
    return trace;
}

int main() {
    Matrix a, b, result;
    
    printf("Matrix Operations Program\n");
    printf("Maximum dimension: %d\n\n", MAX_DIM);
    
    printf("First matrix:\n");
    if (!read_matrix(&a)) {
        printf("Error reading first matrix\n");
        return 1;
    }
    
    printf("Second matrix:\n");
    if (!read_matrix(&b)) {
        printf("Error reading second matrix\n");
        return 1;
    }
    
    printf("\nMatrix A:\n");
    print_matrix(&a);
    
    printf("\nMatrix B:\n");
    print_matrix(&b);
    
    printf("\nMatrix multiplication A * B:\n");
    if (matrix_multiply(&a, &b, &result)) {
        print_matrix(&result);
    } else {
        printf("Cannot multiply: dimension mismatch\n");
    }
    
    printf("\nTranspose of A:\n");
    matrix_transpose(&a, &result);
    print_matrix(&result);
    
    printf("\nTranspose of B:\n");
    matrix_transpose(&b, &result);
    print_matrix(&result);
    
    printf("\nTrace of A: ");
    double trace = matrix_trace(&a);
    if (a.rows == a.cols) {
        printf("%.2f\n", trace);
    } else {
        printf("Not a square matrix\n");
    }
    
    printf("Trace of B: ");
    trace = matrix_trace(&b);
    if (b.rows == b.cols) {
        printf("%.2f\n", trace);
    } else {
        printf("Not a square matrix\n");
    }
    
    return 0;
}