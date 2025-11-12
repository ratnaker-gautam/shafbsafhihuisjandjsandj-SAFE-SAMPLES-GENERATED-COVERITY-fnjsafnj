//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 10

void matrix_multiply(int *a, int *b, int *result, int rows_a, int cols_a, int cols_b) {
    for (int i = 0; i < rows_a; i++) {
        for (int j = 0; j < cols_b; j++) {
            *(result + i * cols_b + j) = 0;
            for (int k = 0; k < cols_a; k++) {
                *(result + i * cols_b + j) += *(a + i * cols_a + k) * *(b + k * cols_b + j);
            }
        }
    }
}

void print_matrix(int *matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%4d", *(matrix + i * cols + j));
        }
        printf("\n");
    }
}

int read_int(const char *prompt, int min, int max) {
    char buffer[100];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
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

int main() {
    int rows_a, cols_a, rows_b, cols_b;
    
    printf("Matrix Multiplication Calculator\n");
    printf("Matrix dimensions must be between 1 and %d\n", MAX_SIZE);
    
    rows_a = read_int("Enter rows for matrix A: ", 1, MAX_SIZE);
    cols_a = read_int("Enter columns for matrix A: ", 1, MAX_SIZE);
    rows_b = read_int("Enter rows for matrix B: ", 1, MAX_SIZE);
    cols_b = read_int("Enter columns for matrix B: ", 1, MAX_SIZE);
    
    if (rows_a == -1 || cols_a == -1 || rows_b == -1 || cols_b == -1) {
        printf("Input error.\n");
        return 1;
    }
    
    if (cols_a != rows_b) {
        printf("Error: Columns of A (%d) must equal rows of B (%d) for multiplication.\n", cols_a, rows_b);
        return 1;
    }
    
    int matrix_a[MAX_SIZE][MAX_SIZE];
    int matrix_b[MAX_SIZE][MAX_SIZE];
    int result[MAX_SIZE][MAX_SIZE];
    
    printf("\nEnter values for matrix A (%dx%d):\n", rows_a, cols_a);
    for (int i = 0; i < rows_a; i++) {
        for (int j = 0; j < cols_a; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "A[%d][%d]: ", i, j);
            int value = read_int(prompt, -1000, 1000);
            if (value == -1) {
                printf("Input error.\n");
                return 1;
            }
            *(*(matrix_a + i) + j) = value;
        }
    }
    
    printf("\nEnter values for matrix B (%dx%d):\n", rows_b, cols_b);
    for (int i = 0; i < rows_b; i++) {
        for (int j = 0; j < cols_b; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "B[%d][%d]: ", i, j);
            int value = read_int(prompt, -1000, 1000);
            if (value == -1) {
                printf("Input error.\n");
                return 1;
            }
            *(*(matrix_b + i) + j) = value;
        }
    }
    
    memset(result, 0, sizeof(result));
    matrix_multiply((int *)matrix_a, (int *)matrix_b, (int *)result, rows_a, cols_a, cols_b);
    
    printf("\nMatrix A:\n");
    print_matrix((int *)matrix_a, rows_a, cols_a);
    
    printf("\nMatrix B:\n");
    print_matrix((int *)matrix_b, rows_b, cols_b);
    
    printf("\nResult (A × B):\n");
    print_matrix((int *)result, rows_a, cols_b);
    
    return 0;
}