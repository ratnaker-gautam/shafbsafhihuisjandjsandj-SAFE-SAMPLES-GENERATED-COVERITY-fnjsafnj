//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 8

void fill_matrix(int rows, int cols, int matrix[rows][cols]) {
    int counter = 1;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = counter++;
        }
    }
}

void print_matrix(int rows, int cols, int matrix[rows][cols]) {
    int i = 0;
    while (i < rows) {
        int j = 0;
        do {
            printf("%4d", matrix[i][j]);
            j++;
        } while (j < cols);
        printf("\n");
        i++;
    }
}

void transpose_matrix(int rows, int cols, int src[rows][cols], int dest[cols][rows]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            dest[j][i] = src[i][j];
        }
    }
}

void add_matrices(int rows, int cols, int a[rows][cols], int b[rows][cols], int result[rows][cols]) {
    int *ptr_a = &a[0][0];
    int *ptr_b = &b[0][0];
    int *ptr_res = &result[0][0];
    int total = rows * cols;
    
    for (int idx = 0; idx < total; idx++) {
        if ((*ptr_b > 0 && *ptr_a > INT_MAX - *ptr_b) || 
            (*ptr_b < 0 && *ptr_a < INT_MIN - *ptr_b)) {
            printf("Integer overflow detected!\n");
            exit(1);
        }
        *ptr_res = *ptr_a + *ptr_b;
        ptr_a++;
        ptr_b++;
        ptr_res++;
    }
}

int get_valid_int(const char* prompt, int min, int max) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Input error\n");
            exit(1);
        }
        
        char* endptr;
        value = strtol(buffer, &endptr, 10);
        
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (value < min || value > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        break;
    }
    
    return value;
}

int main() {
    int rows, cols;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    rows = get_valid_int("Enter number of rows (1-8): ", 1, MAX_SIZE);
    cols = get_valid_int("Enter number of columns (1-8): ", 1, MAX_SIZE);
    
    int matrix_a[MAX_SIZE][MAX_SIZE];
    int matrix_b[MAX_SIZE][MAX_SIZE];
    int result[MAX_SIZE][MAX_SIZE];
    int transposed[MAX_SIZE][MAX_SIZE];
    
    printf("\nMatrix A:\n");
    fill_matrix(rows, cols, matrix_a);
    print_matrix(rows, cols, matrix_a);
    
    printf("\nMatrix B:\n");
    fill_matrix(rows, cols, matrix_b);
    print_matrix(rows, cols, matrix_b);
    
    printf("\nMatrix Addition Result:\n");
    add_matrices(rows, cols, matrix_a, matrix_b, result);
    print_matrix(rows, cols, result);
    
    if (rows == cols) {
        printf("\nTranspose of Matrix A:\n");
        transpose_matrix(rows, cols, matrix_a, transposed);
        print_matrix(cols, rows, transposed);
    } else {
        printf("\nTranspose requires square matrix (rows == columns)\n");
    }
    
    printf("\nElement access using different loop styles:\n");
    printf("While loop (first row): ");
    int col_idx = 0;
    while (col_idx < cols) {
        printf("%d ", matrix_a[0][col_idx]);
        col_idx++;
    }
    printf("\n");
    
    printf("Do-while loop (first column): ");
    int row_idx = 0;
    if (rows > 0) {
        do {
            printf("%d ", matrix_a[row_idx][0]);
            row_idx++;
        } while (row_idx < rows);
    }
    printf("\n");
    
    return 0;
}