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
    if (row >= m->rows) {
        return;
    }
    if (col >= m->cols) {
        read_matrix_elements(m, row + 1, 0);
        return;
    }
    printf("Enter element [%d][%d]: ", row, col);
    char buffer[100];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        m->data[row][col] = 0;
    } else {
        int value;
        if (sscanf(buffer, "%d", &value) == 1) {
            m->data[row][col] = value;
        } else {
            m->data[row][col] = 0;
        }
    }
    read_matrix_elements(m, row, col + 1);
}

Matrix multiply_matrices_recursive(Matrix a, Matrix b, Matrix result, int i, int j, int k, int sum) {
    if (i >= a.rows) {
        return result;
    }
    if (j >= b.cols) {
        return multiply_matrices_recursive(a, b, result, i + 1, 0, 0, 0);
    }
    if (k >= a.cols) {
        result.data[i][j] = sum;
        return multiply_matrices_recursive(a, b, result, i, j + 1, 0, 0);
    }
    sum += a.data[i][k] * b.data[k][j];
    return multiply_matrices_recursive(a, b, result, i, j, k + 1, sum);
}

Matrix multiply_matrices(Matrix a, Matrix b) {
    if (a.cols != b.rows) {
        Matrix empty = create_matrix(0, 0);
        return empty;
    }
    Matrix result = create_matrix(a.rows, b.cols);
    return multiply_matrices_recursive(a, b, result, 0, 0, 0, 0);
}

int main() {
    printf("Matrix Multiplication Program\n");
    
    int rows1 = read_int_safe("Enter rows for matrix 1 (1-10): ", 1, MAX_SIZE);
    int cols1 = read_int_safe("Enter columns for matrix 1 (1-10): ", 1, MAX_SIZE);
    
    int rows2 = read_int_safe("Enter rows for matrix 2 (1-10): ", 1, MAX_SIZE);
    int cols2 = read_int_safe("Enter columns for matrix 2 (1-10): ", 1, MAX_SIZE);
    
    if (cols1 != rows2) {
        printf("Matrix multiplication not possible: columns of first must equal rows of second\n");
        return 1;
    }
    
    printf("Enter elements for matrix 1:\n");
    Matrix mat1 = create_matrix(rows1, cols1);
    read_matrix_elements(&mat1, 0, 0);
    
    printf("Enter elements for matrix 2:\n");
    Matrix mat2 = create_matrix(rows2, cols2);
    read_matrix_elements(&mat2, 0, 0);
    
    printf("Matrix 1:\n");
    print_matrix(mat1);
    
    printf("Matrix 2:\n");
    print_matrix(mat2);
    
    Matrix result = multiply_matrices(mat1, mat2);
    
    printf("Result of multiplication:\n");
    print_matrix(result);
    
    return 0;
}