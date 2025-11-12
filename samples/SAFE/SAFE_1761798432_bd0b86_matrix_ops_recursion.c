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

int recursive_transpose(int src[MAX_SIZE][MAX_SIZE], int dest[MAX_SIZE][MAX_SIZE], int row, int col, int rows, int cols) {
    if (row >= rows) return 1;
    if (col >= cols) return recursive_transpose(src, dest, row + 1, 0, rows, cols);
    if (row >= MAX_SIZE || col >= MAX_SIZE) return 0;
    dest[col][row] = src[row][col];
    return recursive_transpose(src, dest, row, col + 1, rows, cols);
}

int main() {
    int matrix[MAX_SIZE][MAX_SIZE];
    int transposed[MAX_SIZE][MAX_SIZE];
    int rows, cols;

    printf("Enter matrix dimensions (rows cols, max %d): ", MAX_SIZE);
    if (!read_matrix(matrix, &rows, &cols)) {
        printf("Invalid input\n");
        return 1;
    }

    printf("\nOriginal matrix:\n");
    print_matrix(matrix, rows, cols);

    int sum = recursive_sum(matrix, 0, 0, rows, cols);
    printf("\nSum of all elements: %d\n", sum);

    int max_val = recursive_max(matrix, 0, 0, rows, cols, matrix[0][0]);
    printf("Maximum element: %d\n", max_val);

    if (rows <= MAX_SIZE && cols <= MAX_SIZE) {
        memset(transposed, 0, sizeof(transposed));
        if (recursive_transpose(matrix, transposed, 0, 0, rows, cols)) {
            printf("\nTransposed matrix:\n");
            print_matrix(transposed, cols, rows);
        } else {
            printf("Transpose failed\n");
        }
    }

    return 0;
}