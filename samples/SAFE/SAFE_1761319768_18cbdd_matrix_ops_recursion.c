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

void recursive_transpose(int matrix[MAX_SIZE][MAX_SIZE], int transposed[MAX_SIZE][MAX_SIZE], int row, int col, int size) {
    if (row >= size) {
        return;
    }
    if (col >= size) {
        recursive_transpose(matrix, transposed, row + 1, 0, size);
        return;
    }
    transposed[col][row] = matrix[row][col];
    recursive_transpose(matrix, transposed, row, col + 1, size);
}

int recursive_trace(int matrix[MAX_SIZE][MAX_SIZE], int index, int size) {
    if (index >= size) {
        return 0;
    }
    return matrix[index][index] + recursive_trace(matrix, index + 1, size);
}

int main() {
    int size;
    printf("Enter matrix size (1-%d): ", MAX_SIZE);
    if (scanf("%d", &size) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    if (size < 1 || size > MAX_SIZE) {
        printf("Size must be between 1 and %d\n", MAX_SIZE);
        return 1;
    }

    int matrix[MAX_SIZE][MAX_SIZE];
    printf("Enter %dx%d matrix elements:\n", size, size);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (scanf("%d", &matrix[i][j]) != 1) {
                printf("Invalid input\n");
                return 1;
            }
        }
    }

    printf("\nOriginal matrix:\n");
    print_matrix(matrix, size);

    int transposed[MAX_SIZE][MAX_SIZE];
    recursive_transpose(matrix, transposed, 0, 0, size);
    printf("\nTransposed matrix:\n");
    print_matrix(transposed, size);

    int trace = recursive_trace(matrix, 0, size);
    printf("\nMatrix trace: %d\n", trace);

    return 0;
}