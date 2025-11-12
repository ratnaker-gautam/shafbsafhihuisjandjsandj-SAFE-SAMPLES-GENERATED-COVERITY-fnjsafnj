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
    return rows > 0 && rows <= MAX_SIZE && cols > 0 && cols <= MAX_SIZE;
}

void read_matrix(Matrix *m) {
    printf("Enter matrix dimensions (rows cols, max %dx%d): ", MAX_SIZE, MAX_SIZE);
    if (scanf("%d %d", &m->rows, &m->cols) != 2) {
        printf("Invalid input\n");
        exit(1);
    }
    
    if (!validate_matrix_size(m->rows, m->cols)) {
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

void matrix_transpose_recursive(Matrix *dest, const Matrix *src, int row, int col) {
    if (row >= src->rows) return;
    
    dest->data[col][row] = src->data[row][col];
    
    if (col + 1 < src->cols) {
        matrix_transpose_recursive(dest, src, row, col + 1);
    } else {
        matrix_transpose_recursive(dest, src, row + 1, 0);
    }
}

int matrix_multiply_element_recursive(const Matrix *a, const Matrix *b, int row, int col, int k, int sum) {
    if (k >= a->cols) return sum;
    
    if (a->cols != b->rows) return 0;
    
    long long product = (long long)a->data[row][k] * b->data[k][col];
    if (product > INT_MAX || product < INT_MIN) return 0;
    
    long long new_sum = (long long)sum + product;
    if (new_sum > INT_MAX || new_sum < INT_MIN) return 0;
    
    return matrix_multiply_element_recursive(a, b, row, col, k + 1, (int)new_sum);
}

void matrix_multiply_recursive(Matrix *result, const Matrix *a, const Matrix *b, int row, int col) {
    if (row >= a->rows) return;
    
    if (a->cols != b->rows) {
        printf("Matrix multiplication not possible\n");
        exit(1);
    }
    
    result->data[row][col] = matrix_multiply_element_recursive(a, b, row, col, 0, 0);
    
    if (col + 1 < b->cols) {
        matrix_multiply_recursive(result, a, b, row, col + 1);
    } else {
        matrix_multiply_recursive(result, a, b, row + 1, 0);
    }
}

int main() {
    Matrix a, b, result;
    
    printf("Matrix A:\n");
    read_matrix(&a);
    
    printf("Matrix B:\n");
    read_matrix(&b);
    
    printf("\nMatrix A:\n");
    print_matrix_recursive(&a, 0, 0);
    
    printf("\nMatrix B:\n");
    print_matrix_recursive(&b, 0, 0);
    
    if (a.rows == a.cols) {
        int trace = matrix_trace_recursive(&a, 0, 0);
        printf("\nTrace of A: %d\n", trace);
    }
    
    if (b.rows == b.cols) {
        int trace = matrix_trace_recursive(&b, 0, 0);
        printf("Trace of B: %d\n", trace);
    }
    
    if (a.rows == b.cols && a.cols == b.rows) {
        Matrix trans_a, trans_b;
        trans_a.rows = a.cols;
        trans_a.cols = a.rows;
        trans_b.rows = b.cols;
        trans_b.cols = b.rows;
        
        matrix_transpose_recursive(&trans_a, &a, 0, 0);
        matrix_transpose_recursive(&trans_b, &b, 0, 0);
        
        printf("\nTranspose of A:\n");
        print_matrix_recursive(&trans_a, 0, 0);
        
        printf("\nTranspose of B:\n");
        print_matrix_recursive