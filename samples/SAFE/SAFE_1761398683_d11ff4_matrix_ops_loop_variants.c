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

void multiply_matrices(int rows1, int cols1, int a[rows1][cols1], 
                      int rows2, int cols2, int b[rows2][cols2], 
                      int result[rows1][cols2]) {
    if (cols1 != rows2) {
        printf("Matrix dimensions incompatible for multiplication\n");
        exit(1);
    }
    
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            result[i][j] = 0;
            for (int k = 0; k < cols1; k++) {
                long long product = (long long)a[i][k] * b[k][j];
                if (product > INT_MAX || product < INT_MIN) {
                    printf("Integer overflow detected during multiplication\n");
                    exit(1);
                }
                if ((result[i][j] > 0 && product > INT_MAX - result[i][j]) ||
                    (result[i][j] < 0 && product < INT_MIN - result[i][j])) {
                    printf("Integer overflow detected during summation\n");
                    exit(1);
                }
                result[i][j] += (int)product;
            }
        }
    }
}

int main() {
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
    int matrix_result[MAX_SIZE][MAX_SIZE];
    
    fill_matrix(rows, cols, matrix_a);
    fill_matrix(rows, cols, matrix_b);
    
    printf("\nMatrix A:\n");
    print_matrix(rows, cols, matrix_a);
    
    printf("\nMatrix B:\n");
    print_matrix(rows, cols, matrix_b);
    
    printf("\nMatrix A + B:\n");
    add_matrices(rows, cols, matrix_a, matrix_b, matrix_result);
    print_matrix(rows, cols, matrix_result);
    
    if (rows == cols) {
        printf("\nTranspose of A:\n");
        int transposed[MAX_SIZE][MAX_SIZE];
        transpose_matrix(rows, cols, matrix_a, transposed);
        print_matrix(cols, rows, transposed);
    }
    
    if (cols <= MAX_SIZE && rows <= MAX_SIZE) {
        printf("\nMatrix A * B:\n");
        multiply_matrices(rows, cols, matrix_a, rows, cols, matrix_b, matrix_result);
        print_matrix(rows, cols, matrix_result);
    }
    
    return 0;
}