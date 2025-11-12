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
    if (rows < 0) rows = 0;
    if (cols < 0) cols = 0;
    if (rows > MAX_SIZE) rows = MAX_SIZE;
    if (cols > MAX_SIZE) cols = MAX_SIZE;
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

Matrix add_matrices_recursive(Matrix a, Matrix b, int row, int col) {
    if (row >= a.rows) {
        return a;
    }
    
    Matrix result = a;
    if (row < MAX_SIZE && col < MAX_SIZE) {
        result.data[row][col] = a.data[row][col] + b.data[row][col];
    }
    
    if (col + 1 < a.cols) {
        return add_matrices_recursive(result, b, row, col + 1);
    } else {
        return add_matrices_recursive(result, b, row + 1, 0);
    }
}

Matrix add_matrices(Matrix a, Matrix b) {
    if (a.rows != b.rows || a.cols != b.cols) {
        Matrix empty = create_matrix(0, 0);
        return empty;
    }
    return add_matrices_recursive(a, b, 0, 0);
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
    
    if (i < MAX_SIZE && j < MAX_SIZE && k < MAX_SIZE) {
        result.data[i][j] += a.data[i][k] * b.data[k][j];
    }
    return multiply_matrices_recursive(a, b, result, i, j, k + 1);
}

Matrix multiply_matrices(Matrix a, Matrix b) {
    if (a.cols != b.rows) {
        Matrix empty = create_matrix(0, 0);
        return empty;
    }
    
    Matrix result = create_matrix(a.rows, b.cols);
    return multiply_matrices_recursive(a, b, result, 0, 0, 0);
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

void read_matrix_elements(Matrix* m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Enter element [%d][%d]: ", i, j);
            m->data[i][j] = read_int_safe(prompt, -1000, 1000);
        }
    }
}

int main() {
    printf("Matrix Operations Program\n");
    printf("=========================\n\n");
    
    int rows1 = read_int_safe("Enter rows for matrix 1 (1-10): ", 1, MAX_SIZE);
    int cols1 = read_int_safe("Enter columns for matrix 1 (1-10): ", 1, MAX_SIZE);
    
    Matrix mat1 = create_matrix(rows1, cols1);
    printf("Enter elements for matrix 1:\n");
    read_matrix_elements(&mat1);
    
    int rows2 = read_int_safe("Enter rows for matrix 2 (1-10): ", 1, MAX_SIZE);
    int cols2 = read_int_safe("Enter columns for matrix 2 (1-10): ", 1, MAX_SIZE);
    
    Matrix mat2 = create_matrix(rows2, cols2);
    printf("Enter elements for matrix 2:\n");
    read_matrix_elements(&mat2);
    
    printf("\nMatrix 1:\n");
    print_matrix(mat1);
    
    printf("\nMatrix 2:\n");
    print_matrix(mat2