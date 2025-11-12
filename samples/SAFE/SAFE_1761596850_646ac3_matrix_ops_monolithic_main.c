//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: matrix_ops
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
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        if (value < min || value > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        return value;
    }
}

void fill_matrix(int rows, int cols, int matrix[MAX_DIM][MAX_DIM]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = read_int("Enter matrix element: ", INT_MIN, INT_MAX);
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

void multiply_matrices(int r1, int c1, int m1[MAX_DIM][MAX_DIM], int r2, int c2, int m2[MAX_DIM][MAX_DIM], int result[MAX_DIM][MAX_DIM]) {
    for (int i = 0; i < r1; i++) {
        for (int j = 0; j < c2; j++) {
            result[i][j] = 0;
            for (int k = 0; k < c1; k++) {
                if (m1[i][k] > 0 && m2[k][j] > 0) {
                    if (m1[i][k] > INT_MAX / m2[k][j]) {
                        result[i][j] = INT_MAX;
                    } else {
                        result[i][j] += m1[i][k] * m2[k][j];
                    }
                } else if (m1[i][k] < 0 && m2[k][j] < 0) {
                    if (m1[i][k] < INT_MIN / m2[k][j]) {
                        result[i][j] = INT_MIN;
                    } else {
                        result[i][j] += m1[i][k] * m2[k][j];
                    }
                } else {
                    result[i][j] += m1[i][k] * m2[k][j];
                }
            }
        }
    }
}

int main() {
    int rows1, cols1, rows2, cols2;
    int matrix1[MAX_DIM][MAX_DIM];
    int matrix2[MAX_DIM][MAX_DIM];
    int result[MAX_DIM][MAX_DIM];

    printf("Matrix Multiplication Program\n");
    printf("First matrix dimensions:\n");
    rows1 = read_int("Rows: ", 1, MAX_DIM);
    cols1 = read_int("Columns: ", 1, MAX_DIM);

    printf("Second matrix dimensions:\n");
    rows2 = read_int("Rows: ", 1, MAX_DIM);
    cols2 = read_int("Columns: ", 1, MAX_DIM);

    if (cols1 != rows2) {
        printf("Error: Cannot multiply matrices. Columns of first must equal rows of second.\n");
        return 1;
    }

    printf("Enter first matrix elements:\n");
    fill_matrix(rows1, cols1, matrix1);

    printf("Enter second matrix elements:\n");
    fill_matrix(rows2, cols2, matrix2);

    printf("\nFirst matrix:\n");
    print_matrix(rows1, cols1, matrix1);

    printf("\nSecond matrix:\n");
    print_matrix(rows2, cols2, matrix2);

    multiply_matrices(rows1, cols1, matrix1, rows2, cols2, matrix2, result);

    printf("\nResult matrix:\n");
    print_matrix(rows1, cols2, result);

    return 0;
}