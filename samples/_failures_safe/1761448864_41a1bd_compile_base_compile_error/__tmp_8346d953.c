//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 8

void print_matrix(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%4d", matrix[i][j]);
        }
        printf("\n");
    }
}

int read_int(const char* prompt, int min, int max) {
    char buffer[32];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Input error\n");
            exit(1);
        }
        
        char* endptr;
        long long_val = strtol(buffer, &endptr, 10);
        
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (long_val < min || long_val > max) {
            printf("Value must be between %d and %d\n", min, max);
            continue;
        }
        
        value = (int)long_val;
        break;
    }
    
    return value;
}

void fill_matrix(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
    int k = 1;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = k++;
        }
    }
}

void transpose_matrix(int rows, int cols, int src[MAX_SIZE][MAX_SIZE], int dest[MAX_SIZE][MAX_SIZE]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            dest[j][i] = src[i][j];
        }
    }
}

void multiply_matrices(int rows1, int cols1, int mat1[MAX_SIZE][MAX_SIZE],
                      int rows2, int cols2, int mat2[MAX_SIZE][MAX_SIZE],
                      int result[MAX_SIZE][MAX_SIZE]) {
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            int sum = 0;
            for (int k = 0; k < cols1; k++) {
                if (mat1[i][k] > 0 && mat2[k][j] > 0) {
                    if (mat1[i][k] > INT_MAX / mat2[k][j]) {
                        printf("Multiplication overflow detected\n");
                        return;
                    }
                }
                sum += mat1[i][k] * mat2[k][j];
            }
            result[i][j] = sum;
        }
    }
}

int main() {
    int rows1, cols1, rows2, cols2;
    int matrix1[MAX_SIZE][MAX_SIZE];
    int matrix2[MAX_SIZE][MAX_SIZE];
    int result[MAX_SIZE][MAX_SIZE];
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    rows1 = read_int("Enter rows for matrix 1 (1-8): ", 1, MAX_SIZE);
    cols1 = read_int("Enter columns for matrix 1 (1-8): ", 1, MAX_SIZE);
    
    rows2 = read_int("Enter rows for matrix 2 (1-8): ", 1, MAX_SIZE);
    cols2 = read_int("Enter columns for matrix 2 (1-8): ", 1, MAX_SIZE);
    
    fill_matrix(rows1, cols1, matrix1);
    fill_matrix(rows2, cols2, matrix2);
    
    printf("\nMatrix 1:\n");
    print_matrix(rows1, cols1, matrix1);
    
    printf("\nMatrix 2:\n");
    print_matrix(rows2, cols2, matrix2);
    
    if (cols1 == rows2) {
        printf("\nMatrix Multiplication Result:\n");
        memset(result, 0, sizeof(result));
        multiply_matrices(rows1, cols1, matrix1, rows2, cols2, matrix2, result);
        print_matrix(rows1, cols2, result);
    } else {
        printf("\nMatrices cannot be multiplied (columns of matrix 1 != rows of matrix 2)\n");
    }
    
    if (rows1 == cols1) {
        printf("\nTranspose of Matrix 1:\n");
        int transposed[MAX_SIZE][MAX_SIZE];
        transpose_matrix(rows1, cols1, matrix1, transposed);
        print_matrix(cols1, rows1, transposed);
    }
    
    if (rows2 == cols2) {
        printf("\nTranspose of Matrix 2:\n");
        int transposed[MAX_SIZE][MAX_SIZE];
        transpose_matrix(rows2, cols2, matrix2, transposed);
        print_matrix(cols2, rows2, transposed);
    }
    
    printf("\nElement-wise operations:\n");
    if (rows1 == rows2 && cols1 == cols2) {
        printf("Matrix addition:\n");
        for (int i = 0; i < rows1; i++) {
            for (int j = 0; j < cols1; j++) {
                if ((matrix1[i][j] > 0 && matrix2[i][j] > INT_MAX - matrix1[i][j]) ||