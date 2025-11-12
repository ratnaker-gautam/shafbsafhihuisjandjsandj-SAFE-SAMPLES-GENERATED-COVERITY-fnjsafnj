//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
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
                printf("Integer overflow detected at position [%d][%d]\n", i, j);
                exit(1);
            }
            result[i][j] = a[i][j] + b[i][j];
        }
    }
}

void multiply_matrices(int rows1, int cols1, int a[rows1][cols1], int rows2, int cols2, int b[rows2][cols2], int result[rows1][cols2]) {
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
    int rows1, cols1, rows2, cols2;
    
    printf("Enter dimensions for first matrix (rows columns, max %d): ", MAX_DIM);
    if (scanf("%d %d", &rows1, &cols1) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (rows1 <= 0 || rows1 > MAX_DIM || cols1 <= 0 || cols1 > MAX_DIM) {
        printf("Invalid dimensions\n");
        return 1;
    }
    
    printf("Enter dimensions for second matrix (rows columns, max %d): ", MAX_DIM);
    if (scanf("%d %d", &rows2, &cols2) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (rows2 <= 0 || rows2 > MAX_DIM || cols2 <= 0 || cols2 > MAX_DIM) {
        printf("Invalid dimensions\n");
        return 1;
    }
    
    int matrix1[MAX_DIM][MAX_DIM];
    int matrix2[MAX_DIM][MAX_DIM];
    int result[MAX_DIM][MAX_DIM];
    
    fill_matrix(rows1, cols1, matrix1);
    fill_matrix(rows2, cols2, matrix2);
    
    printf("\nMatrix 1:\n");
    print_matrix(rows1, cols1, matrix1);
    
    printf("\nMatrix 2:\n");
    print_matrix(rows2, cols2, matrix2);
    
    printf("\nMatrix 1 transposed:\n");
    int transposed1[cols1][rows1];
    transpose_matrix(rows1, cols1, matrix1, transposed1);
    print_matrix(cols1, rows1, transposed1);
    
    if (rows1 == rows2 && cols1 == cols2) {
        printf("\nMatrix addition result:\n");
        add_matrices(rows1, cols1, matrix1, matrix2, result);
        print_matrix(rows1, cols1, result);
    } else {
        printf("\nMatrices have different dimensions, cannot add\n");
    }
    
    if (cols1 == rows2) {
        printf("\nMatrix multiplication result:\n");
        multiply_matrices(rows1, cols1, matrix1, rows2, cols2, matrix2, result);
        print_matrix(