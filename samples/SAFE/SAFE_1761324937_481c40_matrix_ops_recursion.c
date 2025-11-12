//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 10

int read_matrix(int matrix[MAX_SIZE][MAX_SIZE], int *rows, int *cols) {
    if (scanf("%d %d", rows, cols) != 2) return 0;
    if (*rows <= 0 || *cols <= 0 || *rows > MAX_SIZE || *cols > MAX_SIZE) return 0;
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

void recursive_sum_rows(int matrix[MAX_SIZE][MAX_SIZE], int rows, int cols, int current_row, int sums[MAX_SIZE]) {
    if (current_row >= rows) return;
    sums[current_row] = recursive_sum_row(matrix, current_row, cols, 0);
    recursive_sum_rows(matrix, rows, cols, current_row + 1, sums);
}

int recursive_sum_matrix(int matrix[MAX_SIZE][MAX_SIZE], int rows, int cols, int current_row, int current_col) {
    if (current_row >= rows) return 0;
    if (current_col >= cols) return recursive_sum_matrix(matrix, rows, cols, current_row + 1, 0);
    return matrix[current_row][current_col] + recursive_sum_matrix(matrix, rows, cols, current_row, current_col + 1);
}

int recursive_transpose(int matrix[MAX_SIZE][MAX_SIZE], int transposed[MAX_SIZE][MAX_SIZE], int rows, int cols, int current_row, int current_col) {
    if (current_row >= rows) return 1;
    if (current_col >= cols) return recursive_transpose(matrix, transposed, rows, cols, current_row + 1, 0);
    if (current_col >= MAX_SIZE || current_row >= MAX_SIZE) return 0;
    transposed[current_col][current_row] = matrix[current_row][current_col];
    return recursive_transpose(matrix, transposed, rows, cols, current_row, current_col + 1);
}

int main() {
    int matrix[MAX_SIZE][MAX_SIZE];
    int rows, cols;
    
    printf("Enter matrix dimensions (rows cols): ");
    if (!read_matrix(matrix, &rows, &cols)) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("Original matrix:\n");
    print_matrix(matrix, rows, cols);
    
    int row_sums[MAX_SIZE] = {0};
    recursive_sum_rows(matrix, rows, cols, 0, row_sums);
    printf("Row sums: ");
    for (int i = 0; i < rows; i++) {
        printf("%d ", row_sums[i]);
    }
    printf("\n");
    
    int total_sum = recursive_sum_matrix(matrix, rows, cols, 0, 0);
    printf("Total sum: %d\n", total_sum);
    
    if (rows <= MAX_SIZE && cols <= MAX_SIZE) {
        int transposed[MAX_SIZE][MAX_SIZE];
        if (recursive_transpose(matrix, transposed, rows, cols, 0, 0)) {
            printf("Transposed matrix:\n");
            print_matrix(transposed, cols, rows);
        }
    }
    
    return 0;
}