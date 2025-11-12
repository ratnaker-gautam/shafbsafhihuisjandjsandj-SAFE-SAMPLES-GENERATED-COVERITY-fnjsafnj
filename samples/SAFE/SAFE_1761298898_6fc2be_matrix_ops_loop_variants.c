//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 8

void fill_matrix(int matrix[MAX_SIZE][MAX_SIZE], int size) {
    int value = 1;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrix[i][j] = value;
            value = (value + 1) % 10;
        }
    }
}

void print_matrix(int matrix[MAX_SIZE][MAX_SIZE], int size) {
    int i = 0;
    while (i < size) {
        int j = 0;
        do {
            printf("%3d", matrix[i][j]);
            j++;
        } while (j < size);
        printf("\n");
        i++;
    }
}

void transpose_matrix(int matrix[MAX_SIZE][MAX_SIZE], int size) {
    int temp[MAX_SIZE][MAX_SIZE];
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            temp[j][i] = matrix[i][j];
        }
    }
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrix[i][j] = temp[i][j];
        }
    }
}

void multiply_matrices(int a[MAX_SIZE][MAX_SIZE], int b[MAX_SIZE][MAX_SIZE], int result[MAX_SIZE][MAX_SIZE], int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            int sum = 0;
            for (int k = 0; k < size; k++) {
                if (sum > INT_MAX - a[i][k] * b[k][j]) {
                    printf("Integer overflow detected\n");
                    return;
                }
                sum += a[i][k] * b[k][j];
            }
            result[i][j] = sum;
        }
    }
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

    int matrix_a[MAX_SIZE][MAX_SIZE];
    int matrix_b[MAX_SIZE][MAX_SIZE];
    int result[MAX_SIZE][MAX_SIZE];

    fill_matrix(matrix_a, size);
    fill_matrix(matrix_b, size);

    printf("Matrix A:\n");
    print_matrix(matrix_a, size);

    printf("Matrix B:\n");
    print_matrix(matrix_b, size);

    printf("Transpose of A:\n");
    transpose_matrix(matrix_a, size);
    print_matrix(matrix_a, size);
    transpose_matrix(matrix_a, size);

    printf("Matrix multiplication A * B:\n");
    multiply_matrices(matrix_a, matrix_b, result, size);
    print_matrix(result, size);

    printf("Element-wise operations:\n");
    int i = 0;
    while (i < size) {
        int j = 0;
        do {
            printf("%3d", matrix_a[i][j] + matrix_b[i][j]);
            j++;
        } while (j < size);
        printf("\n");
        i++;
    }

    return 0;
}