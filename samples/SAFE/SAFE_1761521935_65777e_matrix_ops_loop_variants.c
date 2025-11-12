//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 8

void fill_matrix(int rows, int cols, int matrix[rows][cols]) {
    int counter = 1;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = counter++;
        }
    }
}

void print_matrix(int rows, int cols, int matrix[rows][cols]) {
    int i = 0;
    while (i < rows) {
        int j = 0;
        do {
            printf("%4d", matrix[i][j]);
            j++;
        } while (j < cols);
        printf("\n");
        i++;
    }
}

void transpose_matrix(int rows, int cols, int src[rows][cols], int dest[cols][rows]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            dest[j][i] = src[i][j];
        }
    }
}

void multiply_matrices(int r1, int c1, int m1[r1][c1], int r2, int c2, int m2[r2][c2], int result[r1][c2]) {
    for (int i = 0; i < r1; i++) {
        for (int j = 0; j < c2; j++) {
            result[i][j] = 0;
            int k = 0;
            while (k < c1) {
                result[i][j] += m1[i][k] * m2[k][j];
                k++;
            }
        }
    }
}

int get_valid_int(const char* prompt, int min, int max) {
    char input[32];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(input, sizeof(input), stdin) == NULL) {
            return min;
        }
        
        char* endptr;
        long val = strtol(input, &endptr, 10);
        
        if (endptr == input || *endptr != '\n') {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (val < min || val > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        value = (int)val;
        break;
    }
    
    return value;
}

int main(void) {
    int rows1, cols1, rows2, cols2;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    rows1 = get_valid_int("Enter rows for matrix 1 (1-8): ", 1, MAX_SIZE);
    cols1 = get_valid_int("Enter columns for matrix 1 (1-8): ", 1, MAX_SIZE);
    
    int matrix1[MAX_SIZE][MAX_SIZE];
    fill_matrix(rows1, cols1, matrix1);
    
    printf("\nMatrix 1:\n");
    print_matrix(rows1, cols1, matrix1);
    
    int transposed[MAX_SIZE][MAX_SIZE];
    transpose_matrix(rows1, cols1, matrix1, transposed);
    
    printf("\nTransposed Matrix 1:\n");
    print_matrix(cols1, rows1, transposed);
    
    printf("\nMatrix Multiplication:\n");
    rows2 = get_valid_int("Enter rows for matrix 2 (1-8): ", 1, MAX_SIZE);
    cols2 = get_valid_int("Enter columns for matrix 2 (1-8): ", 1, MAX_SIZE);
    
    if (cols1 != rows2) {
        printf("Cannot multiply: columns of matrix 1 (%d) must equal rows of matrix 2 (%d)\n", cols1, rows2);
        return 1;
    }
    
    int matrix2[MAX_SIZE][MAX_SIZE];
    fill_matrix(rows2, cols2, matrix2);
    
    printf("\nMatrix 2:\n");
    print_matrix(rows2, cols2, matrix2);
    
    int result[MAX_SIZE][MAX_SIZE];
    multiply_matrices(rows1, cols1, matrix1, rows2, cols2, matrix2, result);
    
    printf("\nMultiplication Result:\n");
    print_matrix(rows1, cols2, result);
    
    return 0;
}