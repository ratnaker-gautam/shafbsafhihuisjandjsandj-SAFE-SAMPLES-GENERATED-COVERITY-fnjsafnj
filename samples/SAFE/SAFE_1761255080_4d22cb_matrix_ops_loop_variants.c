//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 10

void print_matrix(int matrix[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%4d", matrix[i][j]);
        }
        printf("\n");
    }
}

void transpose_matrix(int src[MAX_SIZE][MAX_SIZE], int dst[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    int i = 0;
    while (i < rows) {
        int j = 0;
        while (j < cols) {
            dst[j][i] = src[i][j];
            j++;
        }
        i++;
    }
}

void multiply_matrices(int a[MAX_SIZE][MAX_SIZE], int b[MAX_SIZE][MAX_SIZE], int result[MAX_SIZE][MAX_SIZE], int m, int n, int p) {
    for (int i = 0; i < m; i++) {
        for (int k = 0; k < p; k++) {
            int sum = 0;
            for (int j = 0; j < n; j++) {
                if (a[i][j] > 0 && b[j][k] > 0) {
                    if (a[i][j] > INT_MAX / b[j][k]) {
                        printf("Integer overflow detected\n");
                        return;
                    }
                } else if (a[i][j] < 0 && b[j][k] < 0) {
                    if (a[i][j] < INT_MAX / b[j][k]) {
                        printf("Integer overflow detected\n");
                        return;
                    }
                }
                sum += a[i][j] * b[j][k];
            }
            result[i][k] = sum;
        }
    }
}

int read_int(const char* prompt, int min, int max) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Input error\n");
            continue;
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

void fill_matrix(int matrix[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    int count = 1;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = count++;
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
    
    rows1 = read_int("Enter rows for matrix 1 (1-10): ", 1, MAX_SIZE);
    cols1 = read_int("Enter columns for matrix 1 (1-10): ", 1, MAX_SIZE);
    
    fill_matrix(matrix1, rows1, cols1);
    printf("\nMatrix 1:\n");
    print_matrix(matrix1, rows1, cols1);
    
    rows2 = read_int("\nEnter rows for matrix 2 (1-10): ", 1, MAX_SIZE);
    cols2 = read_int("Enter columns for matrix 2 (1-10): ", 1, MAX_SIZE);
    
    fill_matrix(matrix2, rows2, cols2);
    printf("\nMatrix 2:\n");
    print_matrix(matrix2, rows2, cols2);
    
    printf("\nTranspose of Matrix 1:\n");
    int transpose[MAX_SIZE][MAX_SIZE];
    transpose_matrix(matrix1, transpose, rows1, cols1);
    print_matrix(transpose, cols1, rows1);
    
    if (cols1 == rows2) {
        printf("\nMatrix Multiplication (Matrix 1 * Matrix 2):\n");
        multiply_matrices(matrix1, matrix2, result, rows1, cols1, cols2);
        print_matrix(result, rows1, cols2);
    } else {
        printf("\nCannot multiply matrices: columns of first (%d) must equal rows of second (%d)\n", cols1, rows2);
    }
    
    return 0;
}