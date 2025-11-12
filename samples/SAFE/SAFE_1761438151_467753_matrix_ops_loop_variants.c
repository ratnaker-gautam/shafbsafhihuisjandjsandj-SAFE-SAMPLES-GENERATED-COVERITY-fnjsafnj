//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 10

void print_matrix(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void transpose_matrix(int rows, int cols, int src[MAX_SIZE][MAX_SIZE], int dest[MAX_SIZE][MAX_SIZE]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            dest[j][i] = src[i][j];
        }
    }
}

void multiply_matrices(int rows1, int cols1, int mat1[MAX_SIZE][MAX_SIZE], int rows2, int cols2, int mat2[MAX_SIZE][MAX_SIZE], int result[MAX_SIZE][MAX_SIZE]) {
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            result[i][j] = 0;
            int k = 0;
            while (k < cols1) {
                result[i][j] += mat1[i][k] * mat2[k][j];
                k++;
            }
        }
    }
}

int read_int_safe(const char* prompt, int min_val, int max_val) {
    char buffer[32];
    long value;
    char* endptr;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Input error\n");
            exit(1);
        }
        
        value = strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (value < min_val || value > max_val) {
            printf("Value must be between %d and %d.\n", min_val, max_val);
            continue;
        }
        
        return (int)value;
    }
}

void read_matrix(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[64];
            snprintf(prompt, sizeof(prompt), "Enter element [%d][%d]: ", i, j);
            matrix[i][j] = read_int_safe(prompt, -1000, 1000);
        }
    }
}

int main() {
    int choice;
    int rows1, cols1, rows2, cols2;
    int matrix1[MAX_SIZE][MAX_SIZE] = {0};
    int matrix2[MAX_SIZE][MAX_SIZE] = {0};
    int result[MAX_SIZE][MAX_SIZE] = {0};
    
    printf("Matrix Operations Program\n");
    printf("1. Transpose Matrix\n");
    printf("2. Multiply Matrices\n");
    
    choice = read_int_safe("Select operation (1-2): ", 1, 2);
    
    if (choice == 1) {
        rows1 = read_int_safe("Enter number of rows (1-10): ", 1, MAX_SIZE);
        cols1 = read_int_safe("Enter number of columns (1-10): ", 1, MAX_SIZE);
        
        printf("Enter matrix elements:\n");
        read_matrix(rows1, cols1, matrix1);
        
        printf("\nOriginal Matrix:\n");
        print_matrix(rows1, cols1, matrix1);
        
        transpose_matrix(rows1, cols1, matrix1, result);
        
        printf("\nTransposed Matrix:\n");
        print_matrix(cols1, rows1, result);
        
    } else if (choice == 2) {
        rows1 = read_int_safe("Enter rows for matrix 1 (1-10): ", 1, MAX_SIZE);
        cols1 = read_int_safe("Enter columns for matrix 1 (1-10): ", 1, MAX_SIZE);
        
        printf("Enter matrix 1 elements:\n");
        read_matrix(rows1, cols1, matrix1);
        
        rows2 = read_int_safe("Enter rows for matrix 2 (1-10): ", 1, MAX_SIZE);
        cols2 = read_int_safe("Enter columns for matrix 2 (1-10): ", 1, MAX_SIZE);
        
        if (cols1 != rows2) {
            printf("Error: Matrix 1 columns must equal matrix 2 rows for multiplication.\n");
            return 1;
        }
        
        printf("Enter matrix 2 elements:\n");
        read_matrix(rows2, cols2, matrix2);
        
        printf("\nMatrix 1:\n");
        print_matrix(rows1, cols1, matrix1);
        
        printf("\nMatrix 2:\n");
        print_matrix(rows2, cols2, matrix2);
        
        multiply_matrices(rows1, cols1, matrix1, rows2, cols2, matrix2, result);
        
        printf("\nProduct Matrix:\n");
        print_matrix(rows1, cols2, result);
    }
    
    return 0;
}