//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 10

int validate_matrix_size(int rows, int cols) {
    return rows > 0 && rows <= MAX_SIZE && cols > 0 && cols <= MAX_SIZE;
}

void recursive_matrix_multiply(int **a, int **b, int **result, int size, int i, int j, int k) {
    if (k >= size) return;
    result[i][j] += a[i][k] * b[k][j];
    recursive_matrix_multiply(a, b, result, size, i, j, k + 1);
}

void recursive_fill_row(int **matrix, int rows, int cols, int row, int col) {
    if (col >= cols) return;
    matrix[row][col] = rand() % 10;
    recursive_fill_row(matrix, rows, cols, row, col + 1);
}

void recursive_fill_matrix(int **matrix, int rows, int cols, int row) {
    if (row >= rows) return;
    recursive_fill_row(matrix, rows, cols, row, 0);
    recursive_fill_matrix(matrix, rows, cols, row + 1);
}

void recursive_print_row(int **matrix, int cols, int row, int col) {
    if (col >= cols) return;
    printf("%d ", matrix[row][col]);
    recursive_print_row(matrix, cols, row, col + 1);
}

void recursive_print_matrix(int **matrix, int rows, int cols, int row) {
    if (row >= rows) return;
    recursive_print_row(matrix, cols, row, 0);
    printf("\n");
    recursive_print_matrix(matrix, rows, cols, row + 1);
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
    }
    return matrix;
}

void free_matrix(int **matrix, int rows) {
    for (int i = 0; i < rows; i++) free(matrix[i]);
    free(matrix);
}

void recursive_zero_matrix(int **matrix, int rows, int cols, int row, int col) {
    if (row >= rows) return;
    if (col >= cols) {
        recursive_zero_matrix(matrix, rows, cols, row + 1, 0);
        return;
    }
    matrix[row][col] = 0;
    recursive_zero_matrix(matrix, rows, cols, row, col + 1);
}

int main() {
    int rows, cols;
    printf("Enter matrix size (rows cols, max %d): ", MAX_SIZE);
    if (scanf("%d %d", &rows, &cols) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (!validate_matrix_size(rows, cols)) {
        printf("Invalid matrix size\n");
        return 1;
    }
    
    srand(42);
    
    int **matrix1 = allocate_matrix(rows, cols);
    int **matrix2 = allocate_matrix(rows, cols);
    int **result = allocate_matrix(rows, cols);
    
    if (!matrix1 || !matrix2 || !result) {
        printf("Memory allocation failed\n");
        if (matrix1) free_matrix(matrix1, rows);
        if (matrix2) free_matrix(matrix2, rows);
        if (result) free_matrix(result, rows);
        return 1;
    }
    
    recursive_zero_matrix(result, rows, cols, 0, 0);
    recursive_fill_matrix(matrix1, rows, cols, 0);
    recursive_fill_matrix(matrix2, rows, cols, 0);
    
    printf("Matrix 1:\n");
    recursive_print_matrix(matrix1, rows, cols, 0);
    printf("Matrix 2:\n");
    recursive_print_matrix(matrix2, rows, cols, 0);
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            recursive_matrix_multiply(matrix1, matrix2, result, cols, i, j, 0);
        }
    }
    
    printf("Result (matrix multiplication):\n");
    recursive_print_matrix(result, rows, cols, 0);
    
    free_matrix(matrix1, rows);
    free_matrix(matrix2, rows);
    free_matrix(result, rows);
    
    return 0;
}