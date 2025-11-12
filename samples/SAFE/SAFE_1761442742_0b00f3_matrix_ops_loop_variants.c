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
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%4d", matrix[i][j]);
        }
        printf("\n");
    }
}

void transpose_matrix(int rows, int cols, int src[MAX_SIZE][MAX_SIZE], int dst[MAX_SIZE][MAX_SIZE]) {
    int i = 0;
    while (i < rows) {
        int j = 0;
        while (j < cols) {
            dst[j][i] = src[i][j];
            j++;
        }
        i++;
    }
}

void multiply_matrices(int r1, int c1, int m1[MAX_SIZE][MAX_SIZE], int r2, int c2, int m2[MAX_SIZE][MAX_SIZE], int result[MAX_SIZE][MAX_SIZE]) {
    for (int i = 0; i < r1; i++) {
        for (int j = 0; j < c2; j++) {
            result[i][j] = 0;
            for (int k = 0; k < c1; k++) {
                if (m1[i][k] > 0 && m2[k][j] > 0) {
                    if (m1[i][k] > INT_MAX / m2[k][j]) {
                        result[i][j] = 0;
                        break;
                    }
                }
                result[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
}

int main() {
    int rows1, cols1, rows2, cols2;
    
    printf("Enter dimensions for first matrix (rows columns, max %d): ", MAX_SIZE);
    if (scanf("%d %d", &rows1, &cols1) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (rows1 <= 0 || rows1 > MAX_SIZE || cols1 <= 0 || cols1 > MAX_SIZE) {
        printf("Invalid dimensions\n");
        return 1;
    }
    
    printf("Enter dimensions for second matrix (rows columns, max %d): ", MAX_SIZE);
    if (scanf("%d %d", &rows2, &cols2) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (rows2 <= 0 || rows2 > MAX_SIZE || cols2 <= 0 || cols2 > MAX_SIZE) {
        printf("Invalid dimensions\n");
        return 1;
    }
    
    int matrix1[MAX_SIZE][MAX_SIZE];
    int matrix2[MAX_SIZE][MAX_SIZE];
    int result[MAX_SIZE][MAX_SIZE];
    
    fill_matrix(rows1, cols1, matrix1);
    fill_matrix(rows2, cols2, matrix2);
    
    printf("\nMatrix 1:\n");
    print_matrix(rows1, cols1, matrix1);
    
    printf("\nMatrix 2:\n");
    print_matrix(rows2, cols2, matrix2);
    
    printf("\nTranspose of Matrix 1:\n");
    int transposed[MAX_SIZE][MAX_SIZE];
    transpose_matrix(rows1, cols1, matrix1, transposed);
    print_matrix(cols1, rows1, transposed);
    
    if (cols1 == rows2) {
        printf("\nMatrix multiplication (Matrix 1 * Matrix 2):\n");
        multiply_matrices(rows1, cols1, matrix1, rows2, cols2, matrix2, result);
        print_matrix(rows1, cols2, result);
    } else {
        printf("\nMatrices cannot be multiplied (columns of first must equal rows of second)\n");
    }
    
    return 0;
}