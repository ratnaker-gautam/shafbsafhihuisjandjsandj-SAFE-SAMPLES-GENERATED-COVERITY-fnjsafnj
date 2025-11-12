//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MATRIX_SIZE 8

typedef struct {
    int rows;
    int cols;
    int data[MATRIX_SIZE][MATRIX_SIZE];
} Matrix;

void matrix_init(Matrix *m, int rows, int cols) {
    if (rows <= 0 || rows > MATRIX_SIZE || cols <= 0 || cols > MATRIX_SIZE) {
        fprintf(stderr, "Invalid matrix dimensions\n");
        exit(1);
    }
    m->rows = rows;
    m->cols = cols;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            m->data[i][j] = 0;
        }
    }
}

void matrix_multiply(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->cols != b->rows) {
        fprintf(stderr, "Matrix dimensions incompatible for multiplication\n");
        exit(1);
    }
    if (result->rows != a->rows || result->cols != b->cols) {
        fprintf(stderr, "Result matrix has wrong dimensions\n");
        exit(1);
    }
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                if (a->data[i][k] > 0 && b->data[k][j] > 0) {
                    if (a->data[i][k] > INT_MAX / b->data[k][j]) {
                        fprintf(stderr, "Integer overflow detected\n");
                        exit(1);
                    }
                } else if (a->data[i][k] < 0 && b->data[k][j] < 0) {
                    if (a->data[i][k] < INT_MIN / b->data[k][j]) {
                        fprintf(stderr, "Integer overflow detected\n");
                        exit(1);
                    }
                }
                sum += a->data[i][k] * b->data[k][j];
            }
            result->data[i][j] = sum;
        }
    }
}

void matrix_transpose(const Matrix *src, Matrix *dst) {
    if (src->rows != dst->cols || src->cols != dst->rows) {
        fprintf(stderr, "Transpose dimensions mismatch\n");
        exit(1);
    }
    
    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            dst->data[j][i] = src->data[i][j];
        }
    }
}

void matrix_print(const Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%6d ", m->data[i][j]);
        }
        printf("\n");
    }
}

int main() {
    Matrix a, b, result, transposed;
    
    matrix_init(&a, 3, 3);
    matrix_init(&b, 3, 3);
    matrix_init(&result, 3, 3);
    matrix_init(&transposed, 3, 3);
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            a.data[i][j] = i * 3 + j + 1;
            b.data[i][j] = (i == j) ? 1 : 0;
        }
    }
    
    printf("Matrix A:\n");
    matrix_print(&a);
    
    printf("\nMatrix B (identity):\n");
    matrix_print(&b);
    
    printf("\nMultiplying A × B:\n");
    matrix_multiply(&a, &b, &result);
    matrix_print(&result);
    
    printf("\nTranspose of A:\n");
    matrix_transpose(&a, &transposed);
    matrix_print(&transposed);
    
    printf("\nMultiplying A × A^T:\n");
    matrix_init(&result, 3, 3);
    matrix_multiply(&a, &transposed, &result);
    matrix_print(&result);
    
    return 0;
}