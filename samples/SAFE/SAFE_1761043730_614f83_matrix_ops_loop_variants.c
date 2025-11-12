//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 10

void print_matrix(int matrix[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void transpose_matrix(int src[MAX_SIZE][MAX_SIZE], int dest[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    int i = 0;
    while (i < rows) {
        int j = 0;
        do {
            dest[j][i] = src[i][j];
            j++;
        } while (j < cols);
        i++;
    }
}

void multiply_matrices(int a[MAX_SIZE][MAX_SIZE], int b[MAX_SIZE][MAX_SIZE], int result[MAX_SIZE][MAX_SIZE], int rows_a, int cols_a, int cols_b) {
    for (int i = 0; i < rows_a; i++) {
        for (int j = 0; j < cols_b; j++) {
            result[i][j] = 0;
            int k = 0;
            while (k < cols_a) {
                if (a[i][k] > 0 && b[k][j] > INT_MAX / a[i][k]) {
                    printf("Integer overflow detected in multiplication\n");
                    return;
                }
                if (a[i][k] < 0 && b[k][j] < INT_MAX / a[i][k]) {
                    printf("Integer overflow detected in multiplication\n");
                    return;
                }
                result[i][j] += a[i][k] * b[k][j];
                k++;
            }
        }
    }
}

int read_int(const char* prompt, int min, int max) {
    char buffer[32];
    long value;
    char* endptr;
    
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return -1;
    }
    
    value = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n') {
        return -1;
    }
    if (value < min || value > max) {
        return -1;
    }
    
    return (int)value;
}

void fill_matrix(int matrix[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    int count = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = count++;
        }
    }
}

int main(void) {
    int rows_a, cols_a, rows_b, cols_b;
    int matrix_a[MAX_SIZE][MAX_SIZE];
    int matrix_b[MAX_SIZE][MAX_SIZE];
    int result[MAX_SIZE][MAX_SIZE];
    
    printf("Matrix Operations Program\n");
    
    rows_a = read_int("Enter rows for matrix A (1-10): ", 1, MAX_SIZE);
    if (rows_a == -1) {
        printf("Invalid input for rows A\n");
        return 1;
    }
    
    cols_a = read_int("Enter columns for matrix A (1-10): ", 1, MAX_SIZE);
    if (cols_a == -1) {
        printf("Invalid input for columns A\n");
        return 1;
    }
    
    rows_b = read_int("Enter rows for matrix B (1-10): ", 1, MAX_SIZE);
    if (rows_b == -1) {
        printf("Invalid input for rows B\n");
        return 1;
    }
    
    cols_b = read_int("Enter columns for matrix B (1-10): ", 1, MAX_SIZE);
    if (cols_b == -1) {
        printf("Invalid input for columns B\n");
        return 1;
    }
    
    fill_matrix(matrix_a, rows_a, cols_a);
    fill_matrix(matrix_b, rows_b, cols_b);
    
    printf("\nMatrix A:\n");
    print_matrix(matrix_a, rows_a, cols_a);
    
    printf("\nMatrix B:\n");
    print_matrix(matrix_b, rows_b, cols_b);
    
    printf("\nTranspose of A:\n");
    int transposed[MAX_SIZE][MAX_SIZE];
    transpose_matrix(matrix_a, transposed, rows_a, cols_a);
    print_matrix(transposed, cols_a, rows_a);
    
    if (cols_a == rows_b) {
        printf("\nMatrix A * B:\n");
        multiply_matrices(matrix_a, matrix_b, result, rows_a, cols_a, cols_b);
        print_matrix(result, rows_a, cols_b);
    } else {
        printf("\nCannot multiply matrices: columns of A must equal rows of B\n");
    }
    
    return 0;
}