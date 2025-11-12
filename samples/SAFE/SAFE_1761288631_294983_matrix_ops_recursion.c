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

int read_int_safe(int min, int max) {
    char buffer[100];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return -1;
    }
    char *endptr;
    long val = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n') {
        return -1;
    }
    if (val < min || val > max) {
        return -1;
    }
    return (int)val;
}

void read_matrix_input(Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("Enter element [%d][%d]: ", i, j);
            int val = read_int_safe(-1000, 1000);
            if (val == -1) {
                printf("Invalid input. Using 0.\n");
                val = 0;
            }
            m->data[i][j] = val;
        }
    }
}

Matrix multiply_recursive(Matrix a, Matrix b, int row, int col, int k) {
    Matrix result = create_matrix(a.rows, b.cols);
    if (row >= a.rows) {
        return result;
    }
    if (col >= b.cols) {
        return multiply_recursive(a, b, row + 1, 0, 0);
    }
    if (k >= a.cols) {
        result.data[row][col] = 0;
        return multiply_recursive(a, b, row, col + 1, 0);
    }
    Matrix partial = multiply_recursive(a, b, row, col, k + 1);
    partial.data[row][col] += a.data[row][k] * b.data[k][col];
    return partial;
}

Matrix matrix_multiply(Matrix a, Matrix b) {
    if (a.cols != b.rows) {
        Matrix empty = create_matrix(0, 0);
        return empty;
    }
    return multiply_recursive(a, b, 0, 0, 0);
}

int main() {
    printf("Matrix Multiplication using Recursion\n");
    printf("Enter dimensions for first matrix (rows cols, max %d): ", MAX_SIZE);
    int rows1 = read_int_safe(1, MAX_SIZE);
    int cols1 = read_int_safe(1, MAX_SIZE);
    if (rows1 == -1 || cols1 == -1) {
        printf("Invalid dimensions.\n");
        return 1;
    }
    Matrix mat1 = create_matrix(rows1, cols1);
    printf("Enter first matrix elements:\n");
    read_matrix_input(&mat1);

    printf("Enter dimensions for second matrix (rows cols, max %d): ", MAX_SIZE);
    int rows2 = read_int_safe(1, MAX_SIZE);
    int cols2 = read_int_safe(1, MAX_SIZE);
    if (rows2 == -1 || cols2 == -1) {
        printf("Invalid dimensions.\n");
        return 1;
    }
    if (cols1 != rows2) {
        printf("Matrix dimensions incompatible for multiplication.\n");
        return 1;
    }
    Matrix mat2 = create_matrix(rows2, cols2);
    printf("Enter second matrix elements:\n");
    read_matrix_input(&mat2);

    printf("\nFirst matrix:\n");
    print_matrix(mat1);
    printf("\nSecond matrix:\n");
    print_matrix(mat2);

    Matrix result = matrix_multiply(mat1, mat2);
    printf("\nResult matrix:\n");
    print_matrix(result);

    return 0;
}