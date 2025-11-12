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
        if (sscanf(buffer, "%d", &value) == 1) {
            if (value >= min && value <= max) {
                return value;
            }
        }
        printf("Invalid input. Enter integer between %d and %d.\n", min, max);
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

void input_matrix(int rows, int cols, int matrix[MAX_DIM][MAX_DIM]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[64];
            snprintf(prompt, sizeof(prompt), "Enter element [%d][%d]: ", i, j);
            matrix[i][j] = read_int(prompt, -1000, 1000);
        }
    }
}

void add_matrices(int rows, int cols, int a[MAX_DIM][MAX_DIM], int b[MAX_DIM][MAX_DIM], int result[MAX_DIM][MAX_DIM]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if ((a[i][j] > 0 && b[i][j] > INT_MAX - a[i][j]) || (a[i][j] < 0 && b[i][j] < INT_MIN - a[i][j])) {
                result[i][j] = 0;
            } else {
                result[i][j] = a[i][j] + b[i][j];
            }
        }
    }
}

void multiply_matrices(int rows1, int cols1, int a[MAX_DIM][MAX_DIM], int rows2, int cols2, int b[MAX_DIM][MAX_DIM], int result[MAX_DIM][MAX_DIM]) {
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            int sum = 0;
            for (int k = 0; k < cols1; k++) {
                long long product = (long long)a[i][k] * b[k][j];
                if (product > INT_MAX || product < INT_MIN) {
                    sum = 0;
                    break;
                }
                if ((a[i][k] > 0 && b[k][j] > 0 && sum > INT_MAX - product) || (a[i][k] < 0 && b[k][j] < 0 && sum < INT_MIN - product)) {
                    sum = 0;
                    break;
                }
                sum += (int)product;
            }
            result[i][j] = sum;
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
    printf("4. Exit\n");
    
    while (1) {
        choice = read_int("Enter choice (1-4): ", 1, 4);
        
        if (choice == 4) {
            break;
        }
        
        switch (choice) {
            case 1:
                rows1 = read_int("Enter rows for matrix 1 (1-10): ", 1, MAX_DIM);
                cols1 = read_int("Enter columns for matrix 1 (1-10): ", 1, MAX_DIM);
                rows2 = read_int("Enter rows for matrix 2 (1-10): ", 1, MAX_DIM);
                cols2 = read_int("Enter columns for matrix 2 (1-10): ", 1, MAX_DIM);
                
                if (rows1 != rows2 || cols1 != cols2) {
                    printf("Error: Matrices must have same dimensions for addition.\n");
                    break;
                }
                
                printf("Enter first matrix:\n");
                input_matrix(rows1, cols1, matrix1);
                printf("Enter second matrix:\n");
                input_matrix(rows2, cols2, matrix2);
                
                add_matrices(rows1, cols1, matrix1, matrix2, result);
                
                printf("Result of addition:\n");
                print_matrix