//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 10

int is_valid_matrix_size(int rows, int cols) {
    return rows > 0 && rows <= MAX_SIZE && cols > 0 && cols <= MAX_SIZE;
}

void recursive_matrix_multiply(int **a, int **b, int **result, int rows_a, int cols_a, int cols_b, int i, int j, int k) {
    if (i >= rows_a) return;
    if (j >= cols_b) {
        recursive_matrix_multiply(a, b, result, rows_a, cols_a, cols_b, i + 1, 0, 0);
        return;
    }
    if (k >= cols_a) {
        recursive_matrix_multiply(a, b, result, rows_a, cols_a, cols_b, i, j + 1, 0);
        return;
    }
    
    result[i][j] += a[i][k] * b[k][j];
    recursive_matrix_multiply(a, b, result, rows_a, cols_a, cols_b, i, j, k + 1);
}

void recursive_matrix_print(int **matrix, int rows, int cols, int i, int j) {
    if (i >= rows) {
        printf("\n");
        return;
    }
    if (j >= cols) {
        printf("\n");
        recursive_matrix_print(matrix, rows, cols, i + 1, 0);
        return;
    }
    
    printf("%4d", matrix[i][j]);
    recursive_matrix_print(matrix, rows, cols, i, j + 1);
}

void recursive_matrix_fill(int **matrix, int rows, int cols, int i, int j) {
    if (i >= rows) return;
    if (j >= cols) {
        recursive_matrix_fill(matrix, rows, cols, i + 1, 0);
        return;
    }
    
    matrix[i][j] = (i * cols + j) % 10;
    recursive_matrix_fill(matrix, rows, cols, i, j + 1);
}

int **allocate_matrix(int rows, int cols) {
    int **matrix = malloc(rows * sizeof(int *));
    if (!matrix) return NULL;
    
    for (int i = 0; i < rows; i++) {
        matrix[i] = malloc(cols * sizeof(int));
        if (!matrix[i]) {
            for (int j = 0; j < i; j++) free(matrix[j]);
            free(matrix);
            return NULL;
        }
        memset(matrix[i], 0, cols * sizeof(int));
    }
    return matrix;
}

void free_matrix(int **matrix, int rows) {
    if (!matrix) return;
    for (int i = 0; i < rows; i++) free(matrix[i]);
    free(matrix);
}

int main() {
    int rows_a, cols_a, rows_b, cols_b;
    
    printf("Enter dimensions for matrix A (rows cols): ");
    if (scanf("%d %d", &rows_a, &cols_a) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (!is_valid_matrix_size(rows_a, cols_a)) {
        printf("Invalid matrix size\n");
        return 1;
    }
    
    printf("Enter dimensions for matrix B (rows cols): ");
    if (scanf("%d %d", &rows_b, &cols_b) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (!is_valid_matrix_size(rows_b, cols_b)) {
        printf("Invalid matrix size\n");
        return 1;
    }
    
    if (cols_a != rows_b) {
        printf("Matrix dimensions incompatible for multiplication\n");
        return 1;
    }
    
    int **matrix_a = allocate_matrix(rows_a, cols_a);
    int **matrix_b = allocate_matrix(rows_b, cols_b);
    int **result = allocate_matrix(rows_a, cols_b);
    
    if (!matrix_a || !matrix_b || !result) {
        printf("Memory allocation failed\n");
        if (matrix_a) free_matrix(matrix_a, rows_a);
        if (matrix_b) free_matrix(matrix_b, rows_b);
        if (result) free_matrix(result, rows_a);
        return 1;
    }
    
    recursive_matrix_fill(matrix_a, rows_a, cols_a, 0, 0);
    recursive_matrix_fill(matrix_b, rows_b, cols_b, 0, 0);
    
    printf("Matrix A:\n");
    recursive_matrix_print(matrix_a, rows_a, cols_a, 0, 0);
    
    printf("Matrix B:\n");
    recursive_matrix_print(matrix_b, rows_b, cols_b, 0, 0);
    
    recursive_matrix_multiply(matrix_a, matrix_b, result, rows_a, cols_a, cols_b, 0, 0, 0);
    
    printf("Result (A * B):\n");
    recursive_matrix_print(result, rows_a, cols_b, 0, 0);
    
    free_matrix(matrix_a, rows_a);
    free_matrix(matrix_b, rows_b);
    free_matrix(result, rows_a);
    
    return 0;
}