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
            printf("Invalid input. Please enter a number.\n");
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
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "Enter element [%d][%d]: ", i, j);
            matrix[i][j] = read_int(prompt, -100, 100);
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

void transpose_matrix(int rows, int cols, int matrix[MAX_DIM][MAX_DIM], int result[MAX_DIM][MAX_DIM]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[j][i] = matrix[i][j];
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
    printf("3. Matrix Transpose\n");
    
    choice = read_int("Enter your choice (1-3): ", 1, 3);
    
    switch (choice) {
        case 1:
            printf("Matrix Addition\n");
            rows1 = read_int("Enter number of rows for matrix 1 (1-10): ", 1, MAX_DIM);
            cols1 = read_int("Enter number of columns for matrix 1 (1-10): ", 1, MAX_DIM);
            rows2 = read_int("Enter number of rows for matrix 2 (1-10): ", 1, MAX_DIM);
            cols2 = read_int("Enter number of columns for matrix 2 (1-10): ", 1, MAX_DIM);
            
            if (rows1 != rows2 || cols1 != cols2) {
                printf("Error: Matrices must have the same dimensions for addition.\n");
                return 1;
            }
            
            printf("Enter elements for matrix 1:\n");
            read_matrix(rows1, cols1, matrix1);
            printf("Enter elements for matrix 2:\n");
            read_matrix(rows2, cols2, matrix2);
            
            add_matrices(rows1, cols1, matrix1, matrix2, result);
            
            printf("Matrix 1:\n");
            print_matrix(rows1, cols1, matrix1);
            printf("Matrix 2:\n");
            print_matrix(rows2, cols2, matrix2);
            printf("Result:\n");
            print_matrix(rows1, cols1, result);
            break;
            
        case 2:
            printf("Matrix Multiplication\n");
            rows1 = read_int("Enter number of rows for matrix 1 (1-10): ", 1, MAX_DIM);
            cols1 = read_int("Enter number of columns for matrix 1 (1-10): ", 1, MAX_DIM);
            rows2 = read_int("Enter number of rows for matrix 2 (1-10): ", 1, MAX_DIM);
            cols2 = read_int("Enter number of columns for matrix