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

int multiply_helper(Matrix a, Matrix b, Matrix *result, int i, int j, int k) {
    if (k >= a.cols) return 0;
    result->data[i][j] += a.data[i][k] * b.data[k][j];
    return multiply_helper(a, b, result, i, j, k + 1);
}

int multiply_row_col(Matrix a, Matrix b, Matrix *result, int i, int j) {
    if (j >= result->cols) return 0;
    result->data[i][j] = 0;
    multiply_helper(a, b, result, i, j, 0);
    return multiply_row_col(a, b, result, i, j + 1);
}

int multiply_matrices_recursive(Matrix a, Matrix b, Matrix *result, int i) {
    if (i >= result->rows) return 0;
    multiply_row_col(a, b, result, i, 0);
    return multiply_matrices_recursive(a, b, result, i + 1);
}

Matrix matrix_multiply(Matrix a, Matrix b) {
    Matrix result = create_matrix(a.rows, b.cols);
    if (a.cols != b.rows) {
        printf("Error: Matrix dimensions incompatible for multiplication\n");
        result.rows = 0;
        result.cols = 0;
        return result;
    }
    multiply_matrices_recursive(a, b, &result, 0);
    return result;
}

int read_int_safe(const char *prompt, int min, int max) {
    char buffer[100];
    int value;
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Error reading input\n");
            continue;
        }
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        if (value < min || value > max) {
            printf("Value must be between %d and %d\n", min, max);
            continue;
        }
        return value;
    }
}

void read_matrix_elements(Matrix *m, int i, int j) {
    if (i >= m->rows) return;
    if (j >= m->cols) {
        read_matrix_elements(m, i + 1, 0);
        return;
    }
    char prompt[50];
    snprintf(prompt, sizeof(prompt), "Enter element [%d][%d]: ", i, j);
    m->data[i][j] = read_int_safe(prompt, -100, 100);
    read_matrix_elements(m, i, j + 1);
}

Matrix input_matrix(int matrix_num) {
    printf("Matrix %d:\n", matrix_num);
    int rows = read_int_safe("Enter number of rows (1-10): ", 1, MAX_SIZE);
    int cols = read_int_safe("Enter number of columns (1-10): ", 1, MAX_SIZE);
    Matrix m = create_matrix(rows, cols);
    read_matrix_elements(&m, 0, 0);
    return m;
}

int main() {
    printf("Matrix Multiplication using Recursion\n");
    printf("=====================================\n");
    
    Matrix a = input_matrix(1);
    Matrix b = input_matrix(2);
    
    printf("\nMatrix A:\n");
    print_matrix(a);
    printf("\nMatrix B:\n");
    print_matrix(b);
    
    Matrix result = matrix_multiply(a, b);
    if (result.rows > 0 && result.cols > 0) {
        printf("\nResult (A x B):\n");
        print_matrix(result);
    }
    
    return 0;
}