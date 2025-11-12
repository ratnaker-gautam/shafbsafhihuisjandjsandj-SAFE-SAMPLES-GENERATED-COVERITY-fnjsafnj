//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: matrix_ops
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

void print_matrix(const Matrix *m) {
    print_matrix_recursive(m, 0, 0);
}

int matrix_trace_recursive(const Matrix *m, int n, int sum) {
    if (n >= m->rows || n >= m->cols) {
        return sum;
    }
    
    if (sum > 0 && m->data[n][n] > INT_MAX - sum) {
        printf("Integer overflow detected\n");
        exit(1);
    }
    if (sum < 0 && m->data[n][n] < INT_MIN - sum) {
        printf("Integer overflow detected\n");
        exit(1);
    }
    
    return matrix_trace_recursive(m, n + 1, sum + m->data[n][n]);
}

int matrix_trace(const Matrix *m) {
    if (m->rows != m->cols) {
        printf("Matrix must be square for trace calculation\n");
        exit(1);
    }
    return matrix_trace_recursive(m, 0, 0);
}

Matrix matrix_multiply_recursive(const Matrix *a, const Matrix *b, Matrix *result, int i, int j, int k, int sum) {
    if (i >= a->rows) {
        return *result;
    }
    
    if (j >= b->cols) {
        return matrix_multiply_recursive(a, b, result, i + 1, 0, 0, 0);
    }
    
    if (k >= a->cols) {
        result->data[i][j] = sum;
        return matrix_multiply_recursive(a, b, result, i, j + 1, 0, 0);
    }
    
    long long product = (long long)a->data[i][k] * b->data[k][j];
    if (product > INT_MAX || product < INT_MIN) {
        printf("Integer overflow in multiplication\n");
        exit(1);
    }
    
    long long new_sum = (long long)sum + product;
    if (new_sum > INT_MAX || new_sum < INT_MIN) {
        printf("Integer overflow in summation\n");
        exit(1);
    }
    
    return matrix_multiply_recursive(a, b, result, i, j, k + 1, (int)new_sum);
}

Matrix matrix_multiply(const Matrix *a, const Matrix *b) {
    if (a->cols != b->rows) {
        printf("Matrix dimensions incompatible for multiplication\n");
        exit(1);
    }
    
    Matrix result;
    result.rows = a->rows;
    result.cols = b->cols;
    
    return matrix_multiply_recursive(a, b, &result, 0, 0, 0, 0);
}

int main() {
    Matrix a, b;
    
    printf("Matrix A:\n");
    read_matrix(&a);
    
    printf("Matrix B:\n");
    read_matrix(&b);
    
    printf("\nMatrix A:\n");
    print_matrix(&a);
    
    printf("\nMatrix B:\n");
    print_matrix(&b);
    
    if (a.rows == a.cols) {
        int trace = matrix_trace(&a);
        printf("\nTrace of Matrix A: %d\n", trace);
    }
    
    if (b.rows == b.cols) {
        int trace = matrix_trace(&b);
        printf("Trace of Matrix B: %d\n", trace);
    }
    
    Matrix product = matrix_multiply(&a, &b);
    printf("\nMatrix A * B:\n");
    print_matrix(&product);
    
    return 0;
}