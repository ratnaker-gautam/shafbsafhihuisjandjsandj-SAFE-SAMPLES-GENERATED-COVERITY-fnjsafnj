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
    if (row >= m->rows) {
        return;
    }
    
    printf("%6d", m->data[row][col]);
    
    if (col + 1 < m->cols) {
        print_matrix_recursive(m, row, col + 1);
    } else {
        printf("\n");
        print_matrix_recursive(m, row + 1, 0);
    }
}

void transpose_recursive(const Matrix *src, Matrix *dst, int row, int col) {
    if (row >= src->rows) {
        return;
    }
    
    dst->data[col][row] = src->data[row][col];
    
    if (col + 1 < src->cols) {
        transpose_recursive(src, dst, row, col + 1);
    } else {
        transpose_recursive(src, dst, row + 1, 0);
    }
}

Matrix transpose_matrix(const Matrix *m) {
    Matrix result;
    result.rows = m->cols;
    result.cols = m->rows;
    
    transpose_recursive(m, &result, 0, 0);
    return result;
}

int multiply_cell_recursive(const Matrix *a, const Matrix *b, int row, int col, int k, int sum) {
    if (k >= a->cols) {
        return sum;
    }
    
    long long product = (long long)a->data[row][k] * b->data[k][col];
    if (product > INT_MAX || product < INT_MIN) {
        printf("Integer overflow detected\n");
        exit(1);
    }
    
    long long new_sum = (long long)sum + product;
    if (new_sum > INT_MAX || new_sum < INT_MIN) {
        printf("Integer overflow detected\n");
        exit(1);
    }
    
    return multiply_cell_recursive(a, b, row, col, k + 1, (int)new_sum);
}

void multiply_matrices_recursive(const Matrix *a, const Matrix *b, Matrix *result, int row, int col) {
    if (row >= result->rows) {
        return;
    }
    
    result->data[row][col] = multiply_cell_recursive(a, b, row, col, 0, 0);
    
    if (col + 1 < result->cols) {
        multiply_matrices_recursive(a, b, result, row, col + 1);
    } else {
        multiply_matrices_recursive(a, b, result, row + 1, 0);
    }
}

Matrix multiply_matrices(const Matrix *a, const Matrix *b) {
    if (a->cols != b->rows) {
        printf("Matrix dimensions incompatible for multiplication\n");
        exit(1);
    }
    
    Matrix result;
    result.rows = a->rows;
    result.cols = b->cols;
    
    multiply_matrices_recursive(a, b, &result, 0, 0);
    return result;
}

int main() {
    Matrix a, b;
    
    printf("Matrix A:\n");
    read_matrix(&a);
    
    printf("Matrix B:\n");
    read_matrix(&b);
    
    printf("\nMatrix A:\n");
    print_matrix_recursive(&a, 0, 0);
    
    printf("\nMatrix B:\n");
    print_matrix_recursive(&b, 0, 0);
    
    printf("\nTranspose of A:\n");
    Matrix transposed = transpose_matrix(&a);
    print_matrix_recursive(&transposed, 0, 0);
    
    printf("\nTranspose of B:\n");
    Matrix transposed_b = transpose_matrix(&b);
    print_matrix_recursive(&transposed_b, 0, 0);
    
    printf("\nA * B:\n");
    Matrix product = multiply_matrices(&a, &b);
    print_matrix_recursive(&product, 0, 0);
    
    return 0;
}