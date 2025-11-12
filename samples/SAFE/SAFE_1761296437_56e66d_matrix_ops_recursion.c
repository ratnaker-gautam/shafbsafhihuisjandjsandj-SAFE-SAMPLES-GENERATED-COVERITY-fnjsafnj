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

int matrix_multiply_helper(const Matrix *a, const Matrix *b, Matrix *result, 
                          int i, int j, int k, int sum) {
    if (k >= a->cols) {
        return sum;
    }
    
    if (a->data[i][k] > 0 && b->data[k][j] > 0) {
        if (a->data[i][k] > INT_MAX / b->data[k][j]) {
            printf("Integer overflow detected\n");
            exit(1);
        }
    } else if (a->data[i][k] < 0 && b->data[k][j] < 0) {
        if (a->data[i][k] < INT_MIN / b->data[k][j]) {
            printf("Integer overflow detected\n");
            exit(1);
        }
    }
    
    int product = a->data[i][k] * b->data[k][j];
    
    if ((product > 0 && sum > INT_MAX - product) || 
        (product < 0 && sum < INT_MIN - product)) {
        printf("Integer overflow detected\n");
        exit(1);
    }
    
    return matrix_multiply_helper(a, b, result, i, j, k + 1, sum + product);
}

void matrix_multiply_recursive(const Matrix *a, const Matrix *b, Matrix *result, 
                              int i, int j) {
    if (i >= a->rows) {
        return;
    }
    
    if (j >= b->cols) {
        matrix_multiply_recursive(a, b, result, i + 1, 0);
        return;
    }
    
    result->data[i][j] = matrix_multiply_helper(a, b, result, i, j, 0, 0);
    matrix_multiply_recursive(a, b, result, i, j + 1);
}

int multiply_matrices(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->cols != b->rows) {
        printf("Matrix dimensions incompatible for multiplication\n");
        return 0;
    }
    
    if (!validate_matrix_size(a->rows, b->cols)) {
        printf("Result matrix would exceed maximum size\n");
        return 0;
    }
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    matrix_multiply_recursive(a, b, result, 0, 0);
    return 1;
}

int main() {
    Matrix a, b, result;
    
    printf("Matrix A:\n");
    read_matrix(&a);
    
    printf("Matrix B:\n");
    read_matrix(&b);
    
    printf("\nMatrix A:\n");
    print_matrix(&a);
    
    printf("\nMatrix B:\n");
    print_matrix(&b);
    
    if (multiply_matrices(&a, &b, &result)) {
        printf("\nMatrix A * B:\n");
        print_matrix(&result);
    } else {
        printf("Matrix multiplication failed\n");
        return 1;
    }
    
    return 0;
}