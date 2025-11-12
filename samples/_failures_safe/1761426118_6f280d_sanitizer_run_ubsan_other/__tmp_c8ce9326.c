//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 10
#define MATRIX_ELEM(mat, i, j, cols) ((mat)[(i) * (cols) + (j)])

int read_int(const char* prompt, int min, int max) {
    char buffer[32];
    int value;
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min;
        }
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        if (value < min || value > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        return value;
    }
}

void read_matrix(int* mat, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[64];
            snprintf(prompt, sizeof(prompt), "Enter element [%d][%d]: ", i, j);
            MATRIX_ELEM(mat, i, j, cols) = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

void print_matrix(int* mat, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d", MATRIX_ELEM(mat, i, j, cols));
        }
        printf("\n");
    }
}

void add_matrices(int* a, int* b, int* result, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            MATRIX_ELEM(result, i, j, cols) = MATRIX_ELEM(a, i, j, cols) + MATRIX_ELEM(b, i, j, cols);
        }
    }
}

void multiply_matrices(int* a, int a_rows, int a_cols, int* b, int b_rows, int b_cols, int* result) {
    for (int i = 0; i < a_rows; i++) {
        for (int j = 0; j < b_cols; j++) {
            int sum = 0;
            for (int k = 0; k < a_cols; k++) {
                sum += MATRIX_ELEM(a, i, k, a_cols) * MATRIX_ELEM(b, k, j, b_cols);
            }
            MATRIX_ELEM(result, i, j, b_cols) = sum;
        }
    }
}

int main() {
    int choice;
    int rows1, cols1, rows2, cols2;
    int matrix1[MAX_DIM * MAX_DIM];
    int matrix2[MAX_DIM * MAX_DIM];
    int result[MAX_DIM * MAX_DIM];
    
    printf("Matrix Operations Program\n");
    printf("1. Matrix Addition\n");
    printf("2. Matrix Multiplication\n");
    
    choice = read_int("Select operation (1-2): ", 1, 2);
    
    rows1 = read_int("Enter rows for first matrix (1-10): ", 1, MAX_DIM);
    cols1 = read_int("Enter columns for first matrix (1-10): ", 1, MAX_DIM);
    
    printf("Enter first matrix:\n");
    read_matrix(matrix1, rows1, cols1);
    
    if (choice == 1) {
        rows2 = read_int("Enter rows for second matrix (1-10): ", 1, MAX_DIM);
        cols2 = read_int("Enter columns for second matrix (1-10): ", 1, MAX_DIM);
        if (rows1 != rows2 || cols1 != cols2) {
            printf("Error: Matrices must have same dimensions for addition.\n");
            return 1;
        }
        printf("Enter second matrix:\n");
        read_matrix(matrix2, rows2, cols2);
        add_matrices(matrix1, matrix2, result, rows1, cols1);
        printf("Result of addition:\n");
        print_matrix(result, rows1, cols1);
    } else {
        rows2 = read_int("Enter rows for second matrix (1-10): ", 1, MAX_DIM);
        cols2 = read_int("Enter columns for second matrix (1-10): ", 1, MAX_DIM);
        if (cols1 != rows2) {
            printf("Error: First matrix columns must equal second matrix rows for multiplication.\n");
            return 1;
        }
        printf("Enter second matrix:\n");
        read_matrix(matrix2, rows2, cols2);
        multiply_matrices(matrix1, rows1, cols1, matrix2, rows2, cols2, result);
        printf("Result of multiplication:\n");
        print_matrix(result, rows1, cols2);
    }
    
    return 0;
}