//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
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

void add_matrices(int a[MAX_SIZE][MAX_SIZE], int b[MAX_SIZE][MAX_SIZE], 
                 int result[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    int i = 0;
    while (i < rows) {
        int j = 0;
        do {
            if ((a[i][j] > 0 && b[i][j] > INT_MAX - a[i][j]) ||
                (a[i][j] < 0 && b[i][j] < INT_MIN - a[i][j])) {
                printf("Error: Integer overflow detected\n");
                return;
            }
            result[i][j] = a[i][j] + b[i][j];
            j++;
        } while (j < cols);
        i++;
    }
}

void multiply_matrices(int a[MAX_SIZE][MAX_SIZE], int b[MAX_SIZE][MAX_SIZE],
                      int result[MAX_SIZE][MAX_SIZE], int rows, int cols, int inner) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = 0;
            int k = 0;
            while (k < inner) {
                long long product = (long long)a[i][k] * b[k][j];
                if (product > INT_MAX || product < INT_MIN) {
                    printf("Error: Integer overflow in multiplication\n");
                    return;
                }
                long long sum = (long long)result[i][j] + product;
                if (sum > INT_MAX || sum < INT_MIN) {
                    printf("Error: Integer overflow in summation\n");
                    return;
                }
                result[i][j] = (int)sum;
                k++;
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
    
    printf("Enter dimensions of first matrix (rows columns, max %d): ", MAX_SIZE);
    if (scanf("%d %d", &rows1, &cols1) != 2 || rows1 <= 0 || rows1 > MAX_SIZE || 
        cols1 <= 0 || cols1 > MAX_SIZE) {
        printf("Invalid matrix dimensions\n");
        return 1;
    }
    
    printf("Enter elements of first matrix:\n");
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols1; j++) {
            if (scanf("%d", &matrix1[i][j]) != 1) {
                printf("Invalid input\n");
                return 1;
            }
        }
    }
    
    printf("Enter dimensions of second matrix (rows columns, max %d): ", MAX_SIZE);
    if (scanf("%d %d", &rows2, &cols2) != 2 || rows2 <= 0 || rows2 > MAX_SIZE || 
        cols2 <= 0 || cols2 > MAX_SIZE) {
        printf("Invalid matrix dimensions\n");
        return 1;
    }
    
    printf("Enter elements of second matrix:\n");
    for (int i = 0; i < rows2; i++) {
        for (int j = 0; j < cols2; j++) {
            if (scanf("%d", &matrix2[i][j]) != 1) {
                printf("Invalid input\n");
                return 1;
            }
        }
    }
    
    printf("\nMatrix 1:\n");
    print_matrix(matrix1, rows1, cols1);
    
    printf("\nMatrix 2:\n");
    print_matrix(matrix2, rows2, cols2);
    
    printf("\nMatrix Addition:\n");
    if (rows1 == rows2 && cols1 == cols2) {
        add_matrices(matrix1, matrix2, result, rows1, cols1);
        print_matrix(result, rows1, cols1);
    } else {
        printf("Matrices must have same dimensions for addition\n");
    }
    
    printf("\nMatrix Multiplication:\n");
    if (cols1 == rows2) {
        multiply_matrices(matrix1, matrix2, result, rows1, cols2, cols1);
        print_matrix(result, rows1, cols2);
    } else {
        printf("Matrices incompatible for multiplication\n");
    }
    
    printf("\nTranspose of Matrix 1:\n");
    transpose_matrix(matrix1, result, rows1, cols1);
    print_matrix(result, cols1, rows1);
    
    printf("\nTranspose of Matrix 2:\n");
    transpose_matrix(matrix2, result, rows2, cols2);
    print_matrix(result, cols