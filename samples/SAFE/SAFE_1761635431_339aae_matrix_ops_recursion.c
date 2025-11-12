//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 10

void read_matrix(int matrix[MAX_SIZE][MAX_SIZE], int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (scanf("%d", &matrix[i][j]) != 1) {
                printf("Invalid input\n");
                exit(1);
            }
        }
    }
}

void print_matrix(int matrix[MAX_SIZE][MAX_SIZE], int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void recursive_transpose(int matrix[MAX_SIZE][MAX_SIZE], int size, int row, int col) {
    if (row >= size) {
        return;
    }
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

void recursive_trace(int matrix[MAX_SIZE][MAX_SIZE], int size, int row, int *sum) {
    if (row >= size) {
        return;
    }
    if (row < size) {
        *sum += matrix[row][row];
    }
    recursive_trace(matrix, size, row + 1, sum);
}

int main() {
    int size;
    if (scanf("%d", &size) != 1) {
        printf("Invalid size\n");
        return 1;
    }
    if (size <= 0 || size > MAX_SIZE) {
        printf("Size must be between 1 and %d\n", MAX_SIZE);
        return 1;
    }

    int matrix[MAX_SIZE][MAX_SIZE];
    read_matrix(matrix, size);

    printf("Original matrix:\n");
    print_matrix(matrix, size);

    int trace = 0;
    recursive_trace(matrix, size, 0, &trace);
    printf("Trace: %d\n", trace);

    recursive_transpose(matrix, size, 0, 0);
    printf("Transposed matrix:\n");
    print_matrix(matrix, size);

    return 0;
}