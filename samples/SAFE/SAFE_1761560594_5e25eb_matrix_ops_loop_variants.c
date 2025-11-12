//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 8

void fill_matrix(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
    int counter = 1;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = counter++;
        }
    }
}

void print_matrix(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
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

void transpose_matrix(int rows, int cols, int src[MAX_SIZE][MAX_SIZE], int dst[MAX_SIZE][MAX_SIZE]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            dst[j][i] = src[i][j];
        }
    }
}

void multiply_matrices(int r1, int c1, int m1[MAX_SIZE][MAX_SIZE], int r2, int c2, int m2[MAX_SIZE][MAX_SIZE], int result[MAX_SIZE][MAX_SIZE]) {
    for (int i = 0; i < r1; i++) {
        for (int j = 0; j < c2; j++) {
            int sum = 0;
            for (int k = 0; k < c1; k++) {
                if (m1[i][k] > 0 && m2[k][j] > 0) {
                    if (m1[i][k] > INT_MAX / m2[k][j]) {
                        sum = INT_MAX;
                        break;
                    }
                } else if (m1[i][k] < 0 && m2[k][j] < 0) {
                    if (m1[i][k] < INT_MAX / m2[k][j]) {
                        sum = INT_MAX;
                        break;
                    }
                }
                sum += m1[i][k] * m2[k][j];
            }
            result[i][j] = sum;
        }
    }
}

int main() {
    int rows1, cols1, rows2, cols2;
    
    printf("Enter dimensions for first matrix (rows cols, max %d): ", MAX_SIZE);
    if (scanf("%d %d", &rows1, &cols1) != 2 || rows1 <= 0 || cols1 <= 0 || rows1 > MAX_SIZE || cols1 > MAX_SIZE) {
        printf("Invalid dimensions\n");
        return 1;
    }
    
    printf("Enter dimensions for second matrix (rows cols, max %d): ", MAX_SIZE);
    if (scanf("%d %d", &rows2, &cols2) != 2 || rows2 <= 0 || cols2 <= 0 || rows2 > MAX_SIZE || cols2 > MAX_SIZE) {
        printf("Invalid dimensions\n");
        return 1;
    }
    
    if (cols1 != rows2) {
        printf("Matrix multiplication requires cols1 == rows2\n");
        return 1;
    }
    
    int matrix1[MAX_SIZE][MAX_SIZE];
    int matrix2[MAX_SIZE][MAX_SIZE];
    int result[MAX_SIZE][MAX_SIZE];
    int transposed[MAX_SIZE][MAX_SIZE];
    
    fill_matrix(rows1, cols1, matrix1);
    fill_matrix(rows2, cols2, matrix2);
    
    printf("\nMatrix 1:\n");
    print_matrix(rows1, cols1, matrix1);
    
    printf("\nMatrix 2:\n");
    print_matrix(rows2, cols2, matrix2);
    
    printf("\nTranspose of Matrix 1:\n");
    transpose_matrix(rows1, cols1, matrix1, transposed);
    print_matrix(cols1, rows1, transposed);
    
    printf("\nMatrix multiplication result:\n");
    multiply_matrices(rows1, cols1, matrix1, rows2, cols2, matrix2, result);
    print_matrix(rows1, cols2, result);
    
    int trace = 0;
    if (rows1 == cols1) {
        int i = 0;
        while (i < rows1) {
            trace += matrix1[i][i];
            i++;
        }
        printf("\nTrace of Matrix 1: %d\n", trace);
    }
    
    int row_sum[MAX_SIZE] = {0};
    for (int i = 0; i < rows1; i++) {
        int j = 0;
        do {
            row_sum[i] += matrix1[i][j];
            j++;
        } while (j < cols1);
    }
    
    printf("\nRow sums of Matrix 1:\n");
    int idx = 0;
    while (idx < rows1) {
        printf("Row %d: %d\n", idx, row_sum[idx]);
        idx++;
    }
    
    return 0;
}