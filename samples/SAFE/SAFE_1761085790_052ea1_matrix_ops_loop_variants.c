//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: matrix_ops
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
        int j = 0;
        do {
            if (a[i][j] > 0 && b[i][j] > INT_MAX - a[i][j]) {
                printf("Integer overflow detected at [%d][%d]\n", i, j);
                exit(1);
            }
            result[i][j] = a[i][j] + b[i][j];
            j++;
        } while (j < cols);
    }
}

int main(void) {
    int rows, cols;
    
    printf("Enter matrix dimensions (rows columns, max %dx%d): ", MAX_SIZE, MAX_SIZE);
    if (scanf("%d %d", &rows, &cols) != 2) {
        printf("Invalid input format\n");
        return 1;
    }
    
    if (rows <= 0 || cols <= 0 || rows > MAX_SIZE || cols > MAX_SIZE) {
        printf("Invalid dimensions. Must be between 1 and %d\n", MAX_SIZE);
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
    
    printf("\nMatrix A + B:\n");
    add_matrices(rows, cols, matrix_a, matrix_b, matrix_sum);
    print_matrix(rows, cols, matrix_sum);
    
    printf("\nTranspose of A + B:\n");
    transpose_matrix(rows, cols, matrix_sum, matrix_transpose);
    print_matrix(cols, rows, matrix_transpose);
    
    return 0;
}