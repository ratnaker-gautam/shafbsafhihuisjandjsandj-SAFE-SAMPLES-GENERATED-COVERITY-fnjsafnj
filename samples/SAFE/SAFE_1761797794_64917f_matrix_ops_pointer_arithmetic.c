//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 10

int validate_int_input(const char* prompt, int min, int max) {
    char buffer[32];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
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

void matrix_multiply(int* mat1, int* mat2, int* result, int rows1, int cols1, int cols2) {
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            *(result + i * cols2 + j) = 0;
            for (int k = 0; k < cols1; k++) {
                *(result + i * cols2 + j) += *(mat1 + i * cols1 + k) * *(mat2 + k * cols2 + j);
            }
        }
    }
}

void print_matrix(int* matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%4d", *(matrix + i * cols + j));
        }
        printf("\n");
    }
}

void fill_matrix(int* matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[64];
            snprintf(prompt, sizeof(prompt), "Enter element [%d][%d]: ", i, j);
            *(matrix + i * cols + j) = validate_int_input(prompt, -1000, 1000);
        }
    }
}

int main() {
    int rows1, cols1, rows2, cols2;
    
    printf("Matrix Multiplication using Pointer Arithmetic\n");
    printf("Matrix 1 dimensions:\n");
    rows1 = validate_int_input("Rows (1-10): ", 1, MAX_SIZE);
    cols1 = validate_int_input("Columns (1-10): ", 1, MAX_SIZE);
    
    printf("Matrix 2 dimensions:\n");
    rows2 = validate_int_input("Rows (1-10): ", 1, MAX_SIZE);
    cols2 = validate_int_input("Columns (1-10): ", 1, MAX_SIZE);
    
    if (cols1 != rows2) {
        printf("Error: Matrix 1 columns must equal Matrix 2 rows for multiplication.\n");
        return 1;
    }
    
    int matrix1[MAX_SIZE * MAX_SIZE];
    int matrix2[MAX_SIZE * MAX_SIZE];
    int result[MAX_SIZE * MAX_SIZE];
    
    printf("\nEnter elements for Matrix 1 (%dx%d):\n", rows1, cols1);
    fill_matrix(matrix1, rows1, cols1);
    
    printf("\nEnter elements for Matrix 2 (%dx%d):\n", rows2, cols2);
    fill_matrix(matrix2, rows2, cols2);
    
    matrix_multiply(matrix1, matrix2, result, rows1, cols1, cols2);
    
    printf("\nMatrix 1:\n");
    print_matrix(matrix1, rows1, cols1);
    
    printf("\nMatrix 2:\n");
    print_matrix(matrix2, rows2, cols2);
    
    printf("\nResult (%dx%d):\n", rows1, cols2);
    print_matrix(result, rows1, cols2);
    
    return 0;
}