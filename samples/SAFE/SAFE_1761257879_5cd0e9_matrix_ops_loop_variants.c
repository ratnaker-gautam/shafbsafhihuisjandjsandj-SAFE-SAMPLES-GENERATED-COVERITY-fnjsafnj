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

int read_int_safe(const char* prompt, int min, int max) {
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
        
        if (value < min || value > max) {
            printf("Value must be between %d and %d\n", min, max);
            continue;
        }
        
        return (int)value;
    }
}

void read_matrix(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[64];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            matrix[i][j] = read_int_safe(prompt, -1000, 1000);
        }
    }
}

int main() {
    int choice;
    int rows1, cols1, rows2, cols2;
    int matrix1[MAX_SIZE][MAX_SIZE];
    int matrix2[MAX_SIZE][MAX_SIZE];
    int result[MAX_SIZE][MAX_SIZE];
    
    printf("Matrix Operations Program\n");
    printf("1. Transpose matrix\n");
    printf("2. Multiply matrices\n");
    
    choice = read_int_safe("Select operation (1-2): ", 1, 2);
    
    switch (choice) {
        case 1: {
            rows1 = read_int_safe("Enter number of rows (1-10): ", 1, MAX_SIZE);
            cols1 = read_int_safe("Enter number of columns (1-10): ", 1, MAX_SIZE);
            
            read_matrix(rows1, cols1, matrix1);
            
            printf("\nOriginal matrix:\n");
            print_matrix(rows1, cols1, matrix1);
            
            transpose_matrix(rows1, cols1, matrix1, result);
            
            printf("\nTransposed matrix:\n");
            print_matrix(cols1, rows1, result);
            break;
        }
        
        case 2: {
            rows1 = read_int_safe("Enter rows for first matrix (1-10): ", 1, MAX_SIZE);
            cols1 = read_int_safe("Enter columns for first matrix (1-10): ", 1, MAX_SIZE);
            rows2 = read_int_safe("Enter rows for second matrix (1-10): ", 1, MAX_SIZE);
            cols2 = read_int_safe("Enter columns for second matrix (1-10): ", 1, MAX_SIZE);
            
            if (cols1 != rows2) {
                printf("Error: Columns of first matrix must equal rows of second matrix\n");
                return 1;
            }
            
            printf("\nFirst matrix:\n");
            read_matrix(rows1, cols1, matrix1);
            
            printf("\nSecond matrix:\n");
            read_matrix(rows2, cols2, matrix2);
            
            printf("\nFirst matrix:\n");
            print_matrix(rows1, cols1, matrix1);
            
            printf("\nSecond matrix:\n");
            print_matrix(rows2, cols2, matrix2);
            
            multiply_matrices(rows1, cols1, matrix1, rows2, cols2, matrix2, result);
            
            printf("\nResult of multiplication:\n");
            print_matrix(rows1, cols2, result);
            break;
        }
    }
    
    return 0;
}