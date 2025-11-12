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

void fill_matrix(int* matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Enter element [%d][%d]: ", i, j);
            *(matrix + i * cols + j) = read_int("", INT_MIN, INT_MAX);
        }
    }
}

void print_matrix(int* matrix, int rows, int cols) {
    printf("Matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d ", *(matrix + i * cols + j));
        }
        printf("\n");
    }
}

void transpose_matrix(int* src, int* dest, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            *(dest + j * rows + i) = *(src + i * cols + j);
        }
    }
}

void multiply_matrices(int* a, int* b, int* result, int a_rows, int a_cols, int b_cols) {
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
    
    rows1 = read_int("Enter rows for matrix 1 (1-10): ", 1, MAX_SIZE);
    cols1 = read_int("Enter columns for matrix 1 (1-10): ", 1, MAX_SIZE);
    
    int matrix1[MAX_SIZE * MAX_SIZE];
    fill_matrix(matrix1, rows1, cols1);
    print_matrix(matrix1, rows1, cols1);
    
    printf("\n");
    rows2 = read_int("Enter rows for matrix 2 (1-10): ", 1, MAX_SIZE);
    cols2 = read_int("Enter columns for matrix 2 (1-10): ", 1, MAX_SIZE);
    
    int matrix2[MAX_SIZE * MAX_SIZE];
    fill_matrix(matrix2, rows2, cols2);
    print_matrix(matrix2, rows2, cols2);
    
    printf("\nMatrix 1 Transpose:\n");
    int transpose1[MAX_SIZE * MAX_SIZE];
    transpose_matrix(matrix1, transpose1, rows1, cols1);
    print_matrix(transpose1, cols1, rows1);
    
    printf("\nMatrix 2 Transpose:\n");
    int transpose2[MAX_SIZE * MAX_SIZE];
    transpose_matrix(matrix2, transpose2, rows2, cols2);
    print_matrix(transpose2, cols2, rows2);
    
    if (cols1 == rows2) {
        printf("\nMatrix Multiplication (Matrix1 * Matrix2):\n");
        int product[MAX_SIZE * MAX_SIZE];
        multiply_matrices(matrix1, matrix2, product, rows1, cols1, cols2);
        print_matrix(product, rows1, cols2);
    } else {
        printf("\nCannot multiply matrices: columns of matrix1 (%d) != rows of matrix2 (%d)\n", cols1, rows2);
    }
    
    if (rows1 == rows2 && cols1 == cols2) {
        printf("\nMatrix Addition:\n");
        int sum[MAX_SIZE * MAX_SIZE];
        for (int i = 0; i < rows1; i++) {
            for (int j = 0; j < cols1; j++) {
                *(sum + i * cols1 + j) = *(matrix1 + i * cols1 + j) + *(matrix2 + i * cols1 + j);
            }
        }
        print_matrix(sum, rows1, cols1);
    } else {
        printf("\nCannot add matrices: dimensions don't match (%dx%d vs %dx%d)\n", rows1, cols1, rows2, cols2);
    }
    
    return 0;
}