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
    if (rows <= 0 || rows > MAX_SIZE || cols <= 0 || cols > MAX_SIZE) {
        m.rows = 0;
        m.cols = 0;
        return m;
    }
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
    if (m.rows <= 0 || m.cols <= 0) return;
    for (int i = 0; i < m.rows; i++) {
        for (int j = 0; j < m.cols; j++) {
            printf("%d ", m.data[i][j]);
        }
        printf("\n");
    }
}

Matrix add_matrices_recursive(Matrix a, Matrix b, int row, int col) {
    Matrix result = create_matrix(a.rows, a.cols);
    if (a.rows != b.rows || a.cols != b.cols) {
        return create_matrix(0, 0);
    }
    if (row >= a.rows) {
        return result;
    }
    if (col >= a.cols) {
        return add_matrices_recursive(a, b, row + 1, 0);
    }
    result = add_matrices_recursive(a, b, row, col + 1);
    result.data[row][col] = a.data[row][col] + b.data[row][col];
    return result;
}

Matrix multiply_matrices_recursive(Matrix a, Matrix b, int i, int j, int k) {
    Matrix result = create_matrix(a.rows, b.cols);
    if (a.cols != b.rows) {
        return create_matrix(0, 0);
    }
    if (i >= a.rows) {
        return result;
    }
    if (j >= b.cols) {
        return multiply_matrices_recursive(a, b, i + 1, 0, 0);
    }
    if (k >= a.cols) {
        return multiply_matrices_recursive(a, b, i, j + 1, 0);
    }
    result = multiply_matrices_recursive(a, b, i, j, k + 1);
    result.data[i][j] += a.data[i][k] * b.data[k][j];
    return result;
}

int read_int_safe() {
    char buffer[32];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return -1;
    }
    char *endptr;
    long val = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n') {
        return -1;
    }
    if (val < 0 || val > 1000) {
        return -1;
    }
    return (int)val;
}

Matrix read_matrix() {
    int rows, cols;
    printf("Enter rows (1-%d): ", MAX_SIZE);
    rows = read_int_safe();
    printf("Enter cols (1-%d): ", MAX_SIZE);
    cols = read_int_safe();
    if (rows <= 0 || rows > MAX_SIZE || cols <= 0 || cols > MAX_SIZE) {
        return create_matrix(0, 0);
    }
    Matrix m = create_matrix(rows, cols);
    printf("Enter %d elements:\n", rows * cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int val = read_int_safe();
            if (val == -1) {
                return create_matrix(0, 0);
            }
            m.data[i][j] = val;
        }
    }
    return m;
}

int main() {
    printf("Matrix Operations Program\n");
    printf("1. Add matrices\n");
    printf("2. Multiply matrices\n");
    printf("Choose operation: ");
    
    int choice = read_int_safe();
    if (choice != 1 && choice != 2) {
        printf("Invalid choice\n");
        return 1;
    }
    
    printf("First matrix:\n");
    Matrix a = read_matrix();
    if (a.rows == 0) {
        printf("Invalid first matrix\n");
        return 1;
    }
    
    printf("Second matrix:\n");
    Matrix b = read_matrix();
    if (b.rows == 0) {
        printf("Invalid second matrix\n");
        return 1;
    }
    
    Matrix result;
    if (choice == 1) {
        result = add_matrices_recursive(a, b, 0, 0);
        if (result.rows == 0) {
            printf("Cannot add: dimension mismatch\n");
            return 1;
        }
        printf("Sum:\n");
    } else {
        result = multiply_matrices_recursive(a, b, 0, 0, 0);
        if (result.rows == 0) {
            printf("Cannot multiply: dimension mismatch\n");
            return 1;
        }
        printf("Product:\n");
    }
    print