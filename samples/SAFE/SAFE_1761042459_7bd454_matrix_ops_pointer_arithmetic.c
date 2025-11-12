//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 10

void matrix_multiply(int *a, int *b, int *result, int rows_a, int cols_a, int cols_b) {
    for (int i = 0; i < rows_a; i++) {
        for (int j = 0; j < cols_b; j++) {
            *(result + i * cols_b + j) = 0;
            for (int k = 0; k < cols_a; k++) {
                int val_a = *(a + i * cols_a + k);
                int val_b = *(b + k * cols_b + j);
                if (val_a > 0 && val_b > 0 && val_a > INT_MAX / val_b) {
                    *(result + i * cols_b + j) = 0;
                    break;
                }
                *(result + i * cols_b + j) += val_a * val_b;
            }
        }
    }
}

void print_matrix(int *matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", *(matrix + i * cols + j));
        }
        printf("\n");
    }
}

int read_int(const char *prompt, int min, int max) {
    char buffer[32];
    int value;
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min;
        }
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter an integer.\n");
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
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[64];
            snprintf(prompt, sizeof(prompt), "Enter element [%d][%d]: ", i, j);
            *(matrix + i * cols + j) = read_int(prompt, -1000, 1000);
        }
    }
}

int main() {
    int rows_a, cols_a, rows_b, cols_b;
    
    printf("Matrix A dimensions:\n");
    rows_a = read_int("Rows: ", 1, MAX_SIZE);
    cols_a = read_int("Columns: ", 1, MAX_SIZE);
    
    printf("Matrix B dimensions:\n");
    rows_b = read_int("Rows: ", 1, MAX_SIZE);
    cols_b = read_int("Columns: ", 1, MAX_SIZE);
    
    if (cols_a != rows_b) {
        printf("Matrix multiplication not possible: columns of A must equal rows of B.\n");
        return 1;
    }
    
    int matrix_a[MAX_SIZE * MAX_SIZE];
    int matrix_b[MAX_SIZE * MAX_SIZE];
    int result[MAX_SIZE * MAX_SIZE];
    
    printf("Enter elements for Matrix A:\n");
    read_matrix(matrix_a, rows_a, cols_a);
    
    printf("Enter elements for Matrix B:\n");
    read_matrix(matrix_b, rows_b, cols_b);
    
    matrix_multiply(matrix_a, matrix_b, result, rows_a, cols_a, cols_b);
    
    printf("Matrix A:\n");
    print_matrix(matrix_a, rows_a, cols_a);
    
    printf("Matrix B:\n");
    print_matrix(matrix_b, rows_b, cols_b);
    
    printf("Result (A * B):\n");
    print_matrix(result, rows_a, cols_b);
    
    return 0;
}