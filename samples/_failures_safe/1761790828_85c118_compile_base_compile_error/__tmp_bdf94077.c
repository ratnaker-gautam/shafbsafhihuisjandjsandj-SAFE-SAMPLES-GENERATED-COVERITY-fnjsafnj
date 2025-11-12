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
        if (sscanf(buffer, "%d", &value) == 1 && value >= min && value <= max) {
            return value;
        }
        printf("Invalid input. Enter integer between %d and %d.\n", min, max);
    }
}

void fill_matrix(int rows, int cols, int matrix[MAX_DIM][MAX_DIM]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = read_int("Enter element: ", -1000, 1000);
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

void multiply_matrices(int rows1, int cols1, int a[MAX_DIM][MAX_DIM], int rows2, int cols2, int b[MAX_DIM][MAX_DIM], int result[MAX_DIM][MAX_DIM]) {
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            long sum = 0;
            for (int k = 0; k < cols1; k++) {
                long product = (long)a[i][k] * b[k][j];
                if (product > INT_MAX - sum || product < INT_MIN + sum) {
                    sum = 0;
                    break;
                }
                sum += product;
            }
            result[i][j] = (sum > INT_MAX || sum < INT_MIN) ? 0 : (int)sum;
        }
    }
}

int main() {
    int choice;
    int rows1, cols1, rows2, cols2;
    int matrix1[MAX_DIM][MAX_DIM];
    int matrix2[MAX_DIM][MAX_DIM];
    int result[MAX_DIM][MAX_DIM];
    
    printf("Matrix Operations\n");
    printf("1. Add matrices\n");
    printf("2. Multiply matrices\n");
    
    choice = read_int("Select operation (1-2): ", 1, 2);
    
    if (choice == 1) {
        rows1 = read_int("Enter rows for matrix 1 (1-10): ", 1, MAX_DIM);
        cols1 = read_int("Enter columns for matrix 1 (1-10): ", 1, MAX_DIM);
        printf("Fill matrix 1:\n");
        fill_matrix(rows1, cols1, matrix1);
        
        rows2 = read_int("Enter rows for matrix 2 (1-10): ", 1, MAX_DIM);
        cols2 = read_int("Enter columns for matrix 2 (1-10): ", 1, MAX_DIM);
        printf("Fill matrix 2:\n");
        fill_matrix(rows2, cols2, matrix2);
        
        if (rows1 != rows2 || cols1 != cols2) {
            printf("Error: Matrices must have same dimensions for addition.\n");
            return 1;
        }
        
        add_matrices(rows1, cols1, matrix1, matrix2, result);
        
        printf("Matrix 1:\n");
        print_matrix(rows1, cols1, matrix1);
        printf("Matrix 2:\n");
        print_matrix(rows2, cols2, matrix2);
        printf("Result:\n");
        print_matrix(rows1, cols1, result);
    } else {
        rows1 = read_int("Enter rows for matrix 1 (1-10): ", 1, MAX_DIM);
        cols1 = read_int("Enter columns for matrix 1 (1-10): ", 1, MAX_DIM);
        printf("Fill matrix 1:\n");
        fill_matrix(rows1, cols1, matrix1);
        
        rows2 = read_int("Enter rows for matrix 2 (1-10): ", 1, MAX_DIM);
        cols2 = read_int("Enter columns for matrix 2 (1-10): ", 1, MAX_DIM);
        printf("Fill matrix 2:\n");
        fill_matrix(rows2, cols2