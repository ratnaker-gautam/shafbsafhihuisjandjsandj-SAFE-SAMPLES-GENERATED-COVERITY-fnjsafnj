//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 10

int read_int(const char* prompt, int min, int max) {
    char buffer[32];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min;
        }
        
        char* endptr;
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

void matrix_multiply(int* a, int* b, int* result, int rows_a, int cols_a, int cols_b) {
    for (int i = 0; i < rows_a; i++) {
        for (int j = 0; j < cols_b; j++) {
            *(result + i * cols_b + j) = 0;
            for (int k = 0; k < cols_a; k++) {
                int val_a = *(a + i * cols_a + k);
                int val_b = *(b + k * cols_b + j);
                if ((val_a > 0 && val_b > INT_MAX / val_a) || 
                    (val_a < 0 && val_b < INT_MAX / val_a)) {
                    *(result + i * cols_b + j) = 0;
                    break;
                }
                *(result + i * cols_b + j) += val_a * val_b;
            }
        }
    }
}

void print_matrix(int* matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d", *(matrix + i * cols + j));
        }
        printf("\n");
    }
}

int main() {
    int rows_a, cols_a, rows_b, cols_b;
    
    printf("Matrix Multiplication using Pointer Arithmetic\n");
    printf("Matrix dimensions must be between 1 and %d\n", MAX_SIZE);
    
    rows_a = read_int("Enter rows for matrix A: ", 1, MAX_SIZE);
    cols_a = read_int("Enter columns for matrix A: ", 1, MAX_SIZE);
    rows_b = read_int("Enter rows for matrix B: ", 1, MAX_SIZE);
    cols_b = read_int("Enter columns for matrix B: ", 1, MAX_SIZE);
    
    if (cols_a != rows_b) {
        printf("Error: Columns of A (%d) must equal rows of B (%d) for multiplication.\n", cols_a, rows_b);
        return 1;
    }
    
    int matrix_a[MAX_SIZE * MAX_SIZE];
    int matrix_b[MAX_SIZE * MAX_SIZE];
    int result[MAX_SIZE * MAX_SIZE];
    
    printf("\nEnter elements for matrix A (%d x %d):\n", rows_a, cols_a);
    for (int i = 0; i < rows_a; i++) {
        for (int j = 0; j < cols_a; j++) {
            printf("A[%d][%d]: ", i, j);
            *(matrix_a + i * cols_a + j) = read_int("", INT_MIN, INT_MAX);
        }
    }
    
    printf("\nEnter elements for matrix B (%d x %d):\n", rows_b, cols_b);
    for (int i = 0; i < rows_b; i++) {
        for (int j = 0; j < cols_b; j++) {
            printf("B[%d][%d]: ", i, j);
            *(matrix_b + i * cols_b + j) = read_int("", INT_MIN, INT_MAX);
        }
    }
    
    matrix_multiply(matrix_a, matrix_b, result, rows_a, cols_a, cols_b);
    
    printf("\nMatrix A:\n");
    print_matrix(matrix_a, rows_a, cols_a);
    
    printf("\nMatrix B:\n");
    print_matrix(matrix_b, rows_b, cols_b);
    
    printf("\nResult (A x B):\n");
    print_matrix(result, rows_a, cols_b);
    
    return 0;
}