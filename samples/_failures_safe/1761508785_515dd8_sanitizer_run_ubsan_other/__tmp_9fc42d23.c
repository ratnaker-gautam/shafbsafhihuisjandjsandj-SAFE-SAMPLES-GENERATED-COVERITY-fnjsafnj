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
            matrix[i][j] = read_int("Enter matrix element: ", INT_MIN, INT_MAX);
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

void multiply_matrices(int a_rows, int a_cols, int b_cols, int a[MAX_DIM][MAX_DIM], int b[MAX_DIM][MAX_DIM], int result[MAX_DIM][MAX_DIM]) {
    for (int i = 0; i < a_rows; i++) {
        for (int j = 0; j < b_cols; j++) {
            result[i][j] = 0;
            for (int k = 0; k < a_cols; k++) {
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
    
    while (1) {
        printf("\n1. Add matrices\n");
        printf("2. Multiply matrices\n");
        printf("3. Exit\n");
        choice = read_int("Select operation: ", 1, 3);
        
        if (choice == 3) {
            break;
        }
        
        if (choice == 1) {
            printf("\nMatrix Addition\n");
            rows1 = read_int("Enter rows for matrix 1: ", 1, MAX_DIM);
            cols1 = read_int("Enter columns for matrix 1: ", 1, MAX_DIM);
            printf("Matrix 1:\n");
            fill_matrix(rows1, cols1, matrix1);
            
            rows2 = read_int("Enter rows for matrix 2: ", 1, MAX_DIM);
            cols2 = read_int("Enter columns for matrix 2: ", 1, MAX_DIM);
            
            if (rows1 != rows2 || cols1 != cols2) {
                printf("Error: Matrices must have same dimensions for addition.\n");
                continue;
            }
            
            printf("Matrix 2:\n");
            fill_matrix(rows2, cols2, matrix2);
            
            add_matrices(rows1, cols1, matrix1, matrix2, result);
            
            printf("\nResult:\n");
            print_matrix(rows1, cols1, result);
        }
        else if (choice == 2) {
            printf("\nMatrix Multiplication\n");
            rows1 = read_int("Enter rows for matrix 1: ", 1, MAX_DIM);
            cols1 = read_int("Enter columns for matrix 1: ", 1, MAX_DIM);
            printf("Matrix 1:\n");
            fill_matrix(rows1, cols1, matrix1);
            
            rows2 = read_int("Enter rows for matrix 2: ", 1, MAX_DIM);
            cols2 = read_int("Enter columns for matrix 2: ", 1, MAX_DIM);
            
            if (cols1 != rows2) {
                printf("Error: Matrix 1 columns must equal matrix 2 rows for multiplication.\n");
                continue;
            }
            
            printf("Matrix 2:\n");
            fill_matrix(rows2, cols2, matrix2);
            
            multiply_matrices(rows1, cols1, cols2, matrix1, matrix2, result);
            
            printf("\nResult:\n");
            print_matrix(rows1, cols2, result);
        }
    }
    
    return 0;
}