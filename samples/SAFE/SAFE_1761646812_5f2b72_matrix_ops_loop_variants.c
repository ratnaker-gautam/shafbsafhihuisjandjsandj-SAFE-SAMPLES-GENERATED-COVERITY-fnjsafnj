//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 10

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
            printf("Integer overflow detected\n");
            exit(1);
        }
        *ptr_res = *ptr_a + *ptr_b;
        ptr_a++;
        ptr_b++;
        ptr_res++;
    }
}

int get_valid_int(const char* prompt, int min, int max) {
    char input[32];
    long value;
    char *endptr;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Input error\n");
            exit(1);
        }
        
        value = strtol(input, &endptr, 10);
        if (endptr == input || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (value < min || value > max) {
            printf("Value must be between %d and %d\n", min, max);
            continue;
        }
        
        return (int)value;
    }
}

int main(void) {
    int rows, cols;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    rows = get_valid_int("Enter number of rows (1-10): ", 1, MAX_SIZE);
    cols = get_valid_int("Enter number of columns (1-10): ", 1, MAX_SIZE);
    
    int matrix_a[MAX_SIZE][MAX_SIZE];
    int matrix_b[MAX_SIZE][MAX_SIZE];
    int result[MAX_SIZE][MAX_SIZE];
    int transposed[MAX_SIZE][MAX_SIZE];
    
    printf("\nFilling matrix A:\n");
    fill_matrix(rows, cols, matrix_a);
    print_matrix(rows, cols, matrix_a);
    
    printf("\nFilling matrix B:\n");
    fill_matrix(rows, cols, matrix_b);
    print_matrix(rows, cols, matrix_b);
    
    printf("\nAdding matrices A + B:\n");
    add_matrices(rows, cols, matrix_a, matrix_b, result);
    print_matrix(rows, cols, result);
    
    printf("\nTransposing matrix A:\n");
    transpose_matrix(rows, cols, matrix_a, transposed);
    print_matrix(cols, rows, transposed);
    
    printf("\nTransposing matrix B:\n");
    transpose_matrix(rows, cols, matrix_b, transposed);
    print_matrix(cols, rows, transposed);
    
    return 0;
}