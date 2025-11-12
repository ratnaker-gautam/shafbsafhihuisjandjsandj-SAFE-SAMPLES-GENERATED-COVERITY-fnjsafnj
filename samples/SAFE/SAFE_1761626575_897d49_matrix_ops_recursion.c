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

int validate_matrix_size(int rows, int cols) {
    return (rows > 0 && rows <= MAX_SIZE && cols > 0 && cols <= MAX_SIZE);
}

void print_matrix(Matrix m) {
    for (int i = 0; i < m.rows; i++) {
        for (int j = 0; j < m.cols; j++) {
            printf("%4d", m.data[i][j]);
        }
        printf("\n");
    }
}

void fill_matrix_recursive(Matrix *m, int row, int col, int value) {
    if (row >= m->rows || col >= m->cols) {
        return;
    }
    
    m->data[row][col] = value;
    
    if (col + 1 < m->cols) {
        fill_matrix_recursive(m, row, col + 1, value + 1);
    } else if (row + 1 < m->rows) {
        fill_matrix_recursive(m, row + 1, 0, value + 1);
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
        return multiply_matrices_recursive(a, b, row, col + 1, 0);
    }
    
    result = multiply_matrices_recursive(a, b, row, col, k + 1);
    result.data[row][col] += a.data[row][k] * b.data[k][col];
    
    return result;
}

int get_valid_int(const char* prompt, int min, int max) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            continue;
        }
        
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (value >= min && value <= max) {
            return value;
        }
        
        printf("Value must be between %d and %d.\n", min, max);
    }
}

int main() {
    printf("Matrix Operations with Recursion\n");
    printf("===============================\n\n");
    
    int rows1 = get_valid_int("Enter rows for matrix A (1-10): ", 1, MAX_SIZE);
    int cols1 = get_valid_int("Enter columns for matrix A (1-10): ", 1, MAX_SIZE);
    int rows2 = get_valid_int("Enter rows for matrix B (1-10): ", 1, MAX_SIZE);
    int cols2 = get_valid_int("Enter columns for matrix B (1-10): ", 1, MAX_SIZE);
    
    if (cols1 != rows2) {
        printf("Error: Matrix A columns must equal Matrix B rows for multiplication.\n");
        return 1;
    }
    
    Matrix A = create_matrix(rows1, cols1);
    Matrix B = create_matrix(rows2, cols2);
    
    printf("\nFilling matrix A with sequential values:\n");
    fill_matrix_recursive(&A, 0, 0, 1);
    print_matrix(A);
    
    printf("\nFilling matrix B with sequential values:\n");
    fill_matrix_recursive(&B, 0, 0, 1);
    print_matrix(B);
    
    printf("\nMultiplying matrices A and B:\n");
    Matrix C = multiply_matrices_recursive(A, B, 0, 0, 0);
    print_matrix(C);
    
    return 0;
}