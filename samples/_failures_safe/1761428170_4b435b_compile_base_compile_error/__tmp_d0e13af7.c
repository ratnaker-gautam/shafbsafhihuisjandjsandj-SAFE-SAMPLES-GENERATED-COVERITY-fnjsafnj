//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 10

int read_int(const char* prompt, int min, int max) {
    char buffer[32];
    int value;
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min;
        }
        if (sscanf(buffer, "%d", &value) != 1) {
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

void read_matrix(int rows, int cols, int matrix[MAX_DIM][MAX_DIM]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[64];
            snprintf(prompt, sizeof(prompt), "Enter element [%d][%d]: ", i, j);
            matrix[i][j] = read_int(prompt, -1000, 1000);
        }
    }
}

void print_matrix(int rows, int cols, int matrix[MAX_DIM][MAX_DIM]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d", matrix[i][j]);
        }
        printf("\n");
    }
}

void add_matrices(int rows, int cols, int a[MAX_DIM][MAX_DIM], int b[MAX_DIM][MAX_DIM], int result[MAX_DIM][MAX_DIM]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = a[i][j] + b[i][j];
        }
    }
}

void multiply_matrices(int rows1, int cols1, int a[MAX_DIM][MAX_DIM], int rows2, int cols2, int b[MAX_DIM][MAX_DIM], int result[MAX_DIM][MAX_DIM]) {
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            result[i][j] = 0;
            for (int k = 0; k < cols1; k++) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

int main() {
    int choice;
    int rows1, cols1, rows2, cols2;
    int matrix1[MAX_DIM][MAX_DIM];
    int matrix2[MAX_DIM][MAX_DIM];
    int result[MAX_DIM][MAX_DIM];
    
    printf("Matrix Operations Program\n");
    printf("1. Matrix Addition\n");
    printf("2. Matrix Multiplication\n");
    
    choice = read_int("Select operation (1-2): ", 1, 2);
    
    if (choice == 1) {
        printf("\nMatrix Addition\n");
        rows1 = read_int("Enter rows for matrix 1 (1-10): ", 1, MAX_DIM);
        cols1 = read_int("Enter columns for matrix 1 (1-10): ", 1, MAX_DIM);
        rows2 = read_int("Enter rows for matrix 2 (1-10): ", 1, MAX_DIM);
        cols2 = read_int("Enter columns for matrix 2 (1-10): ", 1, MAX_DIM);
        
        if (rows1 != rows2 || cols1 != cols2) {
            printf("Error: Matrices must have same dimensions for addition.\n");
            return 1;
        }
        
        printf("\nEnter elements for matrix 1:\n");
        read_matrix(rows1, cols1, matrix1);
        printf("\nEnter elements for matrix 2:\n");
        read_matrix(rows2, cols2, matrix2);
        
        add_matrices(rows1, cols1, matrix1, matrix2, result);
        
        printf("\nMatrix 1:\n");
        print_matrix(rows1, cols1, matrix1);
        printf("\nMatrix 2:\n");
        print_matrix(rows2, cols2, matrix2);
        printf("\nResult:\n");
        print_matrix(rows1, cols1, result);
    } else {
        printf("\nMatrix Multiplication\n");
        rows1 = read_int("Enter rows for matrix 1 (1-10): ", 1, MAX_DIM);
        cols1 = read_int("Enter columns for matrix 1 (1-10): ", 1, MAX_DIM);
        rows2 = read_int("Enter rows for matrix 2 (1-10): ", 1, MAX_DIM);
        cols2 = read_int("Enter columns for matrix 2 (1-10): ", 1, MAX_DIM);
        
        if (cols1 != rows2) {
            printf("Error: Matrix 1 columns must equal matrix 2 rows for multiplication.\n");
            return 1;
        }
        
        printf("\nEnter elements for matrix 1:\n");
        read_matrix(rows1, cols1, matrix1);
        printf("\nEnter elements for matrix 2:\n");
        read_matrix(rows2, cols2, matrix2