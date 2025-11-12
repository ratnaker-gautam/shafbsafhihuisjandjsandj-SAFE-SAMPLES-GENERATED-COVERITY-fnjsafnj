//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 8

typedef struct {
    int rows;
    int cols;
    int data[MAX_SIZE][MAX_SIZE];
} Matrix;

Matrix create_matrix(int rows, int cols) {
    Matrix m = {0, 0, {{0}}};
    if (rows > 0 && rows <= MAX_SIZE && cols > 0 && cols <= MAX_SIZE) {
        m.rows = rows;
        m.cols = cols;
    }
    return m;
}

void print_matrix(Matrix m) {
    for (int i = 0; i < m.rows; i++) {
        for (int j = 0; j < m.cols; j++) {
            printf("%4d", m.data[i][j]);
        }
        printf("\n");
    }
}

int read_int_safe(const char* prompt, int min_val, int max_val) {
    char buffer[32];
    long val;
    char* endptr;
    
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return min_val - 1;
    }
    
    val = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n') {
        return min_val - 1;
    }
    if (val < min_val || val > max_val) {
        return min_val - 1;
    }
    
    return (int)val;
}

void fill_matrix_recursive(Matrix* m, int row, int col) {
    if (row >= m->rows) return;
    
    if (col >= m->cols) {
        fill_matrix_recursive(m, row + 1, 0);
        return;
    }
    
    printf("Enter element [%d][%d]: ", row, col);
    char buffer[32];
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        long val = strtol(buffer, NULL, 10);
        if (val >= INT_MIN && val <= INT_MAX) {
            m->data[row][col] = (int)val;
        }
    }
    
    fill_matrix_recursive(m, row, col + 1);
}

Matrix multiply_matrices_recursive(Matrix a, Matrix b, Matrix result, int i, int j, int k, int sum) {
    if (i >= a.rows) return result;
    
    if (j >= b.cols) {
        return multiply_matrices_recursive(a, b, result, i + 1, 0, 0, 0);
    }
    
    if (k >= a.cols) {
        result.data[i][j] = sum;
        return multiply_matrices_recursive(a, b, result, i, j + 1, 0, 0);
    }
    
    long product = (long)a.data[i][k] * (long)b.data[k][j];
    if (product > INT_MAX || product < INT_MIN) {
        product = 0;
    }
    
    long new_sum = (long)sum + product;
    if (new_sum > INT_MAX || new_sum < INT_MIN) {
        new_sum = sum > 0 ? INT_MAX : INT_MIN;
    }
    
    return multiply_matrices_recursive(a, b, result, i, j, k + 1, (int)new_sum);
}

Matrix multiply_matrices(Matrix a, Matrix b) {
    Matrix result = create_matrix(0, 0);
    if (a.cols != b.rows) {
        return result;
    }
    result = create_matrix(a.rows, b.cols);
    return multiply_matrices_recursive(a, b, result, 0, 0, 0, 0);
}

int main() {
    printf("Matrix Multiplication using Recursion\n");
    
    int rows1 = read_int_safe("Enter rows for matrix 1 (1-8): ", 1, MAX_SIZE);
    if (rows1 < 1) {
        printf("Invalid input for rows\n");
        return 1;
    }
    
    int cols1 = read_int_safe("Enter columns for matrix 1 (1-8): ", 1, MAX_SIZE);
    if (cols1 < 1) {
        printf("Invalid input for columns\n");
        return 1;
    }
    
    Matrix mat1 = create_matrix(rows1, cols1);
    printf("Fill matrix 1:\n");
    fill_matrix_recursive(&mat1, 0, 0);
    
    int rows2 = read_int_safe("Enter rows for matrix 2 (1-8): ", 1, MAX_SIZE);
    if (rows2 < 1) {
        printf("Invalid input for rows\n");
        return 1;
    }
    
    int cols2 = read_int_safe("Enter columns for matrix 2 (1-8): ", 1, MAX_SIZE);
    if (cols2 < 1) {
        printf("Invalid input for columns\n");
        return 1;
    }
    
    Matrix mat2 = create_matrix(rows2, cols2);
    printf("Fill matrix 2:\n");
    fill_matrix_recursive(&mat2, 0, 0);
    
    if (mat1.cols != mat2.rows) {
        printf("Matrix dimensions incompatible for multiplication\n");
        return 1;
    }
    
    Matrix result = multiply_matrices(mat1, mat2);
    
    printf("\nMatrix 1:\n");
    print_matrix(mat1);
    printf("\n