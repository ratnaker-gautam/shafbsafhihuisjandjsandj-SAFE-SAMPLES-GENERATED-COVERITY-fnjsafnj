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
    char buffer[32];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        char *endptr;
        value = strtol(buffer, &endptr, 10);
        
        if (endptr == buffer || *endptr != '\n') {
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
    printf("=========================\n");
    
    rows1 = read_int("Enter rows for matrix 1 (1-10): ", 1, MAX_SIZE);
    if (rows1 == -1) return 1;
    
    cols1 = read_int("Enter columns for matrix 1 (1-10): ", 1, MAX_SIZE);
    if (cols1 == -1) return 1;
    
    int matrix1[MAX_SIZE * MAX_SIZE];
    printf("Enter elements for matrix 1 (%dx%d):\n", rows1, cols1);
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols1; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            int val = read_int(prompt, -1000, 1000);
            if (val == -1) return 1;
            *(matrix1 + i * cols1 + j) = val;
        }
    }
    
    printf("\nMatrix 1:\n");
    print_matrix(matrix1, rows1, cols1);
    
    printf("\nTranspose of Matrix 1:\n");
    int transpose[MAX_SIZE * MAX_SIZE];
    transpose_matrix(matrix1, transpose, rows1, cols1);
    print_matrix(transpose, cols1, rows1);
    
    printf("\nEnter dimensions for matrix 2 for multiplication:\n");
    rows2 = read_int("Enter rows for matrix 2 (1-10): ", 1, MAX_SIZE);
    if (rows2 == -1) return 1;
    
    cols2 = read_int("Enter columns for matrix 2 (1-10): ", 1, MAX_SIZE);
    if (cols2 == -1) return 1;
    
    if (cols1 != rows2) {
        printf("Error: Matrix 1 columns (%d) must equal matrix 2 rows (%d) for multiplication.\n", cols1, rows2);
        return 1;
    }
    
    int matrix2[MAX_SIZE * MAX_SIZE];
    printf("Enter elements for matrix 2 (%dx%d):\n", rows2, cols2);
    for (int i = 0; i < rows2; i++) {
        for (int j = 0; j < cols2; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            int val = read_int(prompt, -1000, 1000);
            if (val == -1) return 1;
            *(matrix2 + i * cols2 + j) = val;
        }
    }
    
    printf("\nMatrix 2:\n");
    print_matrix(matrix2, rows2, cols2);
    
    printf("\nMatrix 1 * Matrix 2:\n");
    int product[MAX_SIZE * MAX_SIZE];
    multiply_matrices(matrix1, matrix2, product, rows1, cols1, cols2);
    print_matrix(product, rows1, cols2);
    
    return 0;
}