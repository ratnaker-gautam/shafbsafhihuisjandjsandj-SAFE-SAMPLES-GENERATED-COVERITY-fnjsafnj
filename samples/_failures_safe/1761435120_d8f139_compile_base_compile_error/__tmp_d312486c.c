//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
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

void read_matrix(int matrix[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    printf("Enter %d elements for %d x %d matrix:\n", rows * cols, rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scanf("%d", &matrix[i][j]) != 1) {
                printf("Invalid input\n");
                exit(1);
            }
        }
    }
}

void add_matrices(int a[MAX_SIZE][MAX_SIZE], int b[MAX_SIZE][MAX_SIZE], 
                  int result[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    int i = 0;
    while (i < rows) {
        int j = 0;
        do {
            if ((a[i][j] > 0 && b[i][j] > INT_MAX - a[i][j]) ||
                (a[i][j] < 0 && b[i][j] < INT_MIN - a[i][j])) {
                printf("Integer overflow detected\n");
                exit(1);
            }
            result[i][j] = a[i][j] + b[i][j];
            j++;
        } while (j < cols);
        i++;
    }
}

void multiply_matrices(int a[MAX_SIZE][MAX_SIZE], int b[MAX_SIZE][MAX_SIZE],
                      int result[MAX_SIZE][MAX_SIZE], int rows, int cols, int k) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = 0;
            int m = 0;
            while (m < k) {
                if ((a[i][m] > 0 && b[m][j] > 0 && a[i][m] > INT_MAX / b[m][j]) ||
                    (a[i][m] < 0 && b[m][j] < 0 && a[i][m] < INT_MAX / b[m][j]) ||
                    (a[i][m] > 0 && b[m][j] < 0 && b[m][j] < INT_MIN / a[i][m]) ||
                    (a[i][m] < 0 && b[m][j] > 0 && a[i][m] < INT_MIN / b[m][j])) {
                    printf("Integer overflow detected\n");
                    exit(1);
                }
                int product = a[i][m] * b[m][j];
                if ((product > 0 && result[i][j] > INT_MAX - product) ||
                    (product < 0 && result[i][j] < INT_MIN - product)) {
                    printf("Integer overflow detected\n");
                    exit(1);
                }
                result[i][j] += product;
                m++;
            }
        }
    }
}

void transpose_matrix(int matrix[MAX_SIZE][MAX_SIZE], int result[MAX_SIZE][MAX_SIZE], 
                     int rows, int cols) {
    int i = 0;
    do {
        int j = 0;
        while (j < cols) {
            result[j][i] = matrix[i][j];
            j++;
        }
        i++;
    } while (i < rows);
}

int main() {
    int rows1, cols1, rows2, cols2;
    int matrix1[MAX_SIZE][MAX_SIZE];
    int matrix2[MAX_SIZE][MAX_SIZE];
    int result[MAX_SIZE][MAX_SIZE];
    
    printf("Enter dimensions for first matrix (rows cols, max %d): ", MAX_SIZE);
    if (scanf("%d %d", &rows1, &cols1) != 2 || rows1 <= 0 || rows1 > MAX_SIZE || 
        cols1 <= 0 || cols1 > MAX_SIZE) {
        printf("Invalid dimensions\n");
        return 1;
    }
    
    read_matrix(matrix1, rows1, cols1);
    
    printf("Enter dimensions for second matrix (rows cols, max %d): ", MAX_SIZE);
    if (scanf("%d %d", &rows2, &cols2) != 2 || rows2 <= 0 || rows2 > MAX_SIZE || 
        cols2 <= 0 || cols2 > MAX_SIZE) {
        printf("Invalid dimensions\n");
        return 1;
    }
    
    read_matrix(matrix2, rows2, cols2);
    
    printf("\nMatrix 1:\n");
    print_matrix(matrix1, rows1, cols1);
    
    printf("\nMatrix 2:\n");
    print_matrix(matrix2, rows2, cols2);
    
    if (rows1 == rows2 && cols1 == cols2) {
        printf("\nMatrix Addition:\n");
        add_matrices(matrix1, matrix2, result, rows1, cols1);
        print_matrix(result, rows1, cols1);
    } else {
        printf("\nMatrices cannot be added (dimensions differ)\n");
    }
    
    if (cols1 == rows2) {
        printf("\nMatrix Multiplication:\n");
        multiply_matrices(matrix1, matrix2, result, rows1, cols2, cols1);