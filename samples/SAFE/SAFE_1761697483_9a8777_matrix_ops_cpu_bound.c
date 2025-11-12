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
    int data[MAX_DIM][MAX_DIM];
} Matrix;

int validate_matrix_dim(int rows, int cols) {
    if (rows <= 0 || cols <= 0) return 0;
    if (rows > MAX_DIM || cols > MAX_DIM) return 0;
    return 1;
}

int read_matrix(Matrix *m) {
    printf("Enter matrix dimensions (rows cols): ");
    if (scanf("%d %d", &m->rows, &m->cols) != 2) return 0;
    if (!validate_matrix_dim(m->rows, m->cols)) return 0;
    
    printf("Enter %d elements row by row:\n", m->rows * m->cols);
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            if (scanf("%d", &m->data[i][j]) != 1) return 0;
        }
    }
    return 1;
}

void print_matrix(const Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%6d", m->data[i][j]);
        }
        printf("\n");
    }
}

int matrix_multiply(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->cols != b->rows) return 0;
    if (!validate_matrix_dim(a->rows, b->cols)) return 0;
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            int64_t sum = 0;
            for (int k = 0; k < a->cols; k++) {
                sum += (int64_t)a->data[i][k] * (int64_t)b->data[k][j];
                if (sum > INT_MAX || sum < INT_MIN) return 0;
            }
            result->data[i][j] = (int)sum;
        }
    }
    return 1;
}

int matrix_transpose(const Matrix *m, Matrix *result) {
    if (!validate_matrix_dim(m->cols, m->rows)) return 0;
    
    result->rows = m->cols;
    result->cols = m->rows;
    
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            result->data[j][i] = m->data[i][j];
        }
    }
    return 1;
}

int main() {
    Matrix a, b, result;
    
    printf("Matrix Operations Program\n");
    printf("Matrix A:\n");
    if (!read_matrix(&a)) {
        printf("Error: Invalid matrix A input\n");
        return 1;
    }
    
    printf("Matrix B:\n");
    if (!read_matrix(&b)) {
        printf("Error: Invalid matrix B input\n");
        return 1;
    }
    
    printf("\nMatrix A:\n");
    print_matrix(&a);
    printf("\nMatrix B:\n");
    print_matrix(&b);
    
    printf("\nMatrix Multiplication (A * B):\n");
    if (matrix_multiply(&a, &b, &result)) {
        print_matrix(&result);
    } else {
        printf("Error: Cannot multiply matrices\n");
    }
    
    printf("\nMatrix Transpose (A^T):\n");
    if (matrix_transpose(&a, &result)) {
        print_matrix(&result);
    } else {
        printf("Error: Cannot transpose matrix\n");
    }
    
    printf("\nMatrix Transpose (B^T):\n");
    if (matrix_transpose(&b, &result)) {
        print_matrix(&result);
    } else {
        printf("Error: Cannot transpose matrix\n");
    }
    
    return 0;
}