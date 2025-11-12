//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 10

void print_matrix(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%4d", matrix[i][j]);
        }
        printf("\n");
    }
}

void read_matrix(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
    printf("Enter %d elements for %dx%d matrix:\n", rows * cols, rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            while (1) {
                printf("Element [%d][%d]: ", i, j);
                char buffer[32];
                if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                    printf("Input error\n");
                    exit(1);
                }
                char *endptr;
                long val = strtol(buffer, &endptr, 10);
                if (endptr == buffer || *endptr != '\n') {
                    printf("Invalid input. Enter integer: ");
                    continue;
                }
                if (val < INT_MIN || val > INT_MAX) {
                    printf("Value out of range. Try again: ");
                    continue;
                }
                matrix[i][j] = (int)val;
                break;
            }
        }
    }
}

void add_matrices(int rows, int cols, int a[MAX_SIZE][MAX_SIZE], int b[MAX_SIZE][MAX_SIZE], int result[MAX_SIZE][MAX_SIZE]) {
    int i = 0;
    while (i < rows) {
        int j = 0;
        do {
            if ((a[i][j] > 0 && b[i][j] > INT_MAX - a[i][j]) ||
                (a[i][j] < 0 && b[i][j] < INT_MIN - a[i][j])) {
                printf("Integer overflow in addition\n");
                exit(1);
            }
            result[i][j] = a[i][j] + b[i][j];
            j++;
        } while (j < cols);
        i++;
    }
}

void multiply_matrices(int rows1, int cols1, int a[MAX_SIZE][MAX_SIZE], int rows2, int cols2, int b[MAX_SIZE][MAX_SIZE], int result[MAX_SIZE][MAX_SIZE]) {
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            result[i][j] = 0;
            int k = 0;
            while (k < cols1) {
                long product = (long)a[i][k] * b[k][j];
                if (product > INT_MAX || product < INT_MIN) {
                    printf("Integer overflow in multiplication\n");
                    exit(1);
                }
                long sum = (long)result[i][j] + product;
                if (sum > INT_MAX || sum < INT_MIN) {
                    printf("Integer overflow in summation\n");
                    exit(1);
                }
                result[i][j] = (int)sum;
                k++;
            }
        }
    }
}

void transpose_matrix(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE], int result[MAX_SIZE][MAX_SIZE]) {
    int i = 0;
    do {
        int j = 0;
        while (j < cols) {
            result[j][i] = matrix[i][j];
            j++;
        }
        i++;
    } while (i < rows);
}

int main() {
    int rows1, cols1, rows2, cols2;
    int matrix1[MAX_SIZE][MAX_SIZE];
    int matrix2[MAX_SIZE][MAX_SIZE];
    int result[MAX_SIZE][MAX_SIZE];

    printf("Enter dimensions for first matrix (rows cols, max %d): ", MAX_SIZE);
    if (scanf("%d %d", &rows1, &cols1) != 2 || rows1 <= 0 || cols1 <= 0 || rows1 > MAX_SIZE || cols1 > MAX_SIZE) {
        printf("Invalid dimensions\n");
        return 1;
    }
    while (getchar() != '\n');

    read_matrix(rows1, cols1, matrix1);

    printf("Enter dimensions for second matrix (rows cols, max %d): ", MAX_SIZE);
    if (scanf("%d %d", &rows2, &cols2) != 2 || rows2 <= 0 || cols2 <= 0 || rows2 > MAX_SIZE || cols2 > MAX_SIZE) {
        printf("Invalid dimensions\n");
        return 1;
    }
    while (getchar() != '\n');

    read_matrix(rows2, cols2, matrix2);

    printf("\nMatrix 1:\n");
    print_matrix(rows1, cols1, matrix1);

    printf("\nMatrix 2:\n");
    print_matrix(rows2, cols2, matrix2);

    if (rows1 == rows2 && cols1 == cols2) {
        printf("\nMatrix Addition:\n");
        add_matrices(rows1, cols1, matrix1, matrix2, result);
        print_matrix(rows1, cols1, result);
    } else {
        printf("\nMatrices cannot be added (dimensions differ)\n");
    }

    if (cols1 == rows2) {
        printf("\nMatrix Multiplication:\n");
        multiply