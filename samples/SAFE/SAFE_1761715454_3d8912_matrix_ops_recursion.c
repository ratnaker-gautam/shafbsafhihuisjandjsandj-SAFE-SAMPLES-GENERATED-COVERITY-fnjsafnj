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

void print_matrix_recursive(const Matrix *m, int row, int col) {
    if (row >= m->rows) return;
    
    if (col >= m->cols) {
        printf("\n");
        print_matrix_recursive(m, row + 1, 0);
        return;
    }
    
    printf("%4d", m->data[row][col]);
    print_matrix_recursive(m, row, col + 1);
}

int matrix_trace_recursive(const Matrix *m, int diag, int sum) {
    if (diag >= m->rows || diag >= m->cols) return sum;
    
    int new_sum;
    if (sum > 0 && m->data[diag][diag] > INT_MAX - sum) {
        printf("Integer overflow detected\n");
        exit(1);
    }
    if (sum < 0 && m->data[diag][diag] < INT_MIN - sum) {
        printf("Integer overflow detected\n");
        exit(1);
    }
    new_sum = sum + m->data[diag][diag];
    
    return matrix_trace_recursive(m, diag + 1, new_sum);
}

void transpose_recursive(Matrix *result, const Matrix *src, int row, int col) {
    if (row >= src->rows) return;
    
    if (col >= src->cols) {
        transpose_recursive(result, src, row + 1, 0);
        return;
    }
    
    result->data[col][row] = src->data[row][col];
    transpose_recursive(result, src, row, col + 1);
}

Matrix transpose_matrix(const Matrix *m) {
    Matrix result;
    result.rows = m->cols;
    result.cols = m->rows;
    
    transpose_recursive(&result, m, 0, 0);
    return result;
}

int main() {
    Matrix mat, transposed;
    
    printf("Matrix Operations using Recursion\n");
    printf("=================================\n");
    
    read_matrix(&mat);
    
    printf("\nOriginal matrix:\n");
    print_matrix_recursive(&mat, 0, 0);
    
    if (mat.rows == mat.cols) {
        int trace = matrix_trace_recursive(&mat, 0, 0);
        printf("Matrix trace: %d\n", trace);
    } else {
        printf("Matrix is not square, cannot compute trace\n");
    }
    
    printf("\nTransposed matrix:\n");
    transposed = transpose_matrix(&mat);
    print_matrix_recursive(&transposed, 0, 0);
    
    return 0;
}