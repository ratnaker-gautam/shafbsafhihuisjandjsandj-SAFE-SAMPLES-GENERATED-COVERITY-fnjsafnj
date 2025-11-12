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

void read_matrix_elements(Matrix* m, int row, int col) {
    if (row >= m->rows) {
        return;
    }
    if (col >= m->cols) {
        read_matrix_elements(m, row + 1, 0);
        return;
    }
    char prompt[50];
    snprintf(prompt, sizeof(prompt), "Enter element [%d][%d]: ", row, col);
    m->data[row][col] = read_int_safe(prompt, -1000, 1000);
    read_matrix_elements(m, row, col + 1);
}

Matrix multiply_matrices_recursive(Matrix a, Matrix b, Matrix result, int i, int j, int k) {
    if (i >= a.rows) {
        return result;
    }
    if (j >= b.cols) {
        return multiply_matrices_recursive(a, b, result, i + 1, 0, 0);
    }
    if (k >= a.cols) {
        return multiply_matrices_recursive(a, b, result, i, j + 1, 0);
    }
    result.data[i][j] += a.data[i][k] * b.data[k][j];
    return multiply_matrices_recursive(a, b, result, i, j, k + 1);
}

Matrix multiply_matrices(Matrix a, Matrix b) {
    if (a.cols != b.rows) {
        printf("Matrix dimensions incompatible for multiplication.\n");
        exit(1);
    }
    Matrix result = create_matrix(a.rows, b.cols);
    return multiply_matrices_recursive(a, b, result, 0, 0, 0);
}

int main() {
    printf("Matrix Multiplication using Recursion\n");
    printf("Matrix dimensions must be between 1 and %d.\n", MAX_SIZE);
    
    int rows1 = read_int_safe("Enter rows for matrix 1: ", 1, MAX_SIZE);
    int cols1 = read_int_safe("Enter columns for matrix 1: ", 1, MAX_SIZE);
    Matrix mat1 = create_matrix(rows1, cols1);
    printf("Enter elements for matrix 1:\n");
    read_matrix_elements(&mat1, 0, 0);
    
    int rows2 = read_int_safe("Enter rows for matrix 2: ", 1, MAX_SIZE);
    int cols2 = read_int_safe("Enter columns for matrix 2: ", 1, MAX_SIZE);
    Matrix mat2 = create_matrix(rows2, cols2);
    printf("Enter elements for matrix 2:\n");
    read_matrix_elements(&mat2, 0, 0);
    
    if (mat1.cols != mat2.rows) {
        printf("Matrix dimensions incompatible for multiplication.\n");
        return 1;
    }
    
    Matrix result = multiply_matrices(mat1, mat2);
    
    printf("\nMatrix 1:\n");
    print_matrix(mat1);
    printf("\nMatrix 2:\n");
    print_matrix(mat2);
    printf("\nResult of multiplication:\n");
    print_matrix(result);
    
    return 0;
}