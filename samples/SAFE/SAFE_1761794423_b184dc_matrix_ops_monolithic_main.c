//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 10

int main(void) {
    int rows, cols;
    printf("Enter matrix dimensions (rows columns, max %dx%d): ", MAX_SIZE, MAX_SIZE);
    if (scanf("%d %d", &rows, &cols) != 2) {
        printf("Invalid input format\n");
        return 1;
    }
    if (rows <= 0 || cols <= 0 || rows > MAX_SIZE || cols > MAX_SIZE) {
        printf("Invalid dimensions\n");
        return 1;
    }

    int matrix1[MAX_SIZE][MAX_SIZE];
    int matrix2[MAX_SIZE][MAX_SIZE];
    int result[MAX_SIZE][MAX_SIZE];

    printf("Enter elements of first matrix (%dx%d):\n", rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scanf("%d", &matrix1[i][j]) != 1) {
                printf("Invalid matrix element\n");
                return 1;
            }
        }
    }

    printf("Enter elements of second matrix (%dx%d):\n", rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scanf("%d", &matrix2[i][j]) != 1) {
                printf("Invalid matrix element\n");
                return 1;
            }
        }
    }

    printf("Matrix addition result:\n");
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

    printf("Matrix multiplication result:\n");
    if (rows != cols) {
        printf("Multiplication requires square matrices\n");
        return 0;
    }

    int mul_result[MAX_SIZE][MAX_SIZE];
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            mul_result[i][j] = 0;
            for (int k = 0; k < cols; k++) {
                long long product = (long long)matrix1[i][k] * matrix2[k][j];
                if (product > INT_MAX || product < INT_MIN) {
                    printf("Integer overflow in multiplication\n");
                    return 1;
                }
                if ((matrix2[k][j] > 0 && mul_result[i][j] > INT_MAX - (int)product) ||
                    (matrix2[k][j] < 0 && mul_result[i][j] < INT_MIN - (int)product)) {
                    printf("Integer overflow in multiplication sum\n");
                    return 1;
                }
                mul_result[i][j] += (int)product;
            }
            printf("%6d", mul_result[i][j]);
        }
        printf("\n");
    }

    printf("Matrix transpose of first matrix:\n");
    int transpose[MAX_SIZE][MAX_SIZE];
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            transpose[i][j] = matrix1[j][i];
            printf("%6d", transpose[i][j]);
        }
        printf("\n");
    }

    return 0;
}