//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SIZE 100

void matrix_multiply(int rows_a, int cols_a, int rows_b, int cols_b, 
                     int a[][MAX_SIZE], int b[][MAX_SIZE], int result[][MAX_SIZE]) {
    for (int i = 0; i < rows_a; i++) {
        for (int j = 0; j < cols_b; j++) {
            result[i][j] = 0;
            for (int k = 0; k < cols_a; k++) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

void matrix_transpose(int rows, int cols, int matrix[][MAX_SIZE], int result[][MAX_SIZE]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[j][i] = matrix[i][j];
        }
    }
}

int main() {
    int rows_a, cols_a, rows_b, cols_b;
    int matrix_a[MAX_SIZE][MAX_SIZE];
    int matrix_b[MAX_SIZE][MAX_SIZE];
    int result[MAX_SIZE][MAX_SIZE];
    
    srand(time(NULL));
    
    printf("Enter dimensions for matrix A (rows columns): ");
    if (scanf("%d %d", &rows_a, &cols_a) != 2 || rows_a <= 0 || cols_a <= 0 || 
        rows_a > MAX_SIZE || cols_a > MAX_SIZE) {
        printf("Invalid matrix dimensions\n");
        return 1;
    }
    
    printf("Enter dimensions for matrix B (rows columns): ");
    if (scanf("%d %d", &rows_b, &cols_b) != 2 || rows_b <= 0 || cols_b <= 0 || 
        rows_b > MAX_SIZE || cols_b > MAX_SIZE) {
        printf("Invalid matrix dimensions\n");
        return 1;
    }
    
    if (cols_a != rows_b) {
        printf("Matrix dimensions incompatible for multiplication\n");
        return 1;
    }
    
    printf("Generating random matrices...\n");
    for (int i = 0; i < rows_a; i++) {
        for (int j = 0; j < cols_a; j++) {
            matrix_a[i][j] = rand() % 100;
        }
    }
    
    for (int i = 0; i < rows_b; i++) {
        for (int j = 0; j < cols_b; j++) {
            matrix_b[i][j] = rand() % 100;
        }
    }
    
    printf("Matrix A:\n");
    for (int i = 0; i < rows_a; i++) {
        for (int j = 0; j < cols_a; j++) {
            printf("%4d", matrix_a[i][j]);
        }
        printf("\n");
    }
    
    printf("Matrix B:\n");
    for (int i = 0; i < rows_b; i++) {
        for (int j = 0; j < cols_b; j++) {
            printf("%4d", matrix_b[i][j]);
        }
        printf("\n");
    }
    
    printf("Computing matrix multiplication...\n");
    matrix_multiply(rows_a, cols_a, rows_b, cols_b, matrix_a, matrix_b, result);
    
    printf("Result of A * B:\n");
    for (int i = 0; i < rows_a; i++) {
        for (int j = 0; j < cols_b; j++) {
            printf("%6d", result[i][j]);
        }
        printf("\n");
    }
    
    printf("Computing transpose of result...\n");
    int transposed[MAX_SIZE][MAX_SIZE];
    matrix_transpose(rows_a, cols_b, result, transposed);
    
    printf("Transpose of result:\n");
    for (int i = 0; i < cols_b; i++) {
        for (int j = 0; j < rows_a; j++) {
            printf("%6d", transposed[i][j]);
        }
        printf("\n");
    }
    
    return 0;
}