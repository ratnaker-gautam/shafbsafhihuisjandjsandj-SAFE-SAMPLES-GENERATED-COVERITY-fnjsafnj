//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 8

void print_matrix(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%4d", matrix[i][j]);
        }
        printf("\n");
    }
}

int read_int(const char* prompt, int min, int max) {
    char buffer[32];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Input error\n");
            exit(1);
        }
        
        char* endptr;
        long temp = strtol(buffer, &endptr, 10);
        
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (temp < min || temp > max) {
            printf("Value must be between %d and %d\n", min, max);
            continue;
        }
        
        value = (int)temp;
        break;
    }
    
    return value;
}

void fill_matrix(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
    int count = 1;
    int i = 0;
    
    while (i < rows) {
        int j = 0;
        do {
            matrix[i][j] = count++;
            j++;
        } while (j < cols);
        i++;
    }
}

void transpose_matrix(int rows, int cols, int src[MAX_SIZE][MAX_SIZE], int dest[MAX_SIZE][MAX_SIZE]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            dest[j][i] = src[i][j];
        }
    }
}

void add_matrices(int rows, int cols, int a[MAX_SIZE][MAX_SIZE], int b[MAX_SIZE][MAX_SIZE], int result[MAX_SIZE][MAX_SIZE]) {
    int i = 0;
    
    while (i < rows) {
        int j = 0;
        do {
            if ((a[i][j] > 0 && b[i][j] > INT_MAX - a[i][j]) || 
                (a[i][j] < 0 && b[i][j] < INT_MIN - a[i][j])) {
                printf("Integer overflow detected\n");
                exit(1);
            }
            result[i][j] = a[i][j] + b[i][j];
            j++;
        } while (j < cols);
        i++;
    }
}

int main() {
    int rows, cols;
    int matrix_a[MAX_SIZE][MAX_SIZE];
    int matrix_b[MAX_SIZE][MAX_SIZE];
    int result[MAX_SIZE][MAX_SIZE];
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    rows = read_int("Enter number of rows (1-8): ", 1, MAX_SIZE);
    cols = read_int("Enter number of columns (1-8): ", 1, MAX_SIZE);
    
    printf("\nFilling matrix A:\n");
    fill_matrix(rows, cols, matrix_a);
    print_matrix(rows, cols, matrix_a);
    
    printf("\nFilling matrix B:\n");
    fill_matrix(rows, cols, matrix_b);
    print_matrix(rows, cols, matrix_b);
    
    printf("\nMatrix A + Matrix B:\n");
    add_matrices(rows, cols, matrix_a, matrix_b, result);
    print_matrix(rows, cols, result);
    
    if (rows == cols) {
        printf("\nTranspose of Matrix A:\n");
        int transposed[MAX_SIZE][MAX_SIZE];
        transpose_matrix(rows, cols, matrix_a, transposed);
        print_matrix(cols, rows, transposed);
    } else {
        printf("\nTranspose requires square matrix (rows == columns)\n");
    }
    
    printf("\nMatrix operations completed.\n");
    return 0;
}