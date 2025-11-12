//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 10

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
            printf("Input error\n");
            exit(1);
        }
        
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (value >= min && value <= max) {
            return value;
        }
        printf("Value must be between %d and %d\n", min, max);
    }
}

void matrix_multiply(int *a, int *b, int *result, int a_rows, int a_cols, int b_cols) {
    for (int i = 0; i < a_rows; i++) {
        for (int j = 0; j < b_cols; j++) {
            *(result + i * b_cols + j) = 0;
            for (int k = 0; k < a_cols; k++) {
                *(result + i * b_cols + j) += *(a + i * a_cols + k) * *(b + k * b_cols + j);
            }
        }
    }
}

int main() {
    int matrix_a[MAX_SIZE][MAX_SIZE];
    int matrix_b[MAX_SIZE][MAX_SIZE];
    int result[MAX_SIZE][MAX_SIZE];
    
    printf("Matrix Multiplication Calculator\n");
    printf("Maximum matrix size: %dx%d\n\n", MAX_SIZE, MAX_SIZE);
    
    int rows_a = read_int("Enter rows for matrix A (1-10): ", 1, MAX_SIZE);
    int cols_a = read_int("Enter columns for matrix A (1-10): ", 1, MAX_SIZE);
    int rows_b = read_int("Enter rows for matrix B (1-10): ", 1, MAX_SIZE);
    int cols_b = read_int("Enter columns for matrix B (1-10): ", 1, MAX_SIZE);
    
    if (cols_a != rows_b) {
        printf("Error: Matrix A columns (%d) must equal Matrix B rows (%d) for multiplication\n", cols_a, rows_b);
        return 1;
    }
    
    printf("\nEnter elements for matrix A (%dx%d):\n", rows_a, cols_a);
    for (int i = 0; i < rows_a; i++) {
        for (int j = 0; j < cols_a; j++) {
            printf("A[%d][%d]: ", i, j);
            char buffer[100];
            if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                printf("Input error\n");
                return 1;
            }
            if (sscanf(buffer, "%d", &matrix_a[i][j]) != 1) {
                printf("Invalid input. Using 0.\n");
                matrix_a[i][j] = 0;
            }
        }
    }
    
    printf("\nEnter elements for matrix B (%dx%d):\n", rows_b, cols_b);
    for (int i = 0; i < rows_b; i++) {
        for (int j = 0; j < cols_b; j++) {
            printf("B[%d][%d]: ", i, j);
            char buffer[100];
            if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                printf("Input error\n");
                return 1;
            }
            if (sscanf(buffer, "%d", &matrix_b[i][j]) != 1) {
                printf("Invalid input. Using 0.\n");
                matrix_b[i][j] = 0;
            }
        }
    }
    
    printf("\nMatrix A:\n");
    print_matrix((int *)matrix_a, rows_a, cols_a);
    
    printf("\nMatrix B:\n");
    print_matrix((int *)matrix_b, rows_b, cols_b);
    
    matrix_multiply((int *)matrix_a, (int *)matrix_b, (int *)result, rows_a, cols_a, cols_b);
    
    printf("\nResult (A x B):\n");
    print_matrix((int *)result, rows_a, cols_b);
    
    return 0;
}