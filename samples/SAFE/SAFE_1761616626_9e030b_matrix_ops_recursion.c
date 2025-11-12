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

void print_matrix(const Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%d ", m->data[i][j]);
        }
        printf("\n");
    }
}

int recursive_matrix_sum(const Matrix *m, int row, int col) {
    if (row >= m->rows) return 0;
    
    int next_row = row;
    int next_col = col + 1;
    if (next_col >= m->cols) {
        next_row++;
        next_col = 0;
    }
    
    int current = m->data[row][col];
    if (current > 0 && current > INT_MAX - recursive_matrix_sum(m, next_row, next_col)) {
        printf("Integer overflow detected\n");
        exit(1);
    }
    
    return current + recursive_matrix_sum(m, next_row, next_col);
}

Matrix recursive_matrix_multiply(const Matrix *a, const Matrix *b, int row, int col, int k) {
    Matrix result;
    result.rows = a->rows;
    result.cols = b->cols;
    
    if (row >= a->rows) {
        return result;
    }
    
    if (k >= a->cols) {
        Matrix next = recursive_matrix_multiply(a, b, row, col + 1, 0);
        if (col + 1 >= b->cols) {
            next = recursive_matrix_multiply(a, b, row + 1, 0, 0);
        }
        return next;
    }
    
    int sum = 0;
    for (int i = k; i < a->cols; i++) {
        if (a->data[row][i] > 0 && b->data[i][col] > 0 && a->data[row][i] > INT_MAX / b->data[i][col]) {
            printf("Multiplication overflow detected\n");
            exit(1);
        }
        int product = a->data[row][i] * b->data[i][col];
        if (product > 0 && sum > INT_MAX - product) {
            printf("Sum overflow detected\n");
            exit(1);
        }
        sum += product;
    }
    
    result.data[row][col] = sum;
    Matrix next = recursive_matrix_multiply(a, b, row, col, a->cols);
    result.data[row][col] = next.data[row][col];
    return result;
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
    
    int sum_a = recursive_matrix_sum(&a, 0, 0);
    printf("\nSum of all elements in matrix A: %d\n", sum_a);
    
    int sum_b = recursive_matrix_sum(&b, 0, 0);
    printf("Sum of all elements in matrix B: %d\n", sum_b);
    
    if (a.cols != b.rows) {
        printf("Cannot multiply matrices: incompatible dimensions\n");
        return 1;
    }
    
    Matrix product = recursive_matrix_multiply(&a, &b, 0, 0, 0);
    printf("\nMatrix product A x B:\n");
    print_matrix(&product);
    
    return 0;
}