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
        fprintf(stderr, "Invalid input\n");
        exit(1);
    }
    
    if (!validate_matrix_size(m->rows, m->cols)) {
        fprintf(stderr, "Invalid matrix size\n");
        exit(1);
    }
    
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            if (scanf("%d", &m->data[i][j]) != 1) {
                fprintf(stderr, "Invalid matrix element\n");
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

int recursive_trace(const Matrix *m, int row, int col, int sum) {
    if (row >= m->rows || col >= m->cols) {
        return sum;
    }
    
    if (m->rows != m->cols) {
        return 0;
    }
    
    int new_sum;
    if (sum > INT_MAX - m->data[row][col]) {
        fprintf(stderr, "Integer overflow detected\n");
        exit(1);
    }
    new_sum = sum + m->data[row][col];
    
    return recursive_trace(m, row + 1, col + 1, new_sum);
}

int matrix_trace(const Matrix *m) {
    if (m->rows != m->cols) {
        return 0;
    }
    return recursive_trace(m, 0, 0, 0);
}

Matrix recursive_multiply(const Matrix *a, const Matrix *b, int row, int col, int k, int sum) {
    Matrix result;
    result.rows = a->rows;
    result.cols = b->cols;
    
    if (row >= a->rows) {
        return result;
    }
    
    if (col >= b->cols) {
        Matrix next_row = recursive_multiply(a, b, row + 1, 0, 0, 0);
        for (int i = 0; i < a->rows; i++) {
            for (int j = 0; j < b->cols; j++) {
                result.data[i][j] = next_row.data[i][j];
            }
        }
        return result;
    }
    
    if (k >= a->cols) {
        result.data[row][col] = sum;
        Matrix next_col = recursive_multiply(a, b, row, col + 1, 0, 0);
        for (int i = 0; i < a->rows; i++) {
            for (int j = 0; j < b->cols; j++) {
                result.data[i][j] = next_col.data[i][j];
            }
        }
        return result;
    }
    
    long long product = (long long)a->data[row][k] * b->data[k][col];
    if (product > INT_MAX || product < INT_MIN) {
        fprintf(stderr, "Integer overflow in multiplication\n");
        exit(1);
    }
    
    long long new_sum = (long long)sum + product;
    if (new_sum > INT_MAX || new_sum < INT_MIN) {
        fprintf(stderr, "Integer overflow in sum\n");
        exit(1);
    }
    
    return recursive_multiply(a, b, row, col, k + 1, (int)new_sum);
}

Matrix matrix_multiply(const Matrix *a, const Matrix *b) {
    if (a->cols != b->rows) {
        fprintf(stderr, "Matrix dimensions incompatible for multiplication\n");
        exit(1);
    }
    
    return recursive_multiply(a, b, 0, 0, 0, 0);
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
    
    if (a.cols == b.rows) {
        Matrix product = matrix_multiply(&a, &b);
        printf("\nMatrix A * B:\n");
        print_matrix(&product);
    }
    
    return 0;
}