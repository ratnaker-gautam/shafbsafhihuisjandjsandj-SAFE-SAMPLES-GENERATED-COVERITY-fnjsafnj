//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 100

int read_int(const char* prompt, int min, int max) {
    char buffer[32];
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

void fill_matrix(int* matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Enter element [%d][%d]: ", i, j);
            *(matrix + i * cols + j) = read_int("", INT_MIN, INT_MAX);
        }
    }
}

void print_matrix(int* matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d ", *(matrix + i * cols + j));
        }
        printf("\n");
    }
}

void multiply_matrices(int* a, int* b, int* result, int a_rows, int a_cols, int b_cols) {
    for (int i = 0; i < a_rows; i++) {
        for (int j = 0; j < b_cols; j++) {
            *(result + i * b_cols + j) = 0;
            for (int k = 0; k < a_cols; k++) {
                int a_val = *(a + i * a_cols + k);
                int b_val = *(b + k * b_cols + j);
                if ((a_val > 0 && b_val > 0 && a_val > INT_MAX / b_val) ||
                    (a_val > 0 && b_val < 0 && b_val < INT_MIN / a_val) ||
                    (a_val < 0 && b_val > 0 && a_val < INT_MIN / b_val) ||
                    (a_val < 0 && b_val < 0 && a_val < INT_MAX / b_val)) {
                    printf("Integer overflow detected in multiplication.\n");
                    *(result + i * b_cols + j) = 0;
                    break;
                }
                int product = a_val * b_val;
                if ((product > 0 && *(result + i * b_cols + j) > INT_MAX - product) ||
                    (product < 0 && *(result + i * b_cols + j) < INT_MIN - product)) {
                    printf("Integer overflow detected in addition.\n");
                    *(result + i * b_cols + j) = 0;
                    break;
                }
                *(result + i * b_cols + j) += product;
            }
        }
    }
}

int main() {
    int a_rows, a_cols, b_rows, b_cols;
    
    printf("Matrix Multiplication using Pointer Arithmetic\n");
    printf("==============================================\n");
    
    a_rows = read_int("Enter rows for matrix A (1-100): ", 1, MAX_DIM);
    a_cols = read_int("Enter columns for matrix A (1-100): ", 1, MAX_DIM);
    b_rows = read_int("Enter rows for matrix B (1-100): ", 1, MAX_DIM);
    b_cols = read_int("Enter columns for matrix B (1-100): ", 1, MAX_DIM);
    
    if (a_cols != b_rows) {
        printf("Error: Matrix A columns (%d) must equal Matrix B rows (%d) for multiplication.\n", a_cols, b_rows);
        return 1;
    }
    
    size_t a_size = (size_t)a_rows * a_cols;
    size_t b_size = (size_t)b_rows * b_cols;
    size_t result_size = (size_t)a_rows * b_cols;
    
    if (a_size > SIZE_MAX / sizeof(int) || b_size > SIZE_MAX / sizeof(int) || result_size > SIZE_MAX / sizeof(int)) {
        printf("Error: Matrix dimensions too large.\n");
        return 1;
    }
    
    int* matrix_a = malloc(a_size * sizeof(int));
    int* matrix_b = malloc(b_size * sizeof(int));
    int* result = malloc(result_size * sizeof(int));
    
    if (matrix_a == NULL || matrix_b == NULL || result == NULL) {
        printf("Error: Memory allocation failed.\n");
        free(matrix_a);
        free(matrix_b);
        free(result);
        return 1;
    }
    
    printf("\nEnter elements for matrix A (%d x %d):\n", a_rows, a_cols);
    fill_matrix(matrix_a, a_rows, a_cols);
    
    printf("\nEnter elements for matrix B (%d x %d):\n", b_rows, b_cols);
    fill_matrix(matrix_b, b_rows, b_cols);
    
    printf("\nMatrix A:\n");
    print_matrix(matrix_a, a_rows, a_cols);
    
    printf("\nMatrix