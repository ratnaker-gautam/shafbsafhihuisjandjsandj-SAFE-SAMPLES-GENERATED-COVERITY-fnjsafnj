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

Matrix recursive_multiply(Matrix a, Matrix b, int row, int col, int k) {
    if (k == 0) {
        Matrix result = create_matrix(a.rows, b.cols);
        return result;
    }
    
    Matrix partial = recursive_multiply(a, b, row, col, k - 1);
    partial.data[row][col] += a.data[row][k - 1] * b.data[k - 1][col];
    
    if (col + 1 < b.cols) {
        return recursive_multiply(a, b, row, col + 1, a.cols);
    } else if (row + 1 < a.rows) {
        return recursive_multiply(a, b, row + 1, 0, a.cols);
    }
    
    return partial;
}

Matrix matrix_multiply(Matrix a, Matrix b) {
    if (a.cols != b.rows) {
        printf("Error: Matrix dimensions incompatible for multiplication\n");
        exit(1);
    }
    return recursive_multiply(a, b, 0, 0, a.cols);
}

int read_int(const char* prompt, int min, int max) {
    char buffer[100];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Error reading input\n");
            exit(1);
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

void read_matrix_elements(Matrix* m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Enter element [%d][%d]: ", i, j);
            m->data[i][j] = read_int(prompt, -1000, 1000);
        }
    }
}

int main() {
    printf("Matrix Multiplication using Recursion\n");
    printf("=====================================\n");
    
    int rows1 = read_int("Enter rows for matrix A (1-10): ", 1, MAX_SIZE);
    int cols1 = read_int("Enter columns for matrix A (1-10): ", 1, MAX_SIZE);
    int rows2 = read_int("Enter rows for matrix B (1-10): ", 1, MAX_SIZE);
    int cols2 = read_int("Enter columns for matrix B (1-10): ", 1, MAX_SIZE);
    
    if (cols1 != rows2) {
        printf("Error: Columns of A must equal rows of B for multiplication\n");
        return 1;
    }
    
    printf("\nEnter elements for matrix A:\n");
    Matrix a = create_matrix(rows1, cols1);
    read_matrix_elements(&a);
    
    printf("\nEnter elements for matrix B:\n");
    Matrix b = create_matrix(rows2, cols2);
    read_matrix_elements(&b);
    
    printf("\nMatrix A:\n");
    print_matrix(a);
    
    printf("\nMatrix B:\n");
    print_matrix(b);
    
    printf("\nResult of A * B:\n");
    Matrix result = matrix_multiply(a, b);
    print_matrix(result);
    
    return 0;
}