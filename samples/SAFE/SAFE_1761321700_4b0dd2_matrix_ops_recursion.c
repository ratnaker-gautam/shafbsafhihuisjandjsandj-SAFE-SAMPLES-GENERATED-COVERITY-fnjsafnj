//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 10

typedef struct {
    int rows;
    int cols;
    int data[MAX_SIZE][MAX_SIZE];
} Matrix;

Matrix create_matrix(int rows, int cols) {
    Matrix m;
    m.rows = rows;
    m.cols = cols;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            m.data[i][j] = 0;
        }
    }
    return m;
}

void print_matrix(Matrix m) {
    for (int i = 0; i < m.rows; i++) {
        for (int j = 0; j < m.cols; j++) {
            printf("%d ", m.data[i][j]);
        }
        printf("\n");
    }
}

int validate_matrix_size(int rows, int cols) {
    if (rows <= 0 || cols <= 0) return 0;
    if (rows > MAX_SIZE || cols > MAX_SIZE) return 0;
    return 1;
}

int read_int_safe() {
    char buffer[32];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return -1;
    char *endptr;
    long val = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n') return -1;
    if (val < -1000 || val > 1000) return -1;
    return (int)val;
}

Matrix input_matrix() {
    int rows, cols;
    printf("Enter number of rows (1-%d): ", MAX_SIZE);
    rows = read_int_safe();
    printf("Enter number of columns (1-%d): ", MAX_SIZE);
    cols = read_int_safe();
    
    if (!validate_matrix_size(rows, cols)) {
        printf("Invalid matrix size\n");
        exit(1);
    }
    
    Matrix m = create_matrix(rows, cols);
    printf("Enter matrix elements row by row:\n");
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            int val = read_int_safe();
            if (val == -1) {
                printf("Invalid input\n");
                exit(1);
            }
            m.data[i][j] = val;
        }
    }
    return m;
}

Matrix multiply_matrices_recursive(Matrix a, Matrix b, int row, int col, int k) {
    Matrix result = create_matrix(a.rows, b.cols);
    
    if (row >= a.rows) return result;
    if (col >= b.cols) {
        return multiply_matrices_recursive(a, b, row + 1, 0, 0);
    }
    if (k >= a.cols) {
        result.data[row][col] = 0;
        return multiply_matrices_recursive(a, b, row, col + 1, 0);
    }
    
    Matrix partial = multiply_matrices_recursive(a, b, row, col, k + 1);
    partial.data[row][col] += a.data[row][k] * b.data[k][col];
    return partial;
}

Matrix multiply_matrices(Matrix a, Matrix b) {
    if (a.cols != b.rows) {
        printf("Matrix dimensions incompatible for multiplication\n");
        exit(1);
    }
    return multiply_matrices_recursive(a, b, 0, 0, 0);
}

int main() {
    printf("Matrix Multiplication using Recursion\n");
    printf("First matrix:\n");
    Matrix a = input_matrix();
    printf("Second matrix:\n");
    Matrix b = input_matrix();
    
    printf("\nFirst matrix:\n");
    print_matrix(a);
    printf("\nSecond matrix:\n");
    print_matrix(b);
    
    Matrix result = multiply_matrices(a, b);
    printf("\nResult matrix:\n");
    print_matrix(result);
    
    return 0;
}