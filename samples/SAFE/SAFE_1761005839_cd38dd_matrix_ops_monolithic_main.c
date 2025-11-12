//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: monolithic_main ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 10

int read_int(const char* prompt, int min, int max) {
    char buffer[32];
    int value;
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min;
        }
        if (sscanf(buffer, "%d", &value) == 1 && value >= min && value <= max) {
            return value;
        }
        printf("Invalid input. Enter integer between %d and %d.\n", min, max);
    }
}

void fill_matrix(int rows, int cols, int matrix[MAX_DIM][MAX_DIM]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = read_int("Enter element: ", -1000, 1000);
        }
    }
}

void print_matrix(int rows, int cols, int matrix[MAX_DIM][MAX_DIM]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d", matrix[i][j]);
        }
        printf("\n");
    }
}

void multiply_matrices(int r1, int c1, int m1[MAX_DIM][MAX_DIM],
                      int r2, int c2, int m2[MAX_DIM][MAX_DIM],
                      int result[MAX_DIM][MAX_DIM]) {
    for (int i = 0; i < r1; i++) {
        for (int j = 0; j < c2; j++) {
            result[i][j] = 0;
            for (int k = 0; k < c1; k++) {
                if (m1[i][k] > 0 && m2[k][j] > 0 && m1[i][k] > INT_MAX / m2[k][j]) {
                    result[i][j] = 0;
                    break;
                } else if (m1[i][k] < 0 && m2[k][j] < 0 && m1[i][k] < INT_MAX / m2[k][j]) {
                    result[i][j] = 0;
                    break;
                } else if (m1[i][k] > 0 && m2[k][j] < 0 && m1[i][k] > INT_MIN / m2[k][j]) {
                    result[i][j] = 0;
                    break;
                } else if (m1[i][k] < 0 && m2[k][j] > 0 && m1[i][k] < INT_MIN / m2[k][j]) {
                    result[i][j] = 0;
                    break;
                }
                int product = m1[i][k] * m2[k][j];
                if ((product > 0 && result[i][j] > INT_MAX - product) ||
                    (product < 0 && result[i][j] < INT_MIN - product)) {
                    result[i][j] = 0;
                    break;
                }
                result[i][j] += product;
            }
        }
    }
}

int main(void) {
    int rows1, cols1, rows2, cols2;
    int matrix1[MAX_DIM][MAX_DIM];
    int matrix2[MAX_DIM][MAX_DIM];
    int result[MAX_DIM][MAX_DIM];

    printf("Matrix Multiplication Program\n");
    printf("Matrix 1 dimensions:\n");
    rows1 = read_int("Rows: ", 1, MAX_DIM);
    cols1 = read_int("Columns: ", 1, MAX_DIM);

    printf("Matrix 2 dimensions:\n");
    rows2 = read_int("Rows: ", 1, MAX_DIM);
    cols2 = read_int("Columns: ", 1, MAX_DIM);

    if (cols1 != rows2) {
        printf("Error: Matrix dimensions incompatible for multiplication.\n");
        return 1;
    }

    printf("Enter elements for Matrix 1:\n");
    fill_matrix(rows1, cols1, matrix1);

    printf("Enter elements for Matrix 2:\n");
    fill_matrix(rows2, cols2, matrix2);

    printf("\nMatrix 1:\n");
    print_matrix(rows1, cols1, matrix1);

    printf("\nMatrix 2:\n");
    print_matrix(rows2, cols2, matrix2);

    multiply_matrices(rows1, cols1, matrix1, rows2, cols2, matrix2, result);

    printf("\nResult Matrix:\n");
    print_matrix(rows1, cols2, result);

    return 0;
}