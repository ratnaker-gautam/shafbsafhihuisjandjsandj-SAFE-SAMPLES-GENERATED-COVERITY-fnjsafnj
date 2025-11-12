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
    
    if (row == col) {
        int new_sum;
        if (sum > 0 && m->data[row][col] > INT_MAX - sum) {
            printf("Integer overflow detected\n");
            exit(1);
        }
        new_sum = sum + m->data[row][col];
        return recursive_trace(m, row + 1, col + 1, new_sum);
    }
    
    return recursive_trace(m, row + 1, col + 1, sum);
}

int matrix_trace(const Matrix *m) {
    if (m->rows != m->cols) {
        printf("Matrix must be square for trace calculation\n");
        return 0;
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

void matrix_transpose(const Matrix *src, Matrix *dst) {
    dst->rows = src->cols;
    dst->cols = src->rows;
    recursive_transpose(src, dst, 0, 0);
}

int main() {
    Matrix mat, transposed;
    
    printf("Matrix Operations using Recursion\n");
    printf("=================================\n");
    
    read_matrix(&mat);
    
    printf("\nOriginal matrix:\n");
    print_matrix(&mat);
    
    int trace = matrix_trace(&mat);
    if (mat.rows == mat.cols) {
        printf("\nMatrix trace: %d\n", trace);
    }
    
    matrix_transpose(&mat, &transposed);
    printf("\nTransposed matrix:\n");
    print_matrix(&transposed);
    
    return 0;
}