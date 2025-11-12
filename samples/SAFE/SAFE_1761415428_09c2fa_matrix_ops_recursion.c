//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 8

typedef struct {
    int rows;
    int cols;
    int data[MAX_SIZE][MAX_SIZE];
} Matrix;

int validate_matrix_size(int rows, int cols) {
    return (rows > 0 && rows <= MAX_SIZE && cols > 0 && cols <= MAX_SIZE);
}

void read_matrix(Matrix *m) {
    printf("Enter matrix dimensions (rows cols): ");
    if (scanf("%d %d", &m->rows, &m->cols) != 2) {
        printf("Invalid input\n");
        exit(1);
    }
    
    if (!validate_matrix_size(m->rows, m->cols)) {
        printf("Invalid matrix size\n");
        exit(1);
    }
    
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            if (scanf("%d", &m->data[i][j]) != 1) {
                printf("Invalid input\n");
                exit(1);
            }
        }
    }
}

void print_matrix_recursive(const Matrix *m, int row, int col) {
    if (row >= m->rows) return;
    
    printf("%6d", m->data[row][col]);
    
    if (col + 1 < m->cols) {
        print_matrix_recursive(m, row, col + 1);
    } else {
        printf("\n");
        print_matrix_recursive(m, row + 1, 0);
    }
}

void print_matrix(const Matrix *m) {
    print_matrix_recursive(m, 0, 0);
}

int matrix_trace_recursive(const Matrix *m, int n, int sum) {
    if (n >= m->rows || n >= m->cols) return sum;
    return matrix_trace_recursive(m, n + 1, sum + m->data[n][n]);
}

int matrix_trace(const Matrix *m) {
    if (m->rows != m->cols) return 0;
    return matrix_trace_recursive(m, 0, 0);
}

void transpose_recursive(const Matrix *src, Matrix *dst, int row, int col) {
    if (row >= src->rows) return;
    
    dst->data[col][row] = src->data[row][col];
    
    if (col + 1 < src->cols) {
        transpose_recursive(src, dst, row, col + 1);
    } else {
        transpose_recursive(src, dst, row + 1, 0);
    }
}

void transpose_matrix(const Matrix *src, Matrix *dst) {
    dst->rows = src->cols;
    dst->cols = src->rows;
    transpose_recursive(src, dst, 0, 0);
}

int main() {
    Matrix mat, transposed;
    
    printf("Matrix Operations Program\n");
    printf("Maximum matrix size: %dx%d\n", MAX_SIZE, MAX_SIZE);
    
    read_matrix(&mat);
    
    printf("\nOriginal matrix:\n");
    print_matrix(&mat);
    
    int trace = matrix_trace(&mat);
    printf("\nMatrix trace: %d\n", trace);
    
    transpose_matrix(&mat, &transposed);
    printf("\nTransposed matrix:\n");
    print_matrix(&transposed);
    
    return 0;
}