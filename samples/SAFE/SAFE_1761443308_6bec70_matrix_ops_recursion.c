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
    Matrix result = create_matrix(a.rows, b.cols);
    
    if (row >= a.rows) {
        return result;
    }
    
    if (col >= b.cols) {
        return recursive_multiply(a, b, row + 1, 0, 0);
    }
    
    if (k >= a.cols) {
        Matrix sub_result = recursive_multiply(a, b, row, col + 1, 0);
        result.data[row][col] = result.data[row][col];
        for (int i = 0; i < a.rows; i++) {
            for (int j = 0; j < b.cols; j++) {
                result.data[i][j] = sub_result.data[i][j];
            }
        }
        return result;
    }
    
    result.data[row][col] += a.data[row][k] * b.data[k][col];
    Matrix temp = recursive_multiply(a, b, row, col, k + 1);
    for (int i = 0; i < a.rows; i++) {
        for (int j = 0; j < b.cols; j++) {
            result.data[i][j] += temp.data[i][j];
        }
    }
    return result;
}

Matrix multiply_matrices(Matrix a, Matrix b) {
    if (a.cols != b.rows) {
        printf("Error: Matrix dimensions incompatible for multiplication\n");
        exit(1);
    }
    return recursive_multiply(a, b, 0, 0, 0);
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
            m->data[i][j] = read_int(prompt, -100, 100);
        }
    }
}

int main() {
    printf("Matrix Multiplication using Recursion\n");
    printf("Maximum matrix size: %dx%d\n", MAX_SIZE, MAX_SIZE);
    
    int rows1 = read_int("Enter rows for matrix 1: ", 1, MAX_SIZE);
    int cols1 = read_int("Enter columns for matrix 1: ", 1, MAX_SIZE);
    Matrix mat1 = create_matrix(rows1, cols1);
    printf("Enter elements for matrix 1:\n");
    read_matrix_elements(&mat1);
    
    int rows2 = read_int("Enter rows for matrix 2: ", 1, MAX_SIZE);
    int cols2 = read_int("Enter columns for matrix 2: ", 1, MAX_SIZE);
    Matrix mat2 = create_matrix(rows2, cols2);
    printf("Enter elements for matrix 2:\n");
    read_matrix_elements(&mat2);
    
    printf("\nMatrix 1:\n");
    print_matrix(mat1);
    printf("\nMatrix 2:\n");
    print_matrix(mat2);
    
    if (mat1.cols != mat2.rows) {
        printf("Error: Cannot multiply matrices. Columns of first must equal rows of second.\n");
        return 1;
    }
    
    Matrix result = multiply_matrices(mat1, mat2);
    printf("\nResult of multiplication:\n");
    print_matrix(result);
    
    return 0;
}