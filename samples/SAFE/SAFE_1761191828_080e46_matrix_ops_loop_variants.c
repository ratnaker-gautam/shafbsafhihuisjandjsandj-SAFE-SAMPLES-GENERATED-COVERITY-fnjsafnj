//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 10

void fill_matrix(int rows, int cols, int matrix[rows][cols]) {
    int counter = 1;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = counter++;
        }
    }
}

void print_matrix(int rows, int cols, int matrix[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%4d", matrix[i][j]);
        }
        printf("\n");
    }
}

void transpose_matrix(int rows, int cols, int src[rows][cols], int dest[cols][rows]) {
    int i = 0;
    while (i < rows) {
        int j = 0;
        while (j < cols) {
            dest[j][i] = src[i][j];
            j++;
        }
        i++;
    }
}

void add_matrices(int rows, int cols, int a[rows][cols], int b[rows][cols], int result[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if ((b[i][j] > 0 && a[i][j] > INT_MAX - b[i][j]) || 
                (b[i][j] < 0 && a[i][j] < INT_MIN - b[i][j])) {
                printf("Integer overflow detected at [%d][%d]\n", i, j);
                exit(1);
            }
            result[i][j] = a[i][j] + b[i][j];
        }
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
        value = strtol(buffer, &endptr, 10);
        
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter a valid integer.\n");
            continue;
        }
        
        if (value < min || value > max) {
            printf("Value must be between %d and %d\n", min, max);
            continue;
        }
        
        break;
    }
    
    return value;
}

int main(void) {
    int rows, cols;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    rows = read_int("Enter number of rows (1-10): ", 1, MAX_DIM);
    cols = read_int("Enter number of columns (1-10): ", 1, MAX_DIM);
    
    int matrix_a[MAX_DIM][MAX_DIM];
    int matrix_b[MAX_DIM][MAX_DIM];
    int matrix_sum[MAX_DIM][MAX_DIM];
    int matrix_transpose[MAX_DIM][MAX_DIM];
    
    fill_matrix(rows, cols, matrix_a);
    fill_matrix(rows, cols, matrix_b);
    
    printf("\nMatrix A:\n");
    print_matrix(rows, cols, matrix_a);
    
    printf("\nMatrix B:\n");
    print_matrix(rows, cols, matrix_b);
    
    printf("\nMatrix A + Matrix B:\n");
    add_matrices(rows, cols, matrix_a, matrix_b, matrix_sum);
    print_matrix(rows, cols, matrix_sum);
    
    printf("\nTranspose of Matrix A:\n");
    transpose_matrix(rows, cols, matrix_a, matrix_transpose);
    print_matrix(cols, rows, matrix_transpose);
    
    printf("\nTranspose of Matrix B:\n");
    transpose_matrix(rows, cols, matrix_b, matrix_transpose);
    print_matrix(cols, rows, matrix_transpose);
    
    return 0;
}