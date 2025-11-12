//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_DIM 100

int read_int(const char* prompt, int min, int max) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Error reading input\n");
            exit(1);
        }
        
        if (sscanf(buffer, "%d", &value) == 1) {
            if (value >= min && value <= max) {
                return value;
            }
        }
        printf("Please enter a valid integer between %d and %d\n", min, max);
    }
}

void read_matrix(int rows, int cols, int matrix[MAX_DIM][MAX_DIM]) {
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            matrix[i][j] = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

void print_matrix(int rows, int cols, int matrix[MAX_DIM][MAX_DIM]) {
    printf("Matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void add_matrices(int rows, int cols, int a[MAX_DIM][MAX_DIM], int b[MAX_DIM][MAX_DIM], int result[MAX_DIM][MAX_DIM]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if ((a[i][j] > 0 && b[i][j] > INT_MAX - a[i][j]) || 
                (a[i][j] < 0 && b[i][j] < INT_MIN - a[i][j])) {
                printf("Integer overflow detected at position [%d][%d]\n", i, j);
                exit(1);
            }
            result[i][j] = a[i][j] + b[i][j];
        }
    }
}

void multiply_matrices(int rows1, int cols1, int cols2, int a[MAX_DIM][MAX_DIM], int b[MAX_DIM][MAX_DIM], int result[MAX_DIM][MAX_DIM]) {
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            result[i][j] = 0;
            for (int k = 0; k < cols1; k++) {
                long long product = (long long)a[i][k] * b[k][j];
                if (product > INT_MAX || product < INT_MIN) {
                    printf("Integer overflow detected at position [%d][%d]\n", i, j);
                    exit(1);
                }
                long long sum = (long long)result[i][j] + product;
                if (sum > INT_MAX || sum < INT_MIN) {
                    printf("Integer overflow detected at position [%d][%d]\n", i, j);
                    exit(1);
                }
                result[i][j] = (int)sum;
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
        rows1 = read_int("Enter number of rows for both matrices: ", 1, MAX_DIM);
        cols1 = read_int("Enter number of columns for both matrices: ", 1, MAX_DIM);
        
        printf("\nFirst matrix:\n");
        read_matrix(rows1, cols1, matrix1);
        
        printf("\nSecond matrix:\n");
        read_matrix(rows1, cols1, matrix2);
        
        add_matrices(rows1, cols1, matrix1, matrix2, result);
        
        printf("\nResult of addition:\n");
        print_matrix(rows1, cols1, result);
        
    } else {
        printf("\nMatrix Multiplication\n");
        rows1 = read_int("Enter number of rows for first matrix: ", 1, MAX_DIM);
        cols1 = read_int("Enter number of columns for first matrix: ", 1, MAX_DIM);
        
        printf("Enter number of rows for second matrix: %d\n", cols1);
        rows2 = cols1;
        cols2 = read_int("Enter number of columns for second matrix: ", 1, MAX_DIM);
        
        printf("\nFirst matrix:\n");
        read_matrix(rows1, cols1, matrix1);
        
        printf("\nSecond matrix:\n");
        read_matrix(rows2, cols2, matrix2);
        
        multiply_matrices(