//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: matrix_ops
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

void fill_matrix(int rows, int cols, int matrix[][MAX_SIZE]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = rand() % 10;
        }
    }
}

void print_matrix(int rows, int cols, int matrix[][MAX_SIZE]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%4d", matrix[i][j]);
        }
        printf("\n");
    }
}

int main() {
    srand(time(NULL));
    
    int rows_a, cols_a, rows_b, cols_b;
    
    printf("Enter dimensions for matrix A (rows columns): ");
    if (scanf("%d %d", &rows_a, &cols_a) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("Enter dimensions for matrix B (rows columns): ");
    if (scanf("%d %d", &rows_b, &cols_b) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (rows_a <= 0 || cols_a <= 0 || rows_b <= 0 || cols_b <= 0 ||
        rows_a > MAX_SIZE || cols_a > MAX_SIZE || 
        rows_b > MAX_SIZE || cols_b > MAX_SIZE) {
        printf("Invalid matrix dimensions\n");
        return 1;
    }
    
    if (cols_a != rows_b) {
        printf("Matrix dimensions incompatible for multiplication\n");
        return 1;
    }
    
    int matrix_a[MAX_SIZE][MAX_SIZE];
    int matrix_b[MAX_SIZE][MAX_SIZE];
    int result[MAX_SIZE][MAX_SIZE];
    
    fill_matrix(rows_a, cols_a, matrix_a);
    fill_matrix(rows_b, cols_b, matrix_b);
    
    printf("\nMatrix A:\n");
    print_matrix(rows_a, cols_a, matrix_a);
    
    printf("\nMatrix B:\n");
    print_matrix(rows_b, cols_b, matrix_b);
    
    matrix_multiply(rows_a, cols_a, rows_b, cols_b, matrix_a, matrix_b, result);
    
    printf("\nMatrix A * B:\n");
    print_matrix(rows_a, cols_b, result);
    
    int transposed[MAX_SIZE][MAX_SIZE];
    matrix_transpose(rows_a, cols_b, result, transposed);
    
    printf("\nTranspose of A * B:\n");
    print_matrix(cols_b, rows_a, transposed);
    
    return 0;
}