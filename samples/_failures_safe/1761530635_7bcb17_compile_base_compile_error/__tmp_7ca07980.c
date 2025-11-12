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
    if (m.rows <= 0 || m.cols <= 0) {
        printf("Invalid matrix\n");
        return;
    }
    for (int i = 0; i < m.rows; i++) {
        for (int j = 0; j < m.cols; j++) {
            printf("%3d ", m.data[i][j]);
        }
        printf("\n");
    }
}

Matrix add_matrices_recursive(Matrix a, Matrix b, int row, int col) {
    Matrix result = create_matrix(a.rows, a.cols);
    if (a.rows != b.rows || a.cols != b.cols) {
        return result;
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
        return result;
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

int read_int_safe(const char* prompt, int min, int max) {
    char buffer[100];
    int value;
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min;
        }
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        if (value < min || value > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        return value;
    }
}

void fill_matrix_interactive(Matrix* m) {
    if (m->rows <= 0 || m->cols <= 0) return;
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Enter value for [%d][%d]: ", i, j);
            m->data[i][j] = read_int_safe(prompt, -1000, 1000);
        }
    }
}

int main() {
    printf("Matrix Operations using Recursion\n");
    printf("=================================\n");
    
    int rows1 = read_int_safe("Enter rows for matrix A (1-10): ", 1, MAX_SIZE);
    int cols1 = read_int_safe("Enter columns for matrix A (1-10): ", 1, MAX_SIZE);
    Matrix A = create_matrix(rows1, cols1);
    printf("Filling matrix A:\n");
    fill_matrix_interactive(&A);
    
    int rows2 = read_int_safe("Enter rows for matrix B (1-10): ", 1, MAX_SIZE);
    int cols2 = read_int_safe("Enter columns for matrix B (1-10): ", 1, MAX_SIZE);
    Matrix B = create_matrix(rows2, cols2);
    printf("Filling matrix B:\n");
    fill_matrix_interactive(&B);
    
    printf("\nMatrix A:\n");
    print_matrix(A);
    printf("\nMatrix B:\n");
    print_matrix(B);
    
    printf("\nMatrix Addition:\n");
    Matrix sum = add_matrices_recursive(A, B, 0, 0);
    if (sum.rows > 0 && sum.cols > 0) {
        print_matrix(sum);
    } else {
        printf("Addition not possible - dimension mismatch\n");
    }
    
    printf("\nMatrix Multiplication:\n");
    Matrix product = multiply_matrices_recursive(A, B, 0, 0, 0);
    if (product.