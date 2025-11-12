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

void fill_matrix(int rows, int cols, int matrix[MAX_DIM][MAX_DIM]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = read_int("Enter element: ", INT_MIN, INT_MAX);
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
            if ((a[i][j] > 0 && b[i][j] > INT_MAX - a[i][j]) || (a[i][j] < 0 && b[i][j] < INT_MIN - a[i][j])) {
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
            result[i][j] = 0;
            for (int k = 0; k < a_cols; k++) {
                long long product = (long long)a[i][k] * b[k][j];
                if (product > INT_MAX || product < INT_MIN) {
                    result[i][j] = 0;
                    break;
                }
                long long sum = (long long)result[i][j] + product;
                if (sum > INT_MAX || sum < INT_MIN) {
                    result[i][j] = 0;
                    break;
                }
                result[i][j] = (int)sum;
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
    
    while (1) {
        printf("\nMatrix Operations Menu:\n");
        printf("1. Add two matrices\n");
        printf("2. Multiply two matrices\n");
        printf("3. Transpose a matrix\n");
        printf("4. Exit\n");
        
        choice = read_int("Enter your choice: ", 1, 4);
        
        if (choice == 4) {
            break;
        }
        
        switch (choice) {
            case 1:
                rows1 = read_int("Enter rows for first matrix: ", 1, MAX_DIM);
                cols1 = read_int("Enter columns for first matrix: ", 1, MAX_DIM);
                printf("Enter first matrix:\n");
                fill_matrix(rows1, cols1, matrix1);
                
                rows2 = read_int("Enter rows for second matrix: ", 1, MAX_DIM);
                cols2 = read_int("Enter columns for second matrix: ", 1, MAX_DIM);
                if (rows1 != rows2 || cols1 != cols2) {
                    printf("Matrix dimensions must match for addition.\n");
                    break;
                }
                printf("Enter second matrix:\n");
                fill_matrix(rows2, cols2, matrix2);
                
                add_matrices(rows1, cols1, matrix1, matrix2, result);
                printf("Result:\n");
                print_matrix(rows1, cols1, result);
                break;
                
            case 2:
                rows1 = read_int("Enter rows for first matrix: ", 1, MAX_DIM);
                cols1 = read_int("Enter columns for first matrix: ", 1, MAX_DIM