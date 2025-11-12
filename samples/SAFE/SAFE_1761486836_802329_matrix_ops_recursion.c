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
    printf("Enter matrix dimensions (rows cols, max %dx%d): ", MAX_SIZE, MAX_SIZE);
    if (scanf("%d %d", &m->rows, &m->cols) != 2) {
        fprintf(stderr, "Invalid input\n");
        exit(1);
    }
    
    if (!validate_matrix_size(m->rows, m->cols)) {
        fprintf(stderr, "Invalid matrix size\n");
        exit(1);
    }
    
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            if (scanf("%d", &m->data[i][j]) != 1) {
                fprintf(stderr, "Invalid input\n");
                exit(1);
            }
        }
    }
}

void print_matrix(const Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%d ", m->data[i][j]);
        }
        printf("\n");
    }
}

int recursive_trace(const Matrix *m, int row, int col, int sum) {
    if (row >= m->rows || col >= m->cols) {
        return sum;
    }
    
    if (sum > INT_MAX - m->data[row][col]) {
        fprintf(stderr, "Integer overflow detected\n");
        exit(1);
    }
    
    int new_sum = sum + m->data[row][col];
    
    if (row == m->rows - 1 && col == m->cols - 1) {
        return new_sum;
    }
    
    if (row < m->rows - 1) {
        return recursive_trace(m, row + 1, col, new_sum);
    } else {
        return recursive_trace(m, row, col + 1, new_sum);
    }
}

int calculate_trace(const Matrix *m) {
    if (m->rows != m->cols) {
        fprintf(stderr, "Trace requires square matrix\n");
        exit(1);
    }
    return recursive_trace(m, 0, 0, 0);
}

void recursive_transpose(const Matrix *src, Matrix *dst, int row, int col) {
    if (row >= src->rows) {
        return;
    }
    
    if (col >= src->cols) {
        recursive_transpose(src, dst, row + 1, 0);
        return;
    }
    
    dst->data[col][row] = src->data[row][col];
    recursive_transpose(src, dst, row, col + 1);
}

void transpose_matrix(const Matrix *src, Matrix *dst) {
    dst->rows = src->cols;
    dst->cols = src->rows;
    recursive_transpose(src, dst, 0, 0);
}

int main() {
    Matrix A, B;
    
    printf("Matrix Operations (Recursive Implementation)\n");
    printf("===========================================\n");
    
    printf("Matrix A:\n");
    read_matrix(&A);
    
    printf("\nMatrix A:\n");
    print_matrix(&A);
    
    if (A.rows == A.cols) {
        int trace = calculate_trace(&A);
        printf("\nTrace of matrix A: %d\n", trace);
    }
    
    printf("\nTranspose of matrix A:\n");
    transpose_matrix(&A, &B);
    print_matrix(&B);
    
    return 0;
}