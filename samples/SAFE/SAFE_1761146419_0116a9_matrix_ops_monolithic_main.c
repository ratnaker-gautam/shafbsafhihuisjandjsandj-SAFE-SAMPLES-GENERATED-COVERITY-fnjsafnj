//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 10

int main(void) {
    int rows, cols;
    printf("Enter matrix dimensions (rows columns, max %dx%d): ", MAX_DIM, MAX_DIM);
    if (scanf("%d %d", &rows, &cols) != 2) {
        printf("Invalid input format\n");
        return 1;
    }
    if (rows <= 0 || cols <= 0 || rows > MAX_DIM || cols > MAX_DIM) {
        printf("Invalid dimensions\n");
        return 1;
    }

    int matrix1[MAX_DIM][MAX_DIM];
    int matrix2[MAX_DIM][MAX_DIM];
    int result[MAX_DIM][MAX_DIM];

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

    printf("Choose operation:\n");
    printf("1. Add matrices\n");
    printf("2. Subtract matrices\n");
    printf("3. Multiply matrices\n");
    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("Invalid choice\n");
        return 1;
    }

    switch (choice) {
        case 1:
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    if ((matrix1[i][j] > 0 && matrix2[i][j] > INT_MAX - matrix1[i][j]) ||
                        (matrix1[i][j] < 0 && matrix2[i][j] < INT_MIN - matrix1[i][j])) {
                        printf("Integer overflow in addition\n");
                        return 1;
                    }
                    result[i][j] = matrix1[i][j] + matrix2[i][j];
                }
            }
            break;
        case 2:
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    if ((matrix2[i][j] > 0 && matrix1[i][j] < INT_MIN + matrix2[i][j]) ||
                        (matrix2[i][j] < 0 && matrix1[i][j] > INT_MAX + matrix2[i][j])) {
                        printf("Integer overflow in subtraction\n");
                        return 1;
                    }
                    result[i][j] = matrix1[i][j] - matrix2[i][j];
                }
            }
            break;
        case 3:
            if (rows != cols) {
                printf("Matrix multiplication requires square matrices\n");
                return 1;
            }
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    result[i][j] = 0;
                    for (int k = 0; k < cols; k++) {
                        if (matrix1[i][k] != 0 && matrix2[k][j] != 0) {
                            if ((matrix1[i][k] > 0 && matrix2[k][j] > INT_MAX / matrix1[i][k]) ||
                                (matrix1[i][k] < 0 && matrix2[k][j] < INT_MAX / matrix1[i][k]) ||
                                (matrix1[i][k] > 0 && matrix2[k][j] < INT_MIN / matrix1[i][k]) ||
                                (matrix1[i][k] < 0 && matrix2[k][j] > INT_MIN / matrix1[i][k])) {
                                printf("Integer overflow in multiplication\n");
                                return 1;
                            }
                        }
                        int product = matrix1[i][k] * matrix2[k][j];
                        if ((product > 0 && result[i][j] > INT_MAX - product) ||
                            (product < 0 && result[i][j] < INT_MIN - product)) {
                            printf("Integer overflow in multiplication sum\n");
                            return 1;
                        }
                        result[i][j] += product;
                    }
                }
            }
            break;
        default:
            printf("Invalid operation choice\n");
            return 1;
    }

    printf("Result matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", result[i][j]);
        }
        printf("\n");
    }

    return 0;
}