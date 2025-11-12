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

int recursive_sum(int matrix[MAX_SIZE][MAX_SIZE], int rows, int cols, int r, int c) {
    if (r >= rows) return 0;
    if (c >= cols) return recursive_sum(matrix, rows, cols, r + 1, 0);
    return matrix[r][c] + recursive_sum(matrix, rows, cols, r, c + 1);
}

void recursive_transpose(int src[MAX_SIZE][MAX_SIZE], int dst[MAX_SIZE][MAX_SIZE], int rows, int cols, int r, int c) {
    if (r >= rows) return;
    if (c >= cols) {
        recursive_transpose(src, dst, rows, cols, r + 1, 0);
        return;
    }
    dst[c][r] = src[r][c];
    recursive_transpose(src, dst, rows, cols, r, c + 1);
}

int main() {
    int matrix_a[MAX_SIZE][MAX_SIZE];
    int matrix_b[MAX_SIZE][MAX_SIZE];
    int rows, cols;

    printf("Enter matrix dimensions (rows cols) and elements:\n");
    if (!read_matrix(matrix_a, &rows, &cols)) {
        printf("Invalid input\n");
        return 1;
    }

    printf("Original matrix:\n");
    print_matrix(matrix_a, rows, cols);

    int sum = recursive_sum(matrix_a, rows, cols, 0, 0);
    printf("Sum of all elements: %d\n", sum);

    if (rows == cols) {
        int trace = recursive_trace(matrix_a, rows, 0);
        printf("Matrix trace: %d\n", trace);
    } else {
        printf("Matrix is not square, cannot compute trace\n");
    }

    printf("Transposed matrix:\n");
    recursive_transpose(matrix_a, matrix_b, rows, cols, 0, 0);
    print_matrix(matrix_b, cols, rows);

    return 0;
}