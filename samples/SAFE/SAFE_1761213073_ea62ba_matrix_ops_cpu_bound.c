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
    int rows, cols;
    if (scanf("%d %d", &rows, &cols) != 2) return 0;
    if (!validate_matrix_dim(rows, cols)) return 0;
    
    m->rows = rows;
    m->cols = cols;
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scanf("%d", &m->data[i][j]) != 1) return 0;
        }
    }
    return 1;
}

void print_matrix(const Matrix *m) {
    printf("%d %d\n", m->rows, m->cols);
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%d ", m->data[i][j]);
        }
        printf("\n");
    }
}

int multiply_matrices(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->cols != b->rows) return 0;
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
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
                sum += a->data[i][k] * b->data[k][j];
                if ((a->data[i][k] > 0 && b->data[k][j] > 0 && sum < 0) ||
                    (a->data[i][k] < 0 && b->data[k][j] < 0 && sum > 0)) return 0;
            }
            result->data[i][j] = sum;
        }
    }
    return 1;
}

int transpose_matrix(const Matrix *m, Matrix *result) {
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
    
    printf("Enter first matrix (rows cols then elements):\n");
    if (!read_matrix(&a)) {
        printf("Invalid first matrix input\n");
        return 1;
    }
    
    printf("Enter second matrix (rows cols then elements):\n");
    if (!read_matrix(&b)) {
        printf("Invalid second matrix input\n");
        return 1;
    }
    
    printf("\nMatrix A:\n");
    print_matrix(&a);
    
    printf("\nMatrix B:\n");
    print_matrix(&b);
    
    printf("\nMatrix multiplication result:\n");
    if (multiply_matrices(&a, &b, &result)) {
        print_matrix(&result);
    } else {
        printf("Cannot multiply matrices: dimension mismatch or integer overflow\n");
    }
    
    printf("\nTranspose of matrix A:\n");
    if (transpose_matrix(&a, &result)) {
        print_matrix(&result);
    }
    
    printf("\nTranspose of matrix B:\n");
    if (transpose_matrix(&b, &result)) {
        print_matrix(&result);
    }
    
    return 0;
}