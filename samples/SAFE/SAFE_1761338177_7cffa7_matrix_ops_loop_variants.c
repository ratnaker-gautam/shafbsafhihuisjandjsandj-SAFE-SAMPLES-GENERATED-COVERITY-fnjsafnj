//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 10

void print_matrix(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void transpose_matrix(int rows, int cols, int src[MAX_SIZE][MAX_SIZE], int dest[MAX_SIZE][MAX_SIZE]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            dest[j][i] = src[i][j];
        }
    }
}

void multiply_matrices(int rows1, int cols1, int mat1[MAX_SIZE][MAX_SIZE], int rows2, int cols2, int mat2[MAX_SIZE][MAX_SIZE], int result[MAX_SIZE][MAX_SIZE]) {
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            result[i][j] = 0;
            for (int k = 0; k < cols1; k++) {
                if (__builtin_smul_overflow(mat1[i][k], mat2[k][j], &result[i][j])) {
                    printf("Integer overflow detected during multiplication.\n");
                    exit(1);
                }
                result[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }
}

int main() {
    int rows1, cols1, rows2, cols2;
    int matrix1[MAX_SIZE][MAX_SIZE];
    int matrix2[MAX_SIZE][MAX_SIZE];
    int result[MAX_SIZE][MAX_SIZE];
    
    printf("Enter dimensions of first matrix (rows columns, max %d): ", MAX_SIZE);
    if (scanf("%d %d", &rows1, &cols1) != 2 || rows1 <= 0 || rows1 > MAX_SIZE || cols1 <= 0 || cols1 > MAX_SIZE) {
        printf("Invalid matrix dimensions.\n");
        return 1;
    }
    
    printf("Enter elements of first matrix:\n");
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols1; j++) {
            if (scanf("%d", &matrix1[i][j]) != 1) {
                printf("Invalid input.\n");
                return 1;
            }
        }
    }
    
    printf("Enter dimensions of second matrix (rows columns, max %d): ", MAX_SIZE);
    if (scanf("%d %d", &rows2, &cols2) != 2 || rows2 <= 0 || rows2 > MAX_SIZE || cols2 <= 0 || cols2 > MAX_SIZE) {
        printf("Invalid matrix dimensions.\n");
        return 1;
    }
    
    printf("Enter elements of second matrix:\n");
    for (int i = 0; i < rows2; i++) {
        for (int j = 0; j < cols2; j++) {
            if (scanf("%d", &matrix2[i][j]) != 1) {
                printf("Invalid input.\n");
                return 1;
            }
        }
    }
    
    printf("\nFirst matrix:\n");
    print_matrix(rows1, cols1, matrix1);
    
    printf("\nSecond matrix:\n");
    print_matrix(rows2, cols2, matrix2);
    
    printf("\nTranspose of first matrix:\n");
    int transpose[MAX_SIZE][MAX_SIZE];
    transpose_matrix(rows1, cols1, matrix1, transpose);
    print_matrix(cols1, rows1, transpose);
    
    if (cols1 == rows2) {
        printf("\nMatrix multiplication result:\n");
        multiply_matrices(rows1, cols1, matrix1, rows2, cols2, matrix2, result);
        print_matrix(rows1, cols2, result);
    } else {
        printf("\nMatrices cannot be multiplied (columns of first must equal rows of second).\n");
    }
    
    return 0;
}