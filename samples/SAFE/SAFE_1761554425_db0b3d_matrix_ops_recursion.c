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

int recursive_determinant(int matrix[MAX_SIZE][MAX_SIZE], int size) {
    if (size == 1) {
        return matrix[0][0];
    }
    if (size == 2) {
        return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    }
    int det = 0;
    int sign = 1;
    for (int col = 0; col < size; col++) {
        int submatrix[MAX_SIZE][MAX_SIZE];
        int sub_i = 0;
        for (int i = 1; i < size; i++) {
            int sub_j = 0;
            for (int j = 0; j < size; j++) {
                if (j == col) continue;
                submatrix[sub_i][sub_j] = matrix[i][j];
                sub_j++;
            }
            sub_i++;
        }
        det += sign * matrix[0][col] * recursive_determinant(submatrix, size - 1);
        sign = -sign;
    }
    return det;
}

int main() {
    int size;
    if (scanf("%d", &size) != 1) {
        printf("Invalid size\n");
        return 1;
    }
    if (size <= 0 || size > MAX_SIZE) {
        printf("Size out of range\n");
        return 1;
    }
    int matrixA[MAX_SIZE][MAX_SIZE];
    int matrixB[MAX_SIZE][MAX_SIZE];
    read_matrix(matrixA, size);
    read_matrix(matrixB, size);
    printf("Matrix A:\n");
    print_matrix(matrixA, size);
    printf("Matrix B:\n");
    print_matrix(matrixB, size);
    int transposed[MAX_SIZE][MAX_SIZE];
    memcpy(transposed, matrixA, sizeof(transposed));
    recursive_transpose(transposed, size, 0, 0);
    printf("Transpose of A:\n");
    print_matrix(transposed, size);
    int product[MAX_SIZE][MAX_SIZE] = {0};
    recursive_multiply(matrixA, matrixB, product, size, 0, 0, 0, 0);
    printf("Product of A and B:\n");
    print_matrix(product, size);
    int detA = recursive_determinant(matrixA, size);
    printf("Determinant of A: %d\n", detA);
    return 0;
}