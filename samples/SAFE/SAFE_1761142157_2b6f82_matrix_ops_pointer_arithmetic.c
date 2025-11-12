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
        
        if (value < min || value > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        return value;
    }
}

void read_matrix(int *matrix, int rows, int cols) {
    printf("Enter matrix elements:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            *(matrix + i * cols + j) = read_int(prompt, -1000, 1000);
        }
    }
}

void transpose_matrix(int *src, int *dest, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            *(dest + j * rows + i) = *(src + i * cols + j);
        }
    }
}

void multiply_matrices(int *a, int *b, int *result, int a_rows, int a_cols, int b_cols) {
    for (int i = 0; i < a_rows; i++) {
        for (int j = 0; j < b_cols; j++) {
            int sum = 0;
            for (int k = 0; k < a_cols; k++) {
                sum += *(a + i * a_cols + k) * *(b + k * b_cols + j);
            }
            *(result + i * b_cols + j) = sum;
        }
    }
}

int main() {
    int choice;
    int matrix_a[MAX_SIZE * MAX_SIZE];
    int matrix_b[MAX_SIZE * MAX_SIZE];
    int result[MAX_SIZE * MAX_SIZE];
    
    printf("Matrix Operations Program\n");
    printf("1. Transpose matrix\n");
    printf("2. Multiply matrices\n");
    
    choice = read_int("Enter choice (1-2): ", 1, 2);
    
    if (choice == 1) {
        int rows = read_int("Enter number of rows (1-10): ", 1, MAX_SIZE);
        int cols = read_int("Enter number of columns (1-10): ", 1, MAX_SIZE);
        
        read_matrix(matrix_a, rows, cols);
        
        printf("\nOriginal matrix:\n");
        print_matrix(matrix_a, rows, cols);
        
        transpose_matrix(matrix_a, result, rows, cols);
        
        printf("\nTransposed matrix:\n");
        print_matrix(result, cols, rows);
        
    } else {
        int a_rows = read_int("Enter rows for matrix A (1-10): ", 1, MAX_SIZE);
        int a_cols = read_int("Enter columns for matrix A (1-10): ", 1, MAX_SIZE);
        int b_rows = a_cols;
        int b_cols = read_int("Enter columns for matrix B (1-10): ", 1, MAX_SIZE);
        
        printf("Matrix A:\n");
        read_matrix(matrix_a, a_rows, a_cols);
        
        printf("Matrix B:\n");
        read_matrix(matrix_b, b_rows, b_cols);
        
        printf("\nMatrix A:\n");
        print_matrix(matrix_a, a_rows, a_cols);
        
        printf("\nMatrix B:\n");
        print_matrix(matrix_b, b_rows, b_cols);
        
        multiply_matrices(matrix_a, matrix_b, result, a_rows, a_cols, b_cols);
        
        printf("\nResult of multiplication:\n");
        print_matrix(result, a_rows, b_cols);
    }
    
    return 0;
}