//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 8

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
    int rows1, cols1, rows2, cols2;
    
    printf("Matrix Operations Program\n");
    
    rows1 = read_int("Enter rows for matrix 1 (1-8): ", 1, MAX_SIZE);
    cols1 = read_int("Enter columns for matrix 1 (1-8): ", 1, MAX_SIZE);
    
    int matrix1[MAX_SIZE * MAX_SIZE];
    fill_matrix(matrix1, rows1, cols1);
    print_matrix(matrix1, rows1, cols1);
    
    printf("\nMatrix Transpose:\n");
    int transpose[MAX_SIZE * MAX_SIZE];
    transpose_matrix(matrix1, transpose, rows1, cols1);
    print_matrix(transpose, cols1, rows1);
    
    printf("\nMatrix Multiplication:\n");
    rows2 = read_int("Enter rows for matrix 2 (1-8): ", 1, MAX_SIZE);
    cols2 = read_int("Enter columns for matrix 2 (1-8): ", 1, MAX_SIZE);
    
    if (cols1 != rows2) {
        printf("Cannot multiply: columns of matrix 1 (%d) must equal rows of matrix 2 (%d)\n", cols1, rows2);
        return 1;
    }
    
    int matrix2[MAX_SIZE * MAX_SIZE];
    fill_matrix(matrix2, rows2, cols2);
    print_matrix(matrix2, rows2, cols2);
    
    int product[MAX_SIZE * MAX_SIZE];
    multiply_matrices(matrix1, matrix2, product, rows1, cols1, cols2);
    
    printf("\nMatrix Product:\n");
    print_matrix(product, rows1, cols2);
    
    return 0;
}