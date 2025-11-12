//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 10

void print_matrix(int matrix[MAX_SIZE][MAX_SIZE], int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void fill_matrix_recursive(int matrix[MAX_SIZE][MAX_SIZE], int size, int row, int col, int value) {
    if (row >= size) return;
    if (col >= size) {
        fill_matrix_recursive(matrix, size, row + 1, 0, value);
        return;
    }
    matrix[row][col] = value;
    fill_matrix_recursive(matrix, size, row, col + 1, value + 1);
}

void transpose_recursive(int src[MAX_SIZE][MAX_SIZE], int dst[MAX_SIZE][MAX_SIZE], int size, int row, int col) {
    if (row >= size) return;
    if (col >= size) {
        transpose_recursive(src, dst, size, row + 1, 0);
        return;
    }
    dst[col][row] = src[row][col];
    transpose_recursive(src, dst, size, row, col + 1);
}

int trace_recursive(int matrix[MAX_SIZE][MAX_SIZE], int size, int idx, int sum) {
    if (idx >= size) return sum;
    return trace_recursive(matrix, size, idx + 1, sum + matrix[idx][idx]);
}

int main() {
    int size;
    printf("Enter matrix size (1-%d): ", MAX_SIZE);
    if (scanf("%d", &size) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    if (size < 1 || size > MAX_SIZE) {
        printf("Size out of range\n");
        return 1;
    }

    int matrix[MAX_SIZE][MAX_SIZE];
    fill_matrix_recursive(matrix, size, 0, 0, 1);

    printf("Original matrix:\n");
    print_matrix(matrix, size);

    int transposed[MAX_SIZE][MAX_SIZE];
    transpose_recursive(matrix, transposed, size, 0, 0);

    printf("Transposed matrix:\n");
    print_matrix(transposed, size);

    int trace = trace_recursive(matrix, size, 0, 0);
    printf("Matrix trace: %d\n", trace);

    return 0;
}