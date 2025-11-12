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

void recursive_multiply(int A[MAX_SIZE][MAX_SIZE], int B[MAX_SIZE][MAX_SIZE], int C[MAX_SIZE][MAX_SIZE], int size, int i, int j, int k, int sum) {
    if (i >= size) {
        return;
    }
    if (j >= size) {
        recursive_multiply(A, B, C, size, i + 1, 0, 0, 0);
        return;
    }
    if (k >= size) {
        C[i][j] = sum;
        recursive_multiply(A, B, C, size, i, j + 1, 0, 0);
        return;
    }
    sum += A[i][k] * B[k][j];
    recursive_multiply(A, B, C, size, i, j, k + 1, sum);
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

    int A[MAX_SIZE][MAX_SIZE];
    int B[MAX_SIZE][MAX_SIZE];
    int C[MAX_SIZE][MAX_SIZE];

    printf("Enter matrix A:\n");
    read_matrix(A, size);
    printf("Enter matrix B:\n");
    read_matrix(B, size);

    printf("Matrix A:\n");
    print_matrix(A, size);
    printf("Matrix B:\n");
    print_matrix(B, size);

    memset(C, 0, sizeof(C));
    recursive_multiply(A, B, C, size, 0, 0, 0, 0);
    printf("Matrix multiplication result:\n");
    print_matrix(C, size);

    recursive_transpose(A, size, 0, 0);
    printf("Transpose of A:\n");
    print_matrix(A, size);

    return 0;
}