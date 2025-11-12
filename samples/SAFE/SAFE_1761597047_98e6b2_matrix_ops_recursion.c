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
    if (rows <= 0 || cols <= 0) return 0;
    if (rows > MAX_SIZE || cols > MAX_SIZE) return 0;
    return 1;
}

void read_matrix(Matrix *m) {
    printf("Enter matrix rows and columns (max %d): ", MAX_SIZE);
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
    if (row >= m->rows || col >= m->cols) return sum;
    if (row == col) {
        if (sum > INT_MAX - m->data[row][col]) {
            printf("Integer overflow detected\n");
            exit(1);
        }
        sum += m->data[row][col];
    }
    if (col + 1 < m->cols) {
        return recursive_trace(m, row, col + 1, sum);
    } else {
        return recursive_trace(m, row + 1, 0, sum);
    }
}

int matrix_trace(const Matrix *m) {
    if (m->rows != m->cols) {
        printf("Matrix must be square for trace\n");
        return 0;
    }
    return recursive_trace(m, 0, 0, 0);
}

void recursive_transpose(const Matrix *src, Matrix *dst, int row, int col) {
    if (row >= src->rows || col >= src->cols) return;
    dst->data[col][row] = src->data[row][col];
    if (col + 1 < src->cols) {
        recursive_transpose(src, dst, row, col + 1);
    } else {
        recursive_transpose(src, dst, row + 1, 0);
    }
}

void matrix_transpose(const Matrix *src, Matrix *dst) {
    dst->rows = src->cols;
    dst->cols = src->rows;
    recursive_transpose(src, dst, 0, 0);
}

int main() {
    Matrix mat, transposed;
    
    printf("Matrix Operations using Recursion\n");
    printf("================================\n");
    
    read_matrix(&mat);
    
    printf("\nOriginal matrix:\n");
    print_matrix(&mat);
    
    if (mat.rows == mat.cols) {
        int trace = matrix_trace(&mat);
        printf("\nMatrix trace: %d\n", trace);
    }
    
    matrix_transpose(&mat, &transposed);
    printf("\nTransposed matrix:\n");
    print_matrix(&transposed);
    
    return 0;
}