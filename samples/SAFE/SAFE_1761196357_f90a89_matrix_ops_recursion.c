//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 10

void printMatrix(int matrix[MAX_SIZE][MAX_SIZE], int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void recursiveTranspose(int matrix[MAX_SIZE][MAX_SIZE], int size, int row, int col) {
    if (row >= size) {
        return;
    }
    if (col >= size) {
        recursiveTranspose(matrix, size, row + 1, row + 1);
        return;
    }
    if (row != col && row < col) {
        int temp = matrix[row][col];
        matrix[row][col] = matrix[col][row];
        matrix[col][row] = temp;
    }
    recursiveTranspose(matrix, size, row, col + 1);
}

void recursiveFill(int matrix[MAX_SIZE][MAX_SIZE], int size, int row, int col) {
    if (row >= size) {
        return;
    }
    if (col >= size) {
        recursiveFill(matrix, size, row + 1, 0);
        return;
    }
    matrix[row][col] = row * size + col + 1;
    recursiveFill(matrix, size, row, col + 1);
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
    recursiveFill(matrix, size, 0, 0);

    printf("Original matrix:\n");
    printMatrix(matrix, size);

    recursiveTranspose(matrix, size, 0, 0);

    printf("Transposed matrix:\n");
    printMatrix(matrix, size);

    return 0;
}