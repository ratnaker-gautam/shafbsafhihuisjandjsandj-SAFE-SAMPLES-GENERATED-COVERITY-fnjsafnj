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

int recursive_trace(int matrix[MAX_SIZE][MAX_SIZE], int size, int index) {
    if (index >= size) return 0;
    return matrix[index][index] + recursive_trace(matrix, size, index + 1);
}

int recursive_sum_row(int matrix[MAX_SIZE][MAX_SIZE], int row, int cols, int index) {
    if (index >= cols) return 0;
    return matrix[row][index] + recursive_sum_row(matrix, row, cols, index + 1);
}

int recursive_sum_col(int matrix[MAX_SIZE][MAX_SIZE], int rows, int col, int index) {
    if (index >= rows) return 0;
    return matrix[index][col] + recursive_sum_col(matrix, rows, col, index + 1);
}

void recursive_transpose(int src[MAX_SIZE][MAX_SIZE], int dest[MAX_SIZE][MAX_SIZE], int rows, int cols, int i, int j) {
    if (i >= rows) return;
    if (j >= cols) {
        recursive_transpose(src, dest, rows, cols, i + 1, 0);
        return;
    }
    dest[j][i] = src[i][j];
    recursive_transpose(src, dest, rows, cols, i, j + 1);
}

int main() {
    int matrix[MAX_SIZE][MAX_SIZE];
    int rows, cols;
    
    printf("Enter matrix dimensions (rows cols): ");
    if (!read_matrix(matrix, &rows, &cols)) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("\nOriginal matrix:\n");
    print_matrix(matrix, rows, cols);
    
    if (rows == cols) {
        int trace = recursive_trace(matrix, rows, 0);
        printf("\nMatrix trace: %d\n", trace);
    }
    
    printf("\nRow sums:\n");
    for (int i = 0; i < rows; i++) {
        int sum = recursive_sum_row(matrix, i, cols, 0);
        printf("Row %d: %d\n", i, sum);
    }
    
    printf("\nColumn sums:\n");
    for (int i = 0; i < cols; i++) {
        int sum = recursive_sum_col(matrix, rows, i, 0);
        printf("Column %d: %d\n", i, sum);
    }
    
    if (rows <= MAX_SIZE && cols <= MAX_SIZE) {
        int transposed[MAX_SIZE][MAX_SIZE];
        recursive_transpose(matrix, transposed, rows, cols, 0, 0);
        printf("\nTransposed matrix:\n");
        print_matrix(transposed, cols, rows);
    }
    
    return 0;
}