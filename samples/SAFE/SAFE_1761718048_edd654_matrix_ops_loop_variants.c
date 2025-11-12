//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 10

void print_matrix(int matrix[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void transpose_matrix(int src[MAX_SIZE][MAX_SIZE], int dest[MAX_SIZE][MAX_SIZE], int rows, int cols) {
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

void multiply_matrices(int a[MAX_SIZE][MAX_SIZE], int b[MAX_SIZE][MAX_SIZE], int result[MAX_SIZE][MAX_SIZE], int rows_a, int cols_a, int cols_b) {
    for (int i = 0; i < rows_a; i++) {
        for (int j = 0; j < cols_b; j++) {
            result[i][j] = 0;
            int k = 0;
            do {
                if (k >= cols_a) break;
                long long product = (long long)a[i][k] * b[k][j];
                if (product > INT_MAX || product < INT_MIN) {
                    fprintf(stderr, "Integer overflow detected\n");
                    exit(1);
                }
                result[i][j] += (int)product;
                k++;
            } while (k < cols_a);
        }
    }
}

int main() {
    int rows1, cols1, rows2, cols2;
    
    printf("Enter dimensions of first matrix (rows columns, max %d): ", MAX_SIZE);
    if (scanf("%d %d", &rows1, &cols1) != 2 || rows1 <= 0 || cols1 <= 0 || rows1 > MAX_SIZE || cols1 > MAX_SIZE) {
        fprintf(stderr, "Invalid matrix dimensions\n");
        return 1;
    }
    
    printf("Enter dimensions of second matrix (rows columns, max %d): ", MAX_SIZE);
    if (scanf("%d %d", &rows2, &cols2) != 2 || rows2 <= 0 || cols2 <= 0 || rows2 > MAX_SIZE || cols2 > MAX_SIZE) {
        fprintf(stderr, "Invalid matrix dimensions\n");
        return 1;
    }
    
    if (cols1 != rows2) {
        fprintf(stderr, "Matrix dimensions incompatible for multiplication\n");
        return 1;
    }
    
    int matrix1[MAX_SIZE][MAX_SIZE];
    int matrix2[MAX_SIZE][MAX_SIZE];
    
    printf("Enter elements of first matrix:\n");
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols1; j++) {
            if (scanf("%d", &matrix1[i][j]) != 1) {
                fprintf(stderr, "Invalid input\n");
                return 1;
            }
        }
    }
    
    printf("Enter elements of second matrix:\n");
    for (int i = 0; i < rows2; i++) {
        for (int j = 0; j < cols2; j++) {
            if (scanf("%d", &matrix2[i][j]) != 1) {
                fprintf(stderr, "Invalid input\n");
                return 1;
            }
        }
    }
    
    int transposed[MAX_SIZE][MAX_SIZE];
    transpose_matrix(matrix1, transposed, rows1, cols1);
    
    printf("\nOriginal matrix 1:\n");
    print_matrix(matrix1, rows1, cols1);
    
    printf("\nTransposed matrix 1:\n");
    print_matrix(transposed, cols1, rows1);
    
    int product[MAX_SIZE][MAX_SIZE];
    multiply_matrices(matrix1, matrix2, product, rows1, cols1, cols2);
    
    printf("\nMatrix product:\n");
    print_matrix(product, rows1, cols2);
    
    return 0;
}