//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 10

int read_int(const char *prompt, int min, int max) {
    char buffer[32];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min;
        }
        
        char *endptr;
        long temp = strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (temp < min || temp > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        value = (int)temp;
        break;
    }
    
    return value;
}

void fill_matrix(int *matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Enter element [%d][%d]: ", i, j);
            *(matrix + i * cols + j) = read_int("", INT_MIN, INT_MAX);
        }
    }
}

void print_matrix(int *matrix, int rows, int cols) {
    printf("Matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d ", *(matrix + i * cols + j));
        }
        printf("\n");
    }
}

void multiply_matrices(int *a, int *b, int *result, int a_rows, int a_cols, int b_cols) {
    for (int i = 0; i < a_rows; i++) {
        for (int j = 0; j < b_cols; j++) {
            *(result + i * b_cols + j) = 0;
            for (int k = 0; k < a_cols; k++) {
                *(result + i * b_cols + j) += *(a + i * a_cols + k) * *(b + k * b_cols + j);
            }
        }
    }
}

int main(void) {
    int rows1, cols1, rows2, cols2;
    
    printf("Matrix Multiplication using Pointer Arithmetic\n");
    printf("Matrix A dimensions:\n");
    rows1 = read_int("Rows (1-10): ", 1, MAX_DIM);
    cols1 = read_int("Columns (1-10): ", 1, MAX_DIM);
    
    printf("Matrix B dimensions:\n");
    rows2 = read_int("Rows (1-10): ", 1, MAX_DIM);
    cols2 = read_int("Columns (1-10): ", 1, MAX_DIM);
    
    if (cols1 != rows2) {
        printf("Error: Matrix A columns (%d) must equal Matrix B rows (%d) for multiplication.\n", cols1, rows2);
        return 1;
    }
    
    int matrix_a[MAX_DIM * MAX_DIM];
    int matrix_b[MAX_DIM * MAX_DIM];
    int result[MAX_DIM * MAX_DIM];
    
    printf("\nEnter values for Matrix A:\n");
    fill_matrix(matrix_a, rows1, cols1);
    
    printf("\nEnter values for Matrix B:\n");
    fill_matrix(matrix_b, rows2, cols2);
    
    printf("\n");
    print_matrix(matrix_a, rows1, cols1);
    printf("\n");
    print_matrix(matrix_b, rows2, cols2);
    
    multiply_matrices(matrix_a, matrix_b, result, rows1, cols1, cols2);
    
    printf("\nResult of A × B:\n");
    print_matrix(result, rows1, cols2);
    
    return 0;
}