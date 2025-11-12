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

int validate_matrix_dimensions(int rows, int cols) {
    if (rows <= 0 || cols <= 0) return 0;
    if (rows > MAX_DIM || cols > MAX_DIM) return 0;
    return 1;
}

int read_matrix(Matrix *m) {
    printf("Enter number of rows and columns: ");
    if (scanf("%d %d", &m->rows, &m->cols) != 2) return 0;
    if (!validate_matrix_dimensions(m->rows, m->cols)) return 0;
    
    printf("Enter matrix elements row by row:\n");
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
    if (!validate_matrix_dimensions(a->rows, b->cols)) return 0;
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                if (a->data[i][k] > 0 && b->data[k][j] > 0) {
                    if (a->data[i][k] > INT_MAX / b->data[k][j]) return 0;
                } else if (a->data[i][k] < 0 && b->data[k][j] < 0) {
                    if (a->data[i][k] < INT_MIN / b->data[k][j]) return 0;
                } else if (a->data[i][k] != 0 && b->data[k][j] != 0) {
                    if ((a->data[i][k] > 0 && b->data[k][j] < 0 && a->data[i][k] > INT_MAX / b->data[k][j]) ||
                        (a->data[i][k] < 0 && b->data[k][j] > 0 && a->data[i][k] < INT_MIN / b->data[k][j])) return 0;
                }
                int product = a->data[i][k] * b->data[k][j];
                if ((a->data[i][k] != 0 && product / a->data[i][k] != b->data[k][j])) return 0;
                if ((product > 0 && sum > INT_MAX - product) || (product < 0 && sum < INT_MIN - product)) return 0;
                sum += product;
            }
            result->data[i][j] = sum;
        }
    }
    return 1;
}

int matrix_transpose(const Matrix *src, Matrix *result) {
    if (!validate_matrix_dimensions(src->cols, src->rows)) return 0;
    
    result->rows = src->cols;
    result->cols = src->rows;
    
    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            result->data[j][i] = src->data[i][j];
        }
    }
    return 1;
}

int main() {
    Matrix a, b, result;
    
    printf("Matrix Operations Program\n");
    printf("Matrix A:\n");
    if (!read_matrix(&a)) {
        printf("Error reading matrix A\n");
        return 1;
    }
    
    printf("Matrix B:\n");
    if (!read_matrix(&b)) {
        printf("Error reading matrix B\n");
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
        printf("Multiplication failed: dimension mismatch or integer overflow\n");
    }
    
    printf("\nTranspose of Matrix A:\n");
    if (matrix_transpose(&a, &result)) {
        print_matrix(&result);
    } else {
        printf("Transpose failed\n");
    }
    
    printf("\nTranspose of Matrix B:\n");
    if (matrix_transpose(&b, &result)) {
        print_matrix(&result);
    } else {
        printf("Transpose failed\n");
    }
    
    return 0;
}