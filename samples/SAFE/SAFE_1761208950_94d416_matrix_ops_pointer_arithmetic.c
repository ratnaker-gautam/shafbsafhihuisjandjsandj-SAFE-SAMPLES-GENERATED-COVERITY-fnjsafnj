//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 10

void matrix_multiply(int *a, int *b, int *result, int rows_a, int cols_a, int cols_b) {
    if (a == NULL || b == NULL || result == NULL) return;
    if (rows_a <= 0 || cols_a <= 0 || cols_b <= 0) return;
    
    for (int i = 0; i < rows_a; i++) {
        for (int j = 0; j < cols_b; j++) {
            *(result + i * cols_b + j) = 0;
            for (int k = 0; k < cols_a; k++) {
                if (__builtin_mul_overflow(*(a + i * cols_a + k), *(b + k * cols_b + j), 
                    &(*(result + i * cols_b + j)))) {
                    *(result + i * cols_b + j) = 0;
                    break;
                }
            }
        }
    }
}

void matrix_transpose(int *matrix, int *result, int rows, int cols) {
    if (matrix == NULL || result == NULL) return;
    if (rows <= 0 || cols <= 0) return;
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            *(result + j * rows + i) = *(matrix + i * cols + j);
        }
    }
}

void print_matrix(int *matrix, int rows, int cols) {
    if (matrix == NULL) return;
    if (rows <= 0 || cols <= 0) return;
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%4d", *(matrix + i * cols + j));
        }
        printf("\n");
    }
}

int read_int(const char *prompt, int min, int max) {
    char buffer[32];
    char *endptr;
    long value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min;
        }
        
        value = strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (value < min || value > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        return (int)value;
    }
}

void initialize_matrix(int *matrix, int rows, int cols) {
    if (matrix == NULL) return;
    if (rows <= 0 || cols <= 0) return;
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Enter element [%d][%d]: ", i, j);
            *(matrix + i * cols + j) = read_int("", INT_MIN, INT_MAX);
        }
    }
}

int main() {
    int rows_a, cols_a, rows_b, cols_b;
    int matrix_a[MAX_SIZE * MAX_SIZE];
    int matrix_b[MAX_SIZE * MAX_SIZE];
    int result[MAX_SIZE * MAX_SIZE];
    
    printf("Matrix Operations using Pointer Arithmetic\n");
    printf("==========================================\n\n");
    
    printf("Matrix A dimensions:\n");
    rows_a = read_int("Rows (1-10): ", 1, MAX_SIZE);
    cols_a = read_int("Columns (1-10): ", 1, MAX_SIZE);
    
    printf("\nEnter elements for Matrix A:\n");
    initialize_matrix(matrix_a, rows_a, cols_a);
    
    printf("\nMatrix B dimensions:\n");
    rows_b = read_int("Rows (1-10): ", 1, MAX_SIZE);
    cols_b = read_int("Columns (1-10): ", 1, MAX_SIZE);
    
    printf("\nEnter elements for Matrix B:\n");
    initialize_matrix(matrix_b, rows_b, cols_b);
    
    printf("\nMatrix A:\n");
    print_matrix(matrix_a, rows_a, cols_a);
    
    printf("\nMatrix B:\n");
    print_matrix(matrix_b, rows_b, cols_b);
    
    if (cols_a == rows_b) {
        printf("\nMatrix Multiplication (A × B):\n");
        matrix_multiply(matrix_a, matrix_b, result, rows_a, cols_a, cols_b);
        print_matrix(result, rows_a, cols_b);
    } else {
        printf("\nCannot multiply: columns of A (%d) must equal rows of B (%d)\n", cols_a, rows_b);
    }
    
    printf("\nTranspose of Matrix A:\n");
    int transpose_a[MAX_SIZE * MAX_SIZE];
    matrix_transpose(matrix_a, transpose_a, rows_a, cols_a);
    print_matrix(transpose_a, cols_a, rows_a);
    
    printf("\nTranspose of Matrix B:\n");
    int transpose_b[MAX_SIZE * MAX_SIZE];
    matrix_transpose(matrix_b, transpose_b, rows_b, cols_b);
    print_matrix(transpose_b, cols_b, rows_b);
    
    return 0;
}