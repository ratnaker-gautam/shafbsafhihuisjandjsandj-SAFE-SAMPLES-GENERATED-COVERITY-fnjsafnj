//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 10

int read_matrix(int matrix[MAX_SIZE][MAX_SIZE], int *rows, int *cols) {
    if (scanf("%d %d", rows, cols) != 2) return 0;
    if (*rows <= 0 || *rows > MAX_SIZE) return 0;
    if (*cols <= 0 || *cols > MAX_SIZE) return 0;
    
    for (int i = 0; i < *rows; i++) {
        for (int j = 0; j < *cols; j++) {
            if (scanf("%d", &matrix[i][j]) != 1) return 0;
        }
    }
    return 1;
}

void print_matrix(int matrix[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int recursive_sum_row(int matrix[MAX_SIZE][MAX_SIZE], int row, int col, int current_col) {
    if (current_col >= col) return 0;
    return matrix[row][current_col] + recursive_sum_row(matrix, row, col, current_col + 1);
}

int recursive_sum_matrix(int matrix[MAX_SIZE][MAX_SIZE], int rows, int cols, int current_row) {
    if (current_row >= rows) return 0;
    return recursive_sum_row(matrix, current_row, cols, 0) + 
           recursive_sum_matrix(matrix, rows, cols, current_row + 1);
}

void recursive_transpose(int src[MAX_SIZE][MAX_SIZE], int dest[MAX_SIZE][MAX_SIZE], 
                        int rows, int cols, int i, int j) {
    if (i >= rows) return;
    if (j >= cols) {
        recursive_transpose(src, dest, rows, cols, i + 1, 0);
        return;
    }
    dest[j][i] = src[i][j];
    recursive_transpose(src, dest, rows, cols, i, j + 1);
}

int main() {
    int matrix_a[MAX_SIZE][MAX_SIZE];
    int matrix_b[MAX_SIZE][MAX_SIZE];
    int rows, cols;
    
    printf("Enter matrix dimensions (rows cols): ");
    if (!read_matrix(matrix_a, &rows, &cols)) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("\nOriginal matrix:\n");
    print_matrix(matrix_a, rows, cols);
    
    int total_sum = recursive_sum_matrix(matrix_a, rows, cols, 0);
    printf("\nSum of all elements: %d\n", total_sum);
    
    printf("\nRow sums:\n");
    for (int i = 0; i < rows; i++) {
        int row_sum = recursive_sum_row(matrix_a, i, cols, 0);
        printf("Row %d: %d\n", i, row_sum);
    }
    
    if (rows <= MAX_SIZE && cols <= MAX_SIZE) {
        recursive_transpose(matrix_a, matrix_b, rows, cols, 0, 0);
        printf("\nTransposed matrix:\n");
        print_matrix(matrix_b, cols, rows);
    }
    
    return 0;
}