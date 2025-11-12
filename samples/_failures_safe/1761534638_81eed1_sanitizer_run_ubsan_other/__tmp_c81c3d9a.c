//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

void fill_matrix(int* matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Enter element [%d][%d]: ", i, j);
            *(matrix + i * cols + j) = read_int("", INT_MIN, INT_MAX);
        }
    }
}

void print_matrix(int* matrix, int rows, int cols) {
    printf("Matrix (%dx%d):\n", rows, cols);
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

int multiply_matrices(int* a, int a_rows, int a_cols, int* b, int b_rows, int b_cols, int* result) {
    if (a_cols != b_rows) {
        return 0;
    }
    
    for (int i = 0; i < a_rows; i++) {
        for (int j = 0; j < b_cols; j++) {
            int sum = 0;
            for (int k = 0; k < a_cols; k++) {
                sum += *(a + i * a_cols + k) * *(b + k * b_cols + j);
            }
            *(result + i * b_cols + j) = sum;
        }
    }
    return 1;
}

int main() {
    int rows1, cols1, rows2, cols2;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n\n");
    
    rows1 = read_int("Enter rows for matrix 1 (1-100): ", 1, MAX_DIM);
    cols1 = read_int("Enter columns for matrix 1 (1-100): ", 1, MAX_DIM);
    
    int matrix1[MAX_DIM * MAX_DIM];
    printf("\nFilling matrix 1:\n");
    fill_matrix(matrix1, rows1, cols1);
    
    printf("\n");
    print_matrix(matrix1, rows1, cols1);
    
    printf("\nMatrix transpose:\n");
    int transposed[MAX_DIM * MAX_DIM];
    transpose_matrix(matrix1, transposed, rows1, cols1);
    print_matrix(transposed, cols1, rows1);
    
    printf("\nMatrix multiplication:\n");
    rows2 = read_int("Enter rows for matrix 2 (1-100): ", 1, MAX_DIM);
    cols2 = read_int("Enter columns for matrix 2 (1-100): ", 1, MAX_DIM);
    
    if (cols1 != rows2) {
        printf("Cannot multiply: columns of matrix 1 (%d) must equal rows of matrix 2 (%d)\n", cols1, rows2);
        return 1;
    }
    
    int matrix2[MAX_DIM * MAX_DIM];
    printf("\nFilling matrix 2:\n");
    fill_matrix(matrix2, rows2, cols2);
    
    printf("\nMatrix 2:\n");
    print_matrix(matrix2, rows2, cols2);
    
    int product[MAX_DIM * MAX_DIM];
    if (multiply_matrices(matrix1, rows1, cols1, matrix2, rows2, cols2, product)) {
        printf("\nMatrix product:\n");
        print_matrix(product, rows1, cols2);
    } else {
        printf("Matrix multiplication failed.\n");
    }
    
    return 0;
}