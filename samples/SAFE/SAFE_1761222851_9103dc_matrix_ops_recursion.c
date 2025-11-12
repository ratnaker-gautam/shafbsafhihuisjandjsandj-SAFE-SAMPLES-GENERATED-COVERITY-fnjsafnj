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

int recursive_sum(int matrix[MAX_SIZE][MAX_SIZE], int row, int col, int rows, int cols) {
    if (row >= rows) return 0;
    if (col >= cols) return recursive_sum(matrix, row + 1, 0, rows, cols);
    return matrix[row][col] + recursive_sum(matrix, row, col + 1, rows, cols);
}

int recursive_max(int matrix[MAX_SIZE][MAX_SIZE], int row, int col, int rows, int cols, int current_max) {
    if (row >= rows) return current_max;
    if (col >= cols) return recursive_max(matrix, row + 1, 0, rows, cols, current_max);
    int new_max = (matrix[row][col] > current_max) ? matrix[row][col] : current_max;
    return recursive_max(matrix, row, col + 1, rows, cols, new_max);
}

void recursive_transpose(int src[MAX_SIZE][MAX_SIZE], int dest[MAX_SIZE][MAX_SIZE], int row, int col, int rows, int cols) {
    if (row >= rows) return;
    if (col >= cols) {
        recursive_transpose(src, dest, row + 1, 0, rows, cols);
        return;
    }
    dest[col][row] = src[row][col];
    recursive_transpose(src, dest, row, col + 1, rows, cols);
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

    printf("Original matrix:\n");
    print_matrix(matrix_a, rows, cols);

    int sum = recursive_sum(matrix_a, 0, 0, rows, cols);
    printf("Sum of all elements: %d\n", sum);

    int max_val = recursive_max(matrix_a, 0, 0, rows, cols, matrix_a[0][0]);
    printf("Maximum element: %d\n", max_val);

    if (rows <= MAX_SIZE && cols <= MAX_SIZE) {
        recursive_transpose(matrix_a, matrix_b, 0, 0, rows, cols);
        printf("Transposed matrix:\n");
        print_matrix(matrix_b, cols, rows);
    }

    return 0;
}