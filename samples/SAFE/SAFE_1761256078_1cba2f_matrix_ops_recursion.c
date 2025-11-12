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
    
    if (sum > 0 && m->data[n][n] > INT_MAX - sum) return INT_MAX;
    if (sum < 0 && m->data[n][n] < INT_MIN - sum) return INT_MIN;
    
    return matrix_trace_recursive(m, n + 1, sum + m->data[n][n]);
}

int matrix_trace(const Matrix *m) {
    if (m->rows != m->cols) {
        printf("Matrix must be square for trace\n");
        return 0;
    }
    return matrix_trace_recursive(m, 0, 0);
}

void transpose_recursive(Matrix *result, const Matrix *m, int row, int col) {
    if (row >= m->rows) return;
    
    if (col < m->cols) {
        result->data[col][row] = m->data[row][col];
        transpose_recursive(result, m, row, col + 1);
    } else {
        transpose_recursive(result, m, row + 1, 0);
    }
}

Matrix matrix_transpose(const Matrix *m) {
    Matrix result;
    result.rows = m->cols;
    result.cols = m->rows;
    
    transpose_recursive(&result, m, 0, 0);
    return result;
}

int main() {
    Matrix mat;
    
    printf("Matrix Operations Program\n");
    read_matrix(&mat);
    
    printf("\nOriginal matrix:\n");
    print_matrix(&mat);
    
    if (mat.rows == mat.cols) {
        int trace = matrix_trace(&mat);
        printf("\nMatrix trace: %d\n", trace);
    }
    
    Matrix transposed = matrix_transpose(&mat);
    printf("\nTransposed matrix:\n");
    print_matrix(&transposed);
    
    return 0;
}