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

    printf("Enter elements of first matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scanf("%d", &matrix1[i][j]) != 1) {
                printf("Invalid matrix element\n");
                return 1;
            }
        }
    }

    printf("Enter elements of second matrix:\n");
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
                    if ((matrix2[i][j] > 0 && matrix1[i][j] > INT_MAX - matrix2[i][j]) ||
                        (matrix2[i][j] < 0 && matrix1[i][j] < INT_MIN - matrix2[i][j])) {
                        printf("Integer overflow detected\n");
                        return 1;
                    }
                    result[i][j] = matrix1[i][j] + matrix2[i][j];
                }
            }
            printf("Matrix addition result:\n");
            break;
        case 2:
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    if ((matrix2[i][j] < 0 && matrix1[i][j] > INT_MAX + matrix2[i][j]) ||
                        (matrix2[i][j] > 0 && matrix1[i][j] < INT_MIN + matrix2[i][j])) {
                        printf("Integer overflow detected\n");
                        return 1;
                    }
                    result[i][j] = matrix1[i][j] - matrix2[i][j];
                }
            }
            printf("Matrix subtraction result:\n");
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
                        long long product = (long long)matrix1[i][k] * matrix2[k][j];
                        if (product > INT_MAX || product < INT_MIN) {
                            printf("Integer overflow detected\n");
                            return 1;
                        }
                        if ((matrix2[k][j] > 0 && result[i][j] > INT_MAX - product) ||
                            (matrix2[k][j] < 0 && result[i][j] < INT_MIN - product)) {
                            printf("Integer overflow detected\n");
                            return 1;
                        }
                        result[i][j] += (int)product;
                    }
                }
            }
            printf("Matrix multiplication result:\n");
            break;
        default:
            printf("Invalid operation choice\n");
            return 1;
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d", result[i][j]);
        }
        printf("\n");
    }

    return 0;
}