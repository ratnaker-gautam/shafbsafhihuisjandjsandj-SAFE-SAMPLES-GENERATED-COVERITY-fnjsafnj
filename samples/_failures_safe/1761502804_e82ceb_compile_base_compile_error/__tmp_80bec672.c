//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 10

int read_int(const char* prompt, int min, int max) {
    int value;
    char buffer[100];
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min;
        }
        if (sscanf(buffer, "%d", &value) == 1) {
            if (value >= min && value <= max) {
                return value;
            }
        }
        printf("Invalid input. Enter a number between %d and %d.\n", min, max);
    }
}

void read_matrix(int rows, int cols, int matrix[MAX_DIM][MAX_DIM]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[50];
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
            if ((a[i][j] > 0 && b[i][j] > INT_MAX - a[i][j]) || 
                (a[i][j] < 0 && b[i][j] < INT_MIN - a[i][j])) {
                result[i][j] = 0;
            } else {
                result[i][j] = a[i][j] + b[i][j];
            }
        }
    }
}

void multiply_matrices(int a_rows, int a_cols, int b_cols, int a[MAX_DIM][MAX_DIM], int b[MAX_DIM][MAX_DIM], int result[MAX_DIM][MAX_DIM]) {
    for (int i = 0; i < a_rows; i++) {
        for (int j = 0; j < b_cols; j++) {
            long sum = 0;
            for (int k = 0; k < a_cols; k++) {
                long product = (long)a[i][k] * (long)b[k][j];
                if (product > INT_MAX - sum || product < INT_MIN - sum) {
                    sum = 0;
                    break;
                }
                sum += product;
            }
            result[i][j] = (int)sum;
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
    
    choice = read_int("Select operation (1-3): ", 1, 3);
    
    switch (choice) {
        case 1:
            rows1 = read_int("Enter rows for matrix 1: ", 1, MAX_DIM);
            cols1 = read_int("Enter columns for matrix 1: ", 1, MAX_DIM);
            rows2 = read_int("Enter rows for matrix 2: ", 1, MAX_DIM);
            cols2 = read_int("Enter columns for matrix 2: ", 1, MAX_DIM);
            
            if (rows1 != rows2 || cols1 != cols2) {
                printf("Error: Matrices must have same dimensions for addition.\n");
                return 1;
            }
            
            printf("Enter first matrix:\n");
            read_matrix(rows1, cols1, matrix1);
            printf("Enter second matrix:\n");
            read_matrix(rows2, cols2, matrix2);
            
            add_matrices(rows1, cols1, matrix1, matrix2, result);
            
            printf("Result of addition:\n");
            print_matrix(rows1, cols1, result);
            break;
            
        case 2:
            rows1 = read_int("Enter rows for matrix 1: ", 1, MAX_DIM);
            cols1 = read_int("Enter columns for matrix 1: ", 1, MAX_DIM);
            rows2 = read_int("Enter rows for matrix 2: ", 1, MAX_DIM);
            cols2 = read_int("Enter columns for matrix 2: ", 1, MAX