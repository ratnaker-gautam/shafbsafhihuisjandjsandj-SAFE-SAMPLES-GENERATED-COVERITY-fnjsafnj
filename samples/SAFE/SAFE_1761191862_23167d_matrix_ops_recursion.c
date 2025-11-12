//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 10

int is_valid_matrix_size(int rows, int cols) {
    return rows > 0 && rows <= MAX_SIZE && cols > 0 && cols <= MAX_SIZE;
}

void read_matrix(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Enter element [%d][%d]: ", i, j);
            if (scanf("%d", &matrix[i][j]) != 1) {
                printf("Invalid input\n");
                exit(1);
            }
        }
    }
}

void print_matrix(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int recursive_trace(int matrix[MAX_SIZE][MAX_SIZE], int size, int index) {
    if (index >= size) {
        return 0;
    }
    if (index < 0 || index >= MAX_SIZE) {
        return 0;
    }
    return matrix[index][index] + recursive_trace(matrix, size, index + 1);
}

int recursive_sum(int matrix[MAX_SIZE][MAX_SIZE], int rows, int cols, int i, int j) {
    if (i >= rows) {
        return 0;
    }
    if (j >= cols) {
        return recursive_sum(matrix, rows, cols, i + 1, 0);
    }
    if (i < 0 || i >= MAX_SIZE || j < 0 || j >= MAX_SIZE) {
        return 0;
    }
    return matrix[i][j] + recursive_sum(matrix, rows, cols, i, j + 1);
}

void recursive_transpose(int src[MAX_SIZE][MAX_SIZE], int dst[MAX_SIZE][MAX_SIZE], int rows, int cols, int i, int j) {
    if (i >= rows) {
        return;
    }
    if (j >= cols) {
        recursive_transpose(src, dst, rows, cols, i + 1, 0);
        return;
    }
    if (i < 0 || i >= MAX_SIZE || j < 0 || j >= MAX_SIZE) {
        return;
    }
    dst[j][i] = src[i][j];
    recursive_transpose(src, dst, rows, cols, i, j + 1);
}

int main() {
    int rows, cols;
    printf("Enter number of rows (1-%d): ", MAX_SIZE);
    if (scanf("%d", &rows) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    printf("Enter number of columns (1-%d): ", MAX_SIZE);
    if (scanf("%d", &cols) != 1) {
        printf("Invalid input\n");
        return 1;
    }

    if (!is_valid_matrix_size(rows, cols)) {
        printf("Invalid matrix size\n");
        return 1;
    }

    int matrix[MAX_SIZE][MAX_SIZE];
    printf("Enter matrix elements:\n");
    read_matrix(rows, cols, matrix);

    printf("\nOriginal matrix:\n");
    print_matrix(rows, cols, matrix);

    int total_sum = recursive_sum(matrix, rows, cols, 0, 0);
    printf("\nSum of all elements: %d\n", total_sum);

    if (rows == cols) {
        int trace = recursive_trace(matrix, rows, 0);
        printf("Matrix trace: %d\n", trace);
    } else {
        printf("Matrix is not square, cannot compute trace\n");
    }

    int transposed[MAX_SIZE][MAX_SIZE];
    recursive_transpose(matrix, transposed, rows, cols, 0, 0);
    printf("\nTransposed matrix:\n");
    print_matrix(cols, rows, transposed);

    return 0;
}