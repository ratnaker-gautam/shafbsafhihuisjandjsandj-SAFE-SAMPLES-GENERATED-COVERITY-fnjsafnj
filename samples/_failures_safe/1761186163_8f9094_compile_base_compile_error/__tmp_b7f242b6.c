//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 10

void print_matrix(int matrix[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%4d", matrix[i][j]);
        }
        printf("\n");
    }
}

void read_matrix(int matrix[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    printf("Enter %d elements for %d x %d matrix:\n", rows * cols, rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int valid = 0;
            while (!valid) {
                printf("Element [%d][%d]: ", i, j);
                char input[32];
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    printf("Input error\n");
                    continue;
                }
                char *endptr;
                long val = strtol(input, &endptr, 10);
                if (endptr == input || *endptr != '\n') {
                    printf("Invalid input. Enter integer: ");
                    continue;
                }
                if (val < INT_MIN || val > INT_MAX) {
                    printf("Value out of range. Enter between %d and %d: ", INT_MIN, INT_MAX);
                    continue;
                }
                matrix[i][j] = (int)val;
                valid = 1;
            }
        }
    }
}

void add_matrices(int a[MAX_SIZE][MAX_SIZE], int b[MAX_SIZE][MAX_SIZE], int result[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    int i = 0;
    while (i < rows) {
        int j = 0;
        do {
            if ((a[i][j] > 0 && b[i][j] > INT_MAX - a[i][j]) || (a[i][j] < 0 && b[i][j] < INT_MIN - a[i][j])) {
                printf("Integer overflow detected at [%d][%d]\n", i, j);
                result[i][j] = 0;
            } else {
                result[i][j] = a[i][j] + b[i][j];
            }
            j++;
        } while (j < cols);
        i++;
    }
}

void multiply_matrices(int a[MAX_SIZE][MAX_SIZE], int b[MAX_SIZE][MAX_SIZE], int result[MAX_SIZE][MAX_SIZE], int rows1, int cols1, int cols2) {
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            result[i][j] = 0;
            int k = 0;
            while (k < cols1) {
                long product = (long)a[i][k] * b[k][j];
                if (product > INT_MAX || product < INT_MIN) {
                    printf("Integer overflow detected at [%d][%d]\n", i, j);
                    result[i][j] = 0;
                    break;
                }
                long sum = (long)result[i][j] + product;
                if (sum > INT_MAX || sum < INT_MIN) {
                    printf("Integer overflow detected at [%d][%d]\n", i, j);
                    result[i][j] = 0;
                    break;
                }
                result[i][j] = (int)sum;
                k++;
            }
        }
    }
}

int main() {
    int rows1, cols1, rows2, cols2;
    int matrix1[MAX_SIZE][MAX_SIZE];
    int matrix2[MAX_SIZE][MAX_SIZE];
    int result[MAX_SIZE][MAX_SIZE];

    printf("Matrix Operations Program\n");
    printf("Enter dimensions for first matrix (rows columns, max %d): ", MAX_SIZE);
    if (scanf("%d %d", &rows1, &cols1) != 2 || rows1 <= 0 || rows1 > MAX_SIZE || cols1 <= 0 || cols1 > MAX_SIZE) {
        printf("Invalid dimensions\n");
        return 1;
    }
    while (getchar() != '\n');

    printf("Enter dimensions for second matrix (rows columns, max %d): ", MAX_SIZE);
    if (scanf("%d %d", &rows2, &cols2) != 2 || rows2 <= 0 || rows2 > MAX_SIZE || cols2 <= 0 || cols2 > MAX_SIZE) {
        printf("Invalid dimensions\n");
        return 1;
    }
    while (getchar() != '\n');

    printf("\nFirst matrix:\n");
    read_matrix(matrix1, rows1, cols1);
    printf("\nSecond matrix:\n");
    read_matrix(matrix2, rows2, cols2);

    printf("\nFirst matrix:\n");
    print_matrix(matrix1, rows1, cols1);
    printf("\nSecond matrix:\n");
    print_matrix(matrix2, rows2, cols2);

    if (rows1 == rows2 && cols1 == cols2) {
        printf("\nMatrix addition:\n");
        add_matrices(matrix1, matrix2, result, rows1, cols1);
        print_matrix(result, rows1, cols1);
    } else {
        printf("\nMatrices have different dimensions, cannot add\n");
    }

    if (cols1 == rows2) {