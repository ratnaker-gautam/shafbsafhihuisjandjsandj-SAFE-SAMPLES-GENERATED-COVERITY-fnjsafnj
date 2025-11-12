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
    int k = 1;
    for (int i = 0; i < rows; i++) {
        int j = 0;
        while (j < cols) {
            matrix[i][j] = k++;
            j++;
        }
    }
}

void transpose_matrix(int rows, int cols, int src[MAX_SIZE][MAX_SIZE], int dst[MAX_SIZE][MAX_SIZE]) {
    for (int i = 0; i < rows; i++) {
        int j = 0;
        do {
            dst[j][i] = src[i][j];
            j++;
        } while (j < cols);
    }
}

void multiply_matrices(int rows1, int cols1, int mat1[MAX_SIZE][MAX_SIZE],
                      int rows2, int cols2, int mat2[MAX_SIZE][MAX_SIZE],
                      int result[MAX_SIZE][MAX_SIZE]) {
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            int sum = 0;
            int k = 0;
            while (k < cols1) {
                if (mat1[i][k] > 0 && mat2[k][j] > 0) {
                    if (mat1[i][k] > INT_MAX / mat2[k][j]) {
                        printf("Integer overflow detected\n");
                        return;
                    }
                }
                sum += mat1[i][k] * mat2[k][j];
                k++;
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
    
    rows1 = read_int("Enter rows for matrix 1 (1-8): ", 1, MAX_SIZE);
    cols1 = read_int("Enter columns for matrix 1 (1-8): ", 1, MAX_SIZE);
    
    fill_matrix(rows1, cols1, matrix1);
    
    printf("\nMatrix 1:\n");
    print_matrix(rows1, cols1, matrix1);
    
    int transposed[MAX_SIZE][MAX_SIZE];
    transpose_matrix(rows1, cols1, matrix1, transposed);
    
    printf("\nTransposed Matrix 1:\n");
    print_matrix(cols1, rows1, transposed);
    
    rows2 = read_int("\nEnter rows for matrix 2 (1-8): ", 1, MAX_SIZE);
    cols2 = read_int("Enter columns for matrix 2 (1-8): ", 1, MAX_SIZE);
    
    if (cols1 != rows2) {
        printf("Matrix multiplication not possible: columns of matrix 1 must equal rows of matrix 2\n");
        return 1;
    }
    
    fill_matrix(rows2, cols2, matrix2);
    
    printf("\nMatrix 2:\n");
    print_matrix(rows2, cols2, matrix2);
    
    memset(result, 0, sizeof(result));
    multiply_matrices(rows1, cols1, matrix1, rows2, cols2, matrix2, result);
    
    printf("\nMatrix 1 * Matrix 2:\n");
    print_matrix(rows1, cols2, result);
    
    return 0;
}