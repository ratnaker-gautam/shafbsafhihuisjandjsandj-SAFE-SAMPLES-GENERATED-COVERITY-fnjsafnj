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
    char buffer[32];
    int value;
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Input error\n");
            continue;
        }
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input\n");
            continue;
        }
        if (value >= min && value <= max) {
            return value;
        }
        printf("Value must be between %d and %d\n", min, max);
    }
}

void read_matrix_elements(Matrix* m, int row, int col) {
    if (row >= m->rows) return;
    if (col >= m->cols) {
        read_matrix_elements(m, row + 1, 0);
        return;
    }
    char prompt[32];
    snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", row, col);
    m->data[row][col] = read_int_safe(prompt, -100, 100);
    read_matrix_elements(m, row, col + 1);
}

Matrix multiply_matrices_recursive(Matrix a, Matrix b, Matrix result, int i, int j, int k) {
    if (i >= a.rows) return result;
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
        printf("Matrix dimensions incompatible for multiplication\n");
        exit(1);
    }
    Matrix result = create_matrix(a.rows, b.cols);
    return multiply_matrices_recursive(a, b, result, 0, 0, 0);
}

int main() {
    printf("Matrix Multiplication Program\n");
    
    printf("Matrix A:\n");
    int rows_a = read_int_safe("Enter rows (1-10): ", 1, MAX_SIZE);
    int cols_a = read_int_safe("Enter columns (1-10): ", 1, MAX_SIZE);
    Matrix a = create_matrix(rows_a, cols_a);
    read_matrix_elements(&a, 0, 0);
    
    printf("Matrix B:\n");
    int rows_b = read_int_safe("Enter rows (1-10): ", 1, MAX_SIZE);
    int cols_b = read_int_safe("Enter columns (1-10): ", 1, MAX_SIZE);
    Matrix b = create_matrix(rows_b, cols_b);
    read_matrix_elements(&b, 0, 0);
    
    if (a.cols != b.rows) {
        printf("Error: Cannot multiply matrices (A.cols != B.rows)\n");
        return 1;
    }
    
    printf("\nMatrix A:\n");
    print_matrix(a);
    printf("\nMatrix B:\n");
    print_matrix(b);
    
    Matrix result = multiply_matrices(a, b);
    printf("\nResult (A x B):\n");
    print_matrix(result);
    
    return 0;
}