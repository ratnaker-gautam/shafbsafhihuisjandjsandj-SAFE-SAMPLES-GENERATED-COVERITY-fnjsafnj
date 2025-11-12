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

int read_int_safe(const char* prompt, int min, int max) {
    char buffer[100];
    int value;
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            continue;
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

void read_matrix_elements(Matrix* m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Enter element [%d][%d]: ", i, j);
            m->data[i][j] = read_int_safe(prompt, -1000, 1000);
        }
    }
}

Matrix multiply_matrices_recursive(Matrix a, Matrix b, int row, int col, int k) {
    Matrix result = create_matrix(a.rows, b.cols);
    
    if (row >= a.rows) {
        return result;
    }
    
    if (col >= b.cols) {
        return multiply_matrices_recursive(a, b, row + 1, 0, 0);
    }
    
    if (k >= a.cols) {
        result.data[row][col] = 0;
        for (int i = 0; i < a.cols; i++) {
            result.data[row][col] += a.data[row][i] * b.data[i][col];
        }
        return multiply_matrices_recursive(a, b, row, col + 1, 0);
    }
    
    result.data[row][col] += a.data[row][k] * b.data[k][col];
    return multiply_matrices_recursive(a, b, row, col, k + 1);
}

Matrix multiply_matrices(Matrix a, Matrix b) {
    if (a.cols != b.rows) {
        Matrix empty = create_matrix(0, 0);
        return empty;
    }
    return multiply_matrices_recursive(a, b, 0, 0, 0);
}

int main() {
    printf("Matrix Multiplication using Recursion\n");
    
    int rows1 = read_int_safe("Enter rows for matrix 1 (1-10): ", 1, MAX_SIZE);
    int cols1 = read_int_safe("Enter columns for matrix 1 (1-10): ", 1, MAX_SIZE);
    Matrix mat1 = create_matrix(rows1, cols1);
    read_matrix_elements(&mat1);
    
    int rows2 = read_int_safe("Enter rows for matrix 2 (1-10): ", 1, MAX_SIZE);
    int cols2 = read_int_safe("Enter columns for matrix 2 (1-10): ", 1, MAX_SIZE);
    Matrix mat2 = create_matrix(rows2, cols2);
    read_matrix_elements(&mat2);
    
    if (mat1.cols != mat2.rows) {
        printf("Error: Matrix dimensions incompatible for multiplication.\n");
        return 1;
    }
    
    Matrix result = multiply_matrices(mat1, mat2);
    
    printf("\nMatrix 1:\n");
    print_matrix(mat1);
    printf("\nMatrix 2:\n");
    print_matrix(mat2);
    printf("\nResult:\n");
    print_matrix(result);
    
    return 0;
}