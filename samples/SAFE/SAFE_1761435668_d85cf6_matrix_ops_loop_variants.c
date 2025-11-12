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
    int i = 0;
    while (i < rows) {
        int j = 0;
        do {
            result[i][j] = a[i][j] + b[i][j];
            j++;
        } while (j < cols);
        i++;
    }
}

int main() {
    int rows, cols;
    
    printf("Enter number of rows (1-%d): ", MAX_SIZE);
    if (scanf("%d", &rows) != 1 || rows < 1 || rows > MAX_SIZE) {
        printf("Invalid input for rows\n");
        return 1;
    }
    
    printf("Enter number of columns (1-%d): ", MAX_SIZE);
    if (scanf("%d", &cols) != 1 || cols < 1 || cols > MAX_SIZE) {
        printf("Invalid input for columns\n");
        return 1;
    }
    
    int matrix_a[MAX_SIZE][MAX_SIZE];
    int matrix_b[MAX_SIZE][MAX_SIZE];
    int matrix_sum[MAX_SIZE][MAX_SIZE];
    int matrix_transpose[MAX_SIZE][MAX_SIZE];
    
    fill_matrix(rows, cols, matrix_a);
    fill_matrix(rows, cols, matrix_b);
    
    printf("\nMatrix A:\n");
    print_matrix(rows, cols, matrix_a);
    
    printf("\nMatrix B:\n");
    print_matrix(rows, cols, matrix_b);
    
    add_matrices(rows, cols, matrix_a, matrix_b, matrix_sum);
    printf("\nMatrix A + B:\n");
    print_matrix(rows, cols, matrix_sum);
    
    transpose_matrix(rows, cols, matrix_a, matrix_transpose);
    printf("\nTranspose of A:\n");
    print_matrix(cols, rows, matrix_transpose);
    
    return 0;
}