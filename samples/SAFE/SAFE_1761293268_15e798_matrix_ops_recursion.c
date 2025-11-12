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
    printf("Enter matrix dimensions (rows cols, max %d): ", MAX_SIZE);
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

int safe_add(int a, int b) {
    if ((b > 0 && a > INT_MAX - b) || (b < 0 && a < INT_MIN - b)) {
        printf("Integer overflow detected\n");
        exit(1);
    }
    return a + b;
}

int matrix_sum_recursive(const Matrix *m, int row, int col) {
    if (row >= m->rows) {
        return 0;
    }
    
    if (col >= m->cols) {
        return matrix_sum_recursive(m, row + 1, 0);
    }
    
    int current = m->data[row][col];
    int rest = matrix_sum_recursive(m, row, col + 1);
    
    return safe_add(current, rest);
}

int matrix_trace_recursive(const Matrix *m, int pos) {
    if (pos >= m->rows || pos >= m->cols) {
        return 0;
    }
    
    int current = m->data[pos][pos];
    int rest = matrix_trace_recursive(m, pos + 1);
    
    return safe_add(current, rest);
}

void transpose_recursive(Matrix *src, Matrix *dst, int row, int col) {
    if (row >= src->rows) {
        return;
    }
    
    if (col >= src->cols) {
        transpose_recursive(src, dst, row + 1, 0);
        return;
    }
    
    dst->data[col][row] = src->data[row][col];
    transpose_recursive(src, dst, row, col + 1);
}

int main() {
    Matrix mat, transposed;
    
    printf("Matrix Operations using Recursion\n");
    printf("================================\n");
    
    read_matrix(&mat);
    
    printf("\nOriginal matrix:\n");
    print_matrix(&mat);
    
    int sum = matrix_sum_recursive(&mat, 0, 0);
    printf("\nSum of all elements: %d\n", sum);
    
    if (mat.rows == mat.cols) {
        int trace = matrix_trace_recursive(&mat, 0);
        printf("Matrix trace: %d\n", trace);
        
        transposed.rows = mat.cols;
        transposed.cols = mat.rows;
        transpose_recursive(&mat, &transposed, 0, 0);
        
        printf("\nTransposed matrix:\n");
        print_matrix(&transposed);
    } else {
        printf("Matrix is not square, cannot compute trace or transpose\n");
    }
    
    return 0;
}