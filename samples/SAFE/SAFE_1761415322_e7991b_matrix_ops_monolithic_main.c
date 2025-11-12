//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 10

int read_int(const char* prompt, int min_val, int max_val) {
    char buffer[32];
    int value;
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min_val;
        }
        if (sscanf(buffer, "%d", &value) == 1) {
            if (value >= min_val && value <= max_val) {
                return value;
            }
        }
        printf("Invalid input. Enter value between %d and %d.\n", min_val, max_val);
    }
}

void initialize_matrix(int rows, int cols, int matrix[MAX_DIM][MAX_DIM]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = rand() % 100;
        }
    }
}

void print_matrix(int rows, int cols, int matrix[MAX_DIM][MAX_DIM]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%4d", matrix[i][j]);
        }
        printf("\n");
    }
}

void add_matrices(int rows, int cols, int a[MAX_DIM][MAX_DIM], int b[MAX_DIM][MAX_DIM], int result[MAX_DIM][MAX_DIM]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = a[i][j] + b[i][j];
        }
    }
}

void multiply_matrices(int rows1, int cols1, int a[MAX_DIM][MAX_DIM], int rows2, int cols2, int b[MAX_DIM][MAX_DIM], int result[MAX_DIM][MAX_DIM]) {
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            result[i][j] = 0;
            for (int k = 0; k < cols1; k++) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

int main(void) {
    int rows1, cols1, rows2, cols2;
    int matrix_a[MAX_DIM][MAX_DIM];
    int matrix_b[MAX_DIM][MAX_DIM];
    int result[MAX_DIM][MAX_DIM];
    
    printf("Matrix Operations Program\n");
    
    rows1 = read_int("Enter rows for matrix A (1-10): ", 1, MAX_DIM);
    cols1 = read_int("Enter columns for matrix A (1-10): ", 1, MAX_DIM);
    rows2 = read_int("Enter rows for matrix B (1-10): ", 1, MAX_DIM);
    cols2 = read_int("Enter columns for matrix B (1-10): ", 1, MAX_DIM);
    
    initialize_matrix(rows1, cols1, matrix_a);
    initialize_matrix(rows2, cols2, matrix_b);
    
    printf("\nMatrix A:\n");
    print_matrix(rows1, cols1, matrix_a);
    printf("\nMatrix B:\n");
    print_matrix(rows2, cols2, matrix_b);
    
    printf("\nMatrix Addition:\n");
    if (rows1 == rows2 && cols1 == cols2) {
        add_matrices(rows1, cols1, matrix_a, matrix_b, result);
        print_matrix(rows1, cols1, result);
    } else {
        printf("Cannot add matrices - dimensions don't match.\n");
    }
    
    printf("\nMatrix Multiplication:\n");
    if (cols1 == rows2) {
        multiply_matrices(rows1, cols1, matrix_a, rows2, cols2, matrix_b, result);
        print_matrix(rows1, cols2, result);
    } else {
        printf("Cannot multiply matrices - inner dimensions don't match.\n");
    }
    
    return 0;
}