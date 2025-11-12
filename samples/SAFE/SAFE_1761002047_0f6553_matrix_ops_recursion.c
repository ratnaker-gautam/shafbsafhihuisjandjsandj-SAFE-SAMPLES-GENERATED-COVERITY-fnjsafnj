//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: matrix_ops
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

void read_matrix_elements(Matrix* m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Enter element [%d][%d]: ", i, j);
            m->data[i][j] = read_int_safe(prompt, -1000, 1000);
        }
    }
}

int matrix_trace_recursive(Matrix m, int row) {
    if (row >= m.rows || row >= m.cols) {
        return 0;
    }
    return m.data[row][row] + matrix_trace_recursive(m, row + 1);
}

Matrix matrix_multiply_recursive(Matrix a, Matrix b, int i, int j, int k) {
    Matrix result = create_matrix(a.rows, b.cols);
    if (i >= a.rows) {
        return result;
    }
    if (j >= b.cols) {
        return matrix_multiply_recursive(a, b, i + 1, 0, 0);
    }
    if (k >= a.cols) {
        result.data[i][j] = 0;
        for (int kk = 0; kk < a.cols; kk++) {
            result.data[i][j] += a.data[i][kk] * b.data[kk][j];
        }
        return matrix_multiply_recursive(a, b, i, j + 1, 0);
    }
    return matrix_multiply_recursive(a, b, i, j, k + 1);
}

int main() {
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    int rows1 = read_int_safe("Enter rows for matrix 1 (1-10): ", 1, MAX_SIZE);
    int cols1 = read_int_safe("Enter columns for matrix 1 (1-10): ", 1, MAX_SIZE);
    
    Matrix mat1 = create_matrix(rows1, cols1);
    printf("Enter elements for matrix 1:\n");
    read_matrix_elements(&mat1);
    
    printf("\nMatrix 1:\n");
    print_matrix(mat1);
    
    if (rows1 == cols1) {
        int trace = matrix_trace_recursive(mat1, 0);
        printf("Trace of matrix 1: %d\n", trace);
    }
    
    int rows2 = read_int_safe("Enter rows for matrix 2 (1-10): ", 1, MAX_SIZE);
    int cols2 = read_int_safe("Enter columns for matrix 2 (1-10): ", 1, MAX_SIZE);
    
    if (cols1 != rows2) {
        printf("Matrix multiplication not possible: columns of matrix 1 must equal rows of matrix 2\n");
        return 1;
    }
    
    Matrix mat2 = create_matrix(rows2, cols2);
    printf("Enter elements for matrix 2:\n");
    read_matrix_elements(&mat2);
    
    printf("\nMatrix 2:\n");
    print_matrix(mat2);
    
    Matrix product = matrix_multiply_recursive(mat1, mat2, 0, 0, 0);
    printf("\nMatrix product:\n");
    print_matrix(product);
    
    return 0;
}