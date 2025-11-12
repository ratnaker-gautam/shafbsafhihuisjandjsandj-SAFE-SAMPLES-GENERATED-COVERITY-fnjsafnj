//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ROWS 10
#define MAX_COLS 10

int main(void) {
    int rows, cols;
    printf("Enter matrix dimensions (rows columns): ");
    if (scanf("%d %d", &rows, &cols) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    if (rows <= 0 || cols <= 0 || rows > MAX_ROWS || cols > MAX_COLS) {
        printf("Invalid dimensions\n");
        return 1;
    }

    int matrix[MAX_ROWS][MAX_COLS];
    printf("Enter %d elements:\n", rows * cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scanf("%d", &matrix[i][j]) != 1) {
                printf("Invalid input\n");
                return 1;
            }
        }
    }

    printf("Original matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }

    int transposed[MAX_COLS][MAX_ROWS];
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            transposed[i][j] = matrix[j][i];
        }
    }

    printf("Transposed matrix:\n");
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            printf("%d ", transposed[i][j]);
        }
        printf("\n");
    }

    if (rows == cols) {
        int trace = 0;
        for (int i = 0; i < rows; i++) {
            trace += matrix[i][i];
        }
        printf("Matrix trace: %d\n", trace);
    } else {
        printf("Matrix is not square, cannot compute trace\n");
    }

    int row_sums[MAX_ROWS] = {0};
    int col_sums[MAX_COLS] = {0};
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            row_sums[i] += matrix[i][j];
            col_sums[j] += matrix[i][j];
        }
    }

    printf("Row sums: ");
    for (int i = 0; i < rows; i++) {
        printf("%d ", row_sums[i]);
    }
    printf("\n");

    printf("Column sums: ");
    for (int j = 0; j < cols; j++) {
        printf("%d ", col_sums[j]);
    }
    printf("\n");

    int scalar;
    printf("Enter scalar for multiplication: ");
    if (scanf("%d", &scalar) != 1) {
        printf("Invalid input\n");
        return 1;
    }

    printf("Scaled matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j] * scalar);
        }
        printf("\n");
    }

    return 0;
}