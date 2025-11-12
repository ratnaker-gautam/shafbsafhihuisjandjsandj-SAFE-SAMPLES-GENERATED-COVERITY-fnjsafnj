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

int recursive_trace(int matrix[MAX_SIZE][MAX_SIZE], int size, int index) {
    if (index >= size) {
        return 0;
    }
    return matrix[index][index] + recursive_trace(matrix, size, index + 1);
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
    printf("Enter %d x %d matrix elements:\n", size, size);
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

    printf("\nMatrix trace: %d\n", recursive_trace(matrix, size, 0));

    int temp_matrix[MAX_SIZE][MAX_SIZE];
    memcpy(temp_matrix, matrix, sizeof(temp_matrix));
    recursive_transpose(temp_matrix, size, 0, 0);
    printf("\nTransposed matrix:\n");
    print_matrix(temp_matrix, size);

    return 0;
}