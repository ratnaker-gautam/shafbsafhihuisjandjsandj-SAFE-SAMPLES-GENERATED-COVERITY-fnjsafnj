//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: matrix_ops
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

void recursive_transpose(int matrix[MAX_SIZE][MAX_SIZE], int size, int row, int col) {
    if (row >= size) return;
    if (col >= size) {
        recursive_transpose(matrix, size, row + 1, 0);
        return;
    }
    if (row < col) {
        int temp = matrix[row][col];
        matrix[row][col] = matrix[col][row];
        matrix[col][row] = temp;
    }
    recursive_transpose(matrix, size, row, col + 1);
}

void recursive_fill(int matrix[MAX_SIZE][MAX_SIZE], int size, int row, int col, int value) {
    if (row >= size) return;
    if (col >= size) {
        recursive_fill(matrix, size, row + 1, 0, value);
        return;
    }
    matrix[row][col] = value;
    recursive_fill(matrix, size, row, col + 1, value + 1);
}

int recursive_trace(int matrix[MAX_SIZE][MAX_SIZE], int size, int index, int sum) {
    if (index >= size) return sum;
    return recursive_trace(matrix, size, index + 1, sum + matrix[index][index]);
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
    recursive_fill(matrix, size, 0, 0, 1);

    printf("Original matrix:\n");
    print_matrix(matrix, size);

    int trace = recursive_trace(matrix, size, 0, 0);
    printf("Matrix trace: %d\n", trace);

    recursive_transpose(matrix, size, 0, 0);
    printf("Transposed matrix:\n");
    print_matrix(matrix, size);

    return 0;
}