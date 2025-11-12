//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 8

typedef struct {
    int rows;
    int cols;
    int data[MAX_DIM][MAX_DIM];
} Matrix;

int validate_matrix_dimensions(int rows, int cols) {
    return (rows > 0 && rows <= MAX_DIM && cols > 0 && cols <= MAX_DIM);
}

void read_matrix(Matrix *m) {
    printf("Enter matrix dimensions (rows cols, max %dx%d): ", MAX_DIM, MAX_DIM);
    if (scanf("%d %d", &m->rows, &m->cols) != 2) {
        printf("Invalid input\n");
        exit(1);
    }
    
    if (!validate_matrix_dimensions(m->rows, m->cols)) {
        printf("Invalid dimensions\n");
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

int matrix_trace_recursive(const Matrix *m, int diag, int sum) {
    if (diag >= m->rows || diag >= m->cols) return sum;
    return matrix_trace_recursive(m, diag + 1, sum + m->data[diag][diag]);
}

void transpose_recursive(Matrix *result, const Matrix *src, int row, int col) {
    if (row >= src->rows) return;
    
    if (col < src->cols) {
        result->data[col][row] = src->data[row][col];
        transpose_recursive(result, src, row, col + 1);
    } else {
        transpose_recursive(result, src, row + 1, 0);
    }
}

Matrix transpose_matrix(const Matrix *m) {
    Matrix result;
    result.rows = m->cols;
    result.cols = m->rows;
    transpose_recursive(&result, m, 0, 0);
    return result;
}

int is_symmetric_recursive(const Matrix *m, int row, int col) {
    if (row >= m->rows) return 1;
    if (col >= m->cols) return is_symmetric_recursive(m, row + 1, 0);
    
    if (m->data[row][col] != m->data[col][row]) return 0;
    return is_symmetric_recursive(m, row, col + 1);
}

int is_symmetric(const Matrix *m) {
    if (m->rows != m->cols) return 0;
    return is_symmetric_recursive(m, 0, 0);
}

int main() {
    Matrix mat;
    
    printf("Matrix Operations (Recursive Implementation)\n");
    printf("===========================================\n");
    
    read_matrix(&mat);
    
    printf("\nOriginal matrix:\n");
    print_matrix_recursive(&mat, 0, 0);
    
    if (mat.rows == mat.cols) {
        int trace = matrix_trace_recursive(&mat, 0, 0);
        printf("\nMatrix trace: %d\n", trace);
    } else {
        printf("\nMatrix is not square, no trace calculated\n");
    }
    
    Matrix transposed = transpose_matrix(&mat);
    printf("\nTransposed matrix:\n");
    print_matrix_recursive(&transposed, 0, 0);
    
    if (is_symmetric(&mat)) {
        printf("\nMatrix is symmetric\n");
    } else {
        printf("\nMatrix is not symmetric\n");
    }
    
    return 0;
}