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
            printf("%4d", matrix[i][j]);
        }
        printf("\n");
    }
}

void read_matrix(int matrix[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    printf("Enter %d elements for %dx%d matrix:\n", rows * cols, rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int valid = 0;
            while (!valid) {
                printf("Element [%d][%d]: ", i, j);
                char buffer[32];
                if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                    printf("Input error\n");
                    exit(1);
                }
                char *endptr;
                long val = strtol(buffer, &endptr, 10);
                if (endptr == buffer || *endptr != '\n') {
                    printf("Invalid input. Enter integer: ");
                } else if (val < INT_MIN || val > INT_MAX) {
                    printf("Value out of range. Enter integer between %d and %d: ", INT_MIN, INT_MAX);
                } else {
                    matrix[i][j] = (int)val;
                    valid = 1;
                }
            }
        }
    }
}

void multiply_matrices(int a[MAX_SIZE][MAX_SIZE], int b[MAX_SIZE][MAX_SIZE], 
                      int result[MAX_SIZE][MAX_SIZE], int m, int n, int p) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < p; j++) {
            result[i][j] = 0;
            int k = 0;
            while (k < n) {
                if (a[i][k] > 0 && b[k][j] > 0 && a[i][k] > INT_MAX / b[k][j]) {
                    printf("Multiplication overflow detected\n");
                    exit(1);
                }
                if (a[i][k] < 0 && b[k][j] < 0 && a[i][k] < INT_MAX / b[k][j]) {
                    printf("Multiplication overflow detected\n");
                    exit(1);
                }
                if (a[i][k] > 0 && b[k][j] < 0 && a[i][k] > INT_MIN / b[k][j]) {
                    printf("Multiplication overflow detected\n");
                    exit(1);
                }
                if (a[i][k] < 0 && b[k][j] > 0 && a[i][k] < INT_MIN / b[k][j]) {
                    printf("Multiplication overflow detected\n");
                    exit(1);
                }
                int product = a[i][k] * b[k][j];
                if ((product > 0 && result[i][j] > INT_MAX - product) ||
                    (product < 0 && result[i][j] < INT_MIN - product)) {
                    printf("Addition overflow detected\n");
                    exit(1);
                }
                result[i][j] += product;
                k++;
            }
        }
    }
}

int main(void) {
    int rows1, cols1, rows2, cols2;
    
    printf("Enter dimensions for first matrix (rows columns, max %d): ", MAX_SIZE);
    if (scanf("%d %d", &rows1, &cols1) != 2) {
        printf("Invalid dimensions\n");
        return 1;
    }
    if (rows1 <= 0 || rows1 > MAX_SIZE || cols1 <= 0 || cols1 > MAX_SIZE) {
        printf("Invalid dimensions. Must be 1-%d\n", MAX_SIZE);
        return 1;
    }
    
    printf("Enter dimensions for second matrix (rows columns, max %d): ", MAX_SIZE);
    if (scanf("%d %d", &rows2, &cols2) != 2) {
        printf("Invalid dimensions\n");
        return 1;
    }
    if (rows2 <= 0 || rows2 > MAX_SIZE || cols2 <= 0 || cols2 > MAX_SIZE) {
        printf("Invalid dimensions. Must be 1-%d\n", MAX_SIZE);
        return 1;
    }
    
    if (cols1 != rows2) {
        printf("Matrix dimensions incompatible for multiplication\n");
        return 1;
    }
    
    while (getchar() != '\n');
    
    int matrix1[MAX_SIZE][MAX_SIZE];
    int matrix2[MAX_SIZE][MAX_SIZE];
    int result[MAX_SIZE][MAX_SIZE];
    
    printf("\nFirst matrix:\n");
    read_matrix(matrix1, rows1, cols1);
    
    printf("\nSecond matrix:\n");
    read_matrix(matrix2, rows2, cols2);
    
    multiply_matrices(matrix1, matrix2, result, rows1, cols1, cols2);
    
    printf("\nFirst matrix:\n");
    print_matrix(matrix1, rows1, cols1);
    
    printf("\nSecond matrix:\n");
    print_matrix(matrix2, rows2, cols2);
    
    printf("\nResult of multiplication:\n");
    print_matrix(result, rows1, cols2);
    
    return 0;
}