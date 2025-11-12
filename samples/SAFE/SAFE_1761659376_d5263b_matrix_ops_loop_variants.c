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

void multiply_matrices(int r1, int c1, int m1[r1][c1], int r2, int c2, int m2[r2][c2], int result[r1][c2]) {
    if (c1 != r2) return;
    
    for (int i = 0; i < r1; i++) {
        for (int j = 0; j < c2; j++) {
            result[i][j] = 0;
            for (int k = 0; k < c1; k++) {
                if (m1[i][k] > 0 && m2[k][j] > 0) {
                    if (m1[i][k] > INT_MAX / m2[k][j]) continue;
                }
                result[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
}

int main(void) {
    int rows1, cols1, rows2, cols2;
    
    printf("Enter dimensions for first matrix (rows cols, max %d): ", MAX_SIZE);
    if (scanf("%d %d", &rows1, &cols1) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (rows1 <= 0 || cols1 <= 0 || rows1 > MAX_SIZE || cols1 > MAX_SIZE) {
        printf("Invalid dimensions\n");
        return 1;
    }
    
    printf("Enter dimensions for second matrix (rows cols, max %d): ", MAX_SIZE);
    if (scanf("%d %d", &rows2, &cols2) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (rows2 <= 0 || cols2 <= 0 || rows2 > MAX_SIZE || cols2 > MAX_SIZE) {
        printf("Invalid dimensions\n");
        return 1;
    }
    
    int matrix1[MAX_SIZE][MAX_SIZE];
    int matrix2[MAX_SIZE][MAX_SIZE];
    
    fill_matrix(rows1, cols1, matrix1);
    fill_matrix(rows2, cols2, matrix2);
    
    printf("\nMatrix 1:\n");
    print_matrix(rows1, cols1, matrix1);
    
    printf("\nMatrix 2:\n");
    print_matrix(rows2, cols2, matrix2);
    
    printf("\nTranspose of Matrix 1:\n");
    int transpose1[MAX_SIZE][MAX_SIZE];
    transpose_matrix(rows1, cols1, matrix1, transpose1);
    print_matrix(cols1, rows1, transpose1);
    
    if (cols1 == rows2) {
        printf("\nMatrix multiplication result:\n");
        int product[MAX_SIZE][MAX_SIZE];
        multiply_matrices(rows1, cols1, matrix1, rows2, cols2, matrix2, product);
        print_matrix(rows1, cols2, product);
    } else {
        printf("\nMatrices cannot be multiplied (columns of first != rows of second)\n");
    }
    
    int sum_diag1 = 0;
    int min_dim1 = (rows1 < cols1) ? rows1 : cols1;
    for (int i = 0; i < min_dim1; i++) {
        sum_diag1 += matrix1[i][i];
    }
    printf("\nSum of main diagonal (Matrix 1): %d\n", sum_diag1);
    
    int row_sum2[MAX_SIZE] = {0};
    for (int i = 0; i < rows2; i++) {
        int j = 0;
        do {
            row_sum2[i] += matrix2[i][j];
            j++;
        } while (j < cols2);
    }
    
    printf("Row sums (Matrix 2): ");
    for (int i = 0; i < rows2; i++) {
        printf("%d ", row_sum2[i]);
    }
    printf("\n");
    
    return 0;
}