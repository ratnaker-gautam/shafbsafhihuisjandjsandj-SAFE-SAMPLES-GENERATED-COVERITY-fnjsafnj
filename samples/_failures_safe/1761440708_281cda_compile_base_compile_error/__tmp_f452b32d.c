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
        value = strtol(buffer, &endptr, 10);
        
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (value < min || value > max) {
            printf("Value must be between %d and %d\n", min, max);
            continue;
        }
        
        break;
    }
    
    return value;
}

void fill_matrix(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
    int counter = 1;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = counter++;
        }
    }
}

void transpose_matrix(int rows, int cols, int src[MAX_SIZE][MAX_SIZE], int dst[MAX_SIZE][MAX_SIZE]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            dst[j][i] = src[i][j];
        }
    }
}

void multiply_matrices(int rows1, int cols1, int mat1[MAX_SIZE][MAX_SIZE],
                      int rows2, int cols2, int mat2[MAX_SIZE][MAX_SIZE],
                      int result[MAX_SIZE][MAX_SIZE]) {
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            result[i][j] = 0;
            for (int k = 0; k < cols1; k++) {
                long long product = (long long)mat1[i][k] * mat2[k][j];
                if (product > INT_MAX || product < INT_MIN) {
                    printf("Integer overflow detected in multiplication\n");
                    exit(1);
                }
                long long sum = (long long)result[i][j] + product;
                if (sum > INT_MAX || sum < INT_MIN) {
                    printf("Integer overflow detected in addition\n");
                    exit(1);
                }
                result[i][j] = (int)sum;
            }
        }
    }
}

int main(void) {
    int rows1, cols1, rows2, cols2;
    int matrix1[MAX_SIZE][MAX_SIZE];
    int matrix2[MAX_SIZE][MAX_SIZE];
    int result[MAX_SIZE][MAX_SIZE];
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    rows1 = read_int("Enter rows for matrix 1 (1-8): ", 1, MAX_SIZE);
    cols1 = read_int("Enter columns for matrix 1 (1-8): ", 1, MAX_SIZE);
    
    fill_matrix(rows1, cols1, matrix1);
    
    printf("\nMatrix 1:\n");
    print_matrix(rows1, cols1, matrix1);
    
    printf("\nTranspose of Matrix 1:\n");
    int transpose[MAX_SIZE][MAX_SIZE];
    transpose_matrix(rows1, cols1, matrix1, transpose);
    print_matrix(cols1, rows1, transpose);
    
    rows2 = read_int("\nEnter rows for matrix 2 (1-8): ", 1, MAX_SIZE);
    cols2 = read_int("Enter columns for matrix 2 (1-8): ", 1, MAX_SIZE);
    
    if (cols1 != rows2) {
        printf("Matrix multiplication not possible: columns of matrix 1 (%d) must equal rows of matrix 2 (%d)\n", cols1, rows2);
        return 1;
    }
    
    fill_matrix(rows2, cols2, matrix2);
    
    printf("\nMatrix 2:\n");
    print_matrix(rows2, cols2, matrix2);
    
    printf("\nMatrix 1 * Matrix 2:\n");
    multiply_matrices(rows1, cols1, matrix1, rows2, cols2, matrix2, result);
    print_matrix(rows1, cols2, result);
    
    printf("\nElement-wise operations:\n");
    int sum[MAX_SIZE][MAX_SIZE];
    int diff[MAX_SIZE][MAX_SIZE];
    
    if (rows1 == rows2 && cols1 == cols2) {
        printf("Matrix addition:\n");
        for (int i = 0; i < rows1; i++) {
            int j = 0;
            while (j < cols1) {
                long long add_result = (long long)matrix1[i][j] + matrix2[i][j];
                if (add_result > INT_MAX || add_result < INT_MIN) {
                    printf("Integer overflow detected in addition\n");
                    exit(1);
                }
                sum[i][j] = (