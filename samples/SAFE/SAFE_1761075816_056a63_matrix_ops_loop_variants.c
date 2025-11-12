//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 8

int read_int(const char* prompt, int min, int max) {
    char buffer[32];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min;
        }
        
        char* endptr;
        long lval = strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (lval < min || lval > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        value = (int)lval;
        break;
    }
    
    return value;
}

void fill_matrix(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
    int i = 0;
    while (i < rows) {
        int j = 0;
        do {
            matrix[i][j] = (i * cols + j) % 10;
            j++;
        } while (j < cols);
        i++;
    }
}

void print_matrix(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
    for (int i = 0; i < rows; i++) {
        int j;
        for (j = 0; j < cols - 1; j++) {
            printf("%3d ", matrix[i][j]);
        }
        printf("%3d\n", matrix[i][j]);
    }
}

void transpose_matrix(int rows, int cols, int src[MAX_SIZE][MAX_SIZE], int dst[MAX_SIZE][MAX_SIZE]) {
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

void multiply_matrices(int r1, int c1, int m1[MAX_SIZE][MAX_SIZE], 
                      int r2, int c2, int m2[MAX_SIZE][MAX_SIZE], 
                      int result[MAX_SIZE][MAX_SIZE]) {
    int i = 0;
    do {
        int j = 0;
        while (j < c2) {
            int sum = 0;
            int k;
            for (k = 0; k < c1; k++) {
                sum += m1[i][k] * m2[k][j];
            }
            result[i][j] = sum;
            j++;
        }
        i++;
    } while (i < r1);
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
    
    printf("\nMatrix 1:\n");
    fill_matrix(rows1, cols1, matrix1);
    print_matrix(rows1, cols1, matrix1);
    
    rows2 = read_int("\nEnter rows for matrix 2 (1-8): ", 1, MAX_SIZE);
    cols2 = read_int("Enter columns for matrix 2 (1-8): ", 1, MAX_SIZE);
    
    printf("\nMatrix 2:\n");
    fill_matrix(rows2, cols2, matrix2);
    print_matrix(rows2, cols2, matrix2);
    
    printf("\nTranspose of Matrix 1:\n");
    int transpose[MAX_SIZE][MAX_SIZE];
    transpose_matrix(rows1, cols1, matrix1, transpose);
    print_matrix(cols1, rows1, transpose);
    
    if (cols1 == rows2) {
        printf("\nMatrix Multiplication (Matrix 1 × Matrix 2):\n");
        multiply_matrices(rows1, cols1, matrix1, rows2, cols2, matrix2, result);
        print_matrix(rows1, cols2, result);
    } else {
        printf("\nCannot multiply matrices: columns of matrix 1 (%d) must equal rows of matrix 2 (%d)\n", cols1, rows2);
    }
    
    return 0;
}