//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: monolithic_main ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ROWS 10
#define MAX_COLS 10

int main(void) {
    int rows, cols;
    printf("Enter matrix dimensions (rows cols, max %dx%d): ", MAX_ROWS, MAX_COLS);
    if (scanf("%d %d", &rows, &cols) != 2) {
        printf("Invalid input format\n");
        return 1;
    }
    if (rows <= 0 || cols <= 0 || rows > MAX_ROWS || cols > MAX_COLS) {
        printf("Invalid dimensions\n");
        return 1;
    }

    int matrix1[MAX_ROWS][MAX_COLS];
    int matrix2[MAX_ROWS][MAX_COLS];
    int result[MAX_ROWS][MAX_COLS];

    printf("Enter elements for first matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scanf("%d", &matrix1[i][j]) != 1) {
                printf("Invalid matrix element\n");
                return 1;
            }
        }
    }

    printf("Enter elements for second matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scanf("%d", &matrix2[i][j]) != 1) {
                printf("Invalid matrix element\n");
                return 1;
            }
        }
    }

    printf("Matrix addition:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if ((matrix2[i][j] > 0 && matrix1[i][j] > INT_MAX - matrix2[i][j]) ||
                (matrix2[i][j] < 0 && matrix1[i][j] < INT_MIN - matrix2[i][j])) {
                printf("Integer overflow detected\n");
                return 1;
            }
            result[i][j] = matrix1[i][j] + matrix2[i][j];
            printf("%6d", result[i][j]);
        }
        printf("\n");
    }

    printf("Matrix subtraction:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if ((matrix2[i][j] > 0 && matrix1[i][j] < INT_MIN + matrix2[i][j]) ||
                (matrix2[i][j] < 0 && matrix1[i][j] > INT_MAX + matrix2[i][j])) {
                printf("Integer overflow detected\n");
                return 1;
            }
            result[i][j] = matrix1[i][j] - matrix2[i][j];
            printf("%6d", result[i][j]);
        }
        printf("\n");
    }

    if (rows == cols) {
        printf("Matrix multiplication:\n");
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                result[i][j] = 0;
                for (int k = 0; k < cols; k++) {
                    if (matrix1[i][k] != 0 && matrix2[k][j] != 0) {
                        if ((matrix1[i][k] > 0 && matrix2[k][j] > 0 && matrix1[i][k] > INT_MAX / matrix2[k][j]) ||
                            (matrix1[i][k] > 0 && matrix2[k][j] < 0 && matrix2[k][j] < INT_MIN / matrix1[i][k]) ||
                            (matrix1[i][k] < 0 && matrix2[k][j] > 0 && matrix1[i][k] < INT_MIN / matrix2[k][j]) ||
                            (matrix1[i][k] < 0 && matrix2[k][j] < 0 && matrix1[i][k] < INT_MAX / matrix2[k][j])) {
                            printf("Integer overflow detected\n");
                            return 1;
                        }
                    }
                    int product = matrix1[i][k] * matrix2[k][j];
                    if ((product > 0 && result[i][j] > INT_MAX - product) ||
                        (product < 0 && result[i][j] < INT_MIN - product)) {
                        printf("Integer overflow detected\n");
                        return 1;
                    }
                    result[i][j] += product;
                }
                printf("%6d", result[i][j]);
            }
            printf("\n");
        }
    } else {
        printf("Matrix multiplication requires square matrices\n");
    }

    printf("Matrix transpose:\n");
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            printf("%6d", matrix1[j][i]);
        }
        printf("\n");
    }

    return 0;
}