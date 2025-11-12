//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 10

void print_matrix(int matrix[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%4d", matrix[i][j]);
        }
        printf("\n");
    }
}

void transpose_matrix(int src[MAX_SIZE][MAX_SIZE], int dst[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    int i = 0;
    while (i < rows) {
        int j = 0;
        do {
            dst[j][i] = src[i][j];
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
                if (result[i][j] > INT_MAX - a[i][k] * b[k][j]) {
                    printf("Integer overflow detected in multiplication\n");
                    return;
                }
                result[i][j] += a[i][k] * b[k][j];
                k++;
            }
        }
    }
}

int read_int_safe(const char* prompt, int min_val, int max_val) {
    char buffer[32];
    long value;
    char* endptr;
    
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return -1;
    }
    
    value = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n' && *endptr != '\0') {
        return -1;
    }
    if (value < min_val || value > max_val) {
        return -1;
    }
    
    return (int)value;
}

int main(void) {
    int matrix_a[MAX_SIZE][MAX_SIZE];
    int matrix_b[MAX_SIZE][MAX_SIZE];
    int result[MAX_SIZE][MAX_SIZE];
    int rows_a, cols_a, rows_b, cols_b;
    
    printf("Matrix Operations Program\n");
    
    rows_a = read_int_safe("Enter rows for matrix A (1-10): ", 1, MAX_SIZE);
    if (rows_a == -1) {
        printf("Invalid input for rows A\n");
        return 1;
    }
    
    cols_a = read_int_safe("Enter columns for matrix A (1-10): ", 1, MAX_SIZE);
    if (cols_a == -1) {
        printf("Invalid input for columns A\n");
        return 1;
    }
    
    printf("Enter elements for matrix A:\n");
    for (int i = 0; i < rows_a; i++) {
        for (int j = 0; j < cols_a; j++) {
            int val = read_int_safe("", INT_MIN, INT_MAX);
            if (val == -1) {
                printf("Invalid matrix element\n");
                return 1;
            }
            matrix_a[i][j] = val;
        }
    }
    
    rows_b = read_int_safe("Enter rows for matrix B (1-10): ", 1, MAX_SIZE);
    if (rows_b == -1) {
        printf("Invalid input for rows B\n");
        return 1;
    }
    
    cols_b = read_int_safe("Enter columns for matrix B (1-10): ", 1, MAX_SIZE);
    if (cols_b == -1) {
        printf("Invalid input for columns B\n");
        return 1;
    }
    
    printf("Enter elements for matrix B:\n");
    for (int i = 0; i < rows_b; i++) {
        int j = 0;
        while (j < cols_b) {
            int val = read_int_safe("", INT_MIN, INT_MAX);
            if (val == -1) {
                printf("Invalid matrix element\n");
                return 1;
            }
            matrix_b[i][j] = val;
            j++;
        }
    }
    
    printf("\nMatrix A:\n");
    print_matrix(matrix_a, rows_a, cols_a);
    
    printf("\nMatrix B:\n");
    print_matrix(matrix_b, rows_b, cols_b);
    
    if (rows_a == cols_a) {
        int transposed[MAX_SIZE][MAX_SIZE];
        transpose_matrix(matrix_a, transposed, rows_a, cols_a);
        printf("\nTranspose of A:\n");
        print_matrix(transposed, cols_a, rows_a);
    }
    
    if (cols_a == rows_b) {
        multiply_matrices(matrix_a, matrix_b, result, rows_a, cols_a, cols_b);
        printf("\nMatrix A * B:\n");
        print_matrix(result, rows_a, cols_b);
    } else {
        printf("\nCannot multiply matrices: incompatible dimensions\n");
    }
    
    return 0;
}