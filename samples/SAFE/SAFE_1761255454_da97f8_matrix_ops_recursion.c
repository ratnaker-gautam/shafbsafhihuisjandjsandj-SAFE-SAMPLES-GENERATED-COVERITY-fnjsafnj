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

void recursive_matrix_multiply(Matrix* a, Matrix* b, Matrix* result, int row, int col, int k) {
    if (k >= a->cols) return;
    
    result->data[row][col] += a->data[row][k] * b->data[k][col];
    recursive_matrix_multiply(a, b, result, row, col, k + 1);
}

void recursive_fill_result(Matrix* a, Matrix* b, Matrix* result, int row, int col) {
    if (row >= a->rows) return;
    
    if (col >= b->cols) {
        recursive_fill_result(a, b, result, row + 1, 0);
        return;
    }
    
    result->data[row][col] = 0;
    recursive_matrix_multiply(a, b, result, row, col, 0);
    recursive_fill_result(a, b, result, row, col + 1);
}

int matrix_multiply(Matrix* a, Matrix* b, Matrix* result) {
    if (a->cols != b->rows) return 0;
    if (!validate_matrix_dimensions(a->rows, a->cols) || 
        !validate_matrix_dimensions(b->rows, b->cols)) return 0;
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    recursive_fill_result(a, b, result, 0, 0);
    return 1;
}

void recursive_print_matrix(Matrix* m, int row, int col) {
    if (row >= m->rows) {
        printf("\n");
        return;
    }
    
    if (col >= m->cols) {
        printf("\n");
        recursive_print_matrix(m, row + 1, 0);
        return;
    }
    
    printf("%4d", m->data[row][col]);
    if (col < m->cols - 1) printf(" ");
    recursive_print_matrix(m, row, col + 1);
}

void recursive_read_matrix(Matrix* m, int row, int col) {
    if (row >= m->rows) return;
    
    if (col >= m->cols) {
        recursive_read_matrix(m, row + 1, 0);
        return;
    }
    
    int value;
    if (scanf("%d", &value) != 1) {
        printf("Invalid input\n");
        exit(1);
    }
    
    if (value < -1000 || value > 1000) {
        printf("Value out of range\n");
        exit(1);
    }
    
    m->data[row][col] = value;
    recursive_read_matrix(m, row, col + 1);
}

int main() {
    Matrix a, b, result;
    
    printf("Enter rows and columns for matrix A: ");
    if (scanf("%d %d", &a.rows, &a.cols) != 2) {
        printf("Invalid dimensions\n");
        return 1;
    }
    
    if (!validate_matrix_dimensions(a.rows, a.cols)) {
        printf("Invalid dimensions\n");
        return 1;
    }
    
    printf("Enter %d elements for matrix A:\n", a.rows * a.cols);
    recursive_read_matrix(&a, 0, 0);
    
    printf("Enter rows and columns for matrix B: ");
    if (scanf("%d %d", &b.rows, &b.cols) != 2) {
        printf("Invalid dimensions\n");
        return 1;
    }
    
    if (!validate_matrix_dimensions(b.rows, b.cols)) {
        printf("Invalid dimensions\n");
        return 1;
    }
    
    printf("Enter %d elements for matrix B:\n", b.rows * b.cols);
    recursive_read_matrix(&b, 0, 0);
    
    if (!matrix_multiply(&a, &b, &result)) {
        printf("Matrix multiplication not possible\n");
        return 1;
    }
    
    printf("Result matrix:\n");
    recursive_print_matrix(&result, 0, 0);
    
    return 0;
}