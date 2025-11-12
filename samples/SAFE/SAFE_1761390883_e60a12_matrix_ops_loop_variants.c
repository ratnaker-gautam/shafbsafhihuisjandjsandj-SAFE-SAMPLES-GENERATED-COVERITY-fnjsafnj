//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 10

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
    if (c1 != r2) return;
    
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
            printf("Input error\n");
            exit(1);
        }
        
        char* endptr;
        value = strtol(input, &endptr, 10);
        
        if (endptr == input || *endptr != '\n') {
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

int main(void) {
    int rows1, cols1, rows2, cols2;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    rows1 = get_valid_int("Enter rows for matrix 1 (1-10): ", 1, MAX_SIZE);
    cols1 = get_valid_int("Enter columns for matrix 1 (1-10): ", 1, MAX_SIZE);
    
    int matrix1[MAX_SIZE][MAX_SIZE];
    fill_matrix(rows1, cols1, matrix1);
    
    printf("\nMatrix 1:\n");
    print_matrix(rows1, cols1, matrix1);
    
    rows2 = get_valid_int("\nEnter rows for matrix 2 (1-10): ", 1, MAX_SIZE);
    cols2 = get_valid_int("Enter columns for matrix 2 (1-10): ", 1, MAX_SIZE);
    
    int matrix2[MAX_SIZE][MAX_SIZE];
    fill_matrix(rows2, cols2, matrix2);
    
    printf("\nMatrix 2:\n");
    print_matrix(rows2, cols2, matrix2);
    
    printf("\nTranspose of Matrix 1:\n");
    int transpose[MAX_SIZE][MAX_SIZE];
    transpose_matrix(rows1, cols1, matrix1, transpose);
    print_matrix(cols1, rows1, transpose);
    
    if (cols1 == rows2) {
        printf("\nMatrix Multiplication Result:\n");
        int result[MAX_SIZE][MAX_SIZE];
        multiply_matrices(rows1, cols1, matrix1, rows2, cols2, matrix2, result);
        print_matrix(rows1, cols2, result);
    } else {
        printf("\nCannot multiply matrices: columns of first (%d) must equal rows of second (%d)\n", cols1, rows2);
    }
    
    printf("\nElement-wise operations:\n");
    if (rows1 == rows2 && cols1 == cols2) {
        int sum[MAX_SIZE][MAX_SIZE];
        int diff[MAX_SIZE][MAX_SIZE];
        
        for (int i = 0; i < rows1; i++) {
            int j = 0;
            while (j < cols1) {
                sum[i][j] = matrix1[i][j] + matrix2[i][j];
                diff[i][j] = matrix1[i][j] - matrix2[i][j];
                j++;
            }
        }
        
        printf("Sum:\n");
        print_matrix(rows1, cols1, sum);
        printf("Difference:\n");
        print_matrix(rows1, cols1, diff);
    } else {
        printf("Cannot perform element-wise operations: matrices must have same dimensions\n");
    }
    
    return 0;
}