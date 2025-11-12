//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: matrix_ops
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

void recursive_sum_rows(int matrix[MAX_SIZE][MAX_SIZE], int rows, int cols, int current_row) {
    if (current_row >= rows) return;
    int sum = recursive_sum_row(matrix, current_row, cols, 0);
    printf("Row %d sum: %d\n", current_row, sum);
    recursive_sum_rows(matrix, rows, cols, current_row + 1);
}

int recursive_sum_col(int matrix[MAX_SIZE][MAX_SIZE], int rows, int col, int current_row) {
    if (current_row >= rows) return 0;
    return matrix[current_row][col] + recursive_sum_col(matrix, rows, col, current_row + 1);
}

void recursive_sum_cols(int matrix[MAX_SIZE][MAX_SIZE], int rows, int cols, int current_col) {
    if (current_col >= cols) return;
    int sum = recursive_sum_col(matrix, rows, current_col, 0);
    printf("Column %d sum: %d\n", current_col, sum);
    recursive_sum_cols(matrix, rows, cols, current_col + 1);
}

int recursive_trace(int matrix[MAX_SIZE][MAX_SIZE], int size, int index) {
    if (index >= size) return 0;
    return matrix[index][index] + recursive_trace(matrix, size, index + 1);
}

int main() {
    int matrix[MAX_SIZE][MAX_SIZE];
    int rows, cols;
    
    printf("Enter matrix dimensions (rows cols, max %dx%d): ", MAX_SIZE, MAX_SIZE);
    if (!read_matrix(matrix, &rows, &cols)) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("\nMatrix:\n");
    print_matrix(matrix, rows, cols);
    
    printf("\nRow sums:\n");
    recursive_sum_rows(matrix, rows, cols, 0);
    
    printf("\nColumn sums:\n");
    recursive_sum_cols(matrix, rows, cols, 0);
    
    if (rows == cols) {
        int trace = recursive_trace(matrix, rows, 0);
        printf("\nMatrix trace: %d\n", trace);
    } else {
        printf("\nNot a square matrix, no trace\n");
    }
    
    return 0;
}