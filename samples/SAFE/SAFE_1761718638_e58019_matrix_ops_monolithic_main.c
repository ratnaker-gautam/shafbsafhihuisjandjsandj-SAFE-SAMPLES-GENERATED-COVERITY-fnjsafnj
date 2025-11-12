//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 100

int main(void) {
    int rows, cols;
    printf("Enter matrix dimensions (rows columns): ");
    if (scanf("%d %d", &rows, &cols) != 2) {
        fprintf(stderr, "Error: Invalid input format\n");
        return 1;
    }
    if (rows <= 0 || cols <= 0 || rows > MAX_DIM || cols > MAX_DIM) {
        fprintf(stderr, "Error: Dimensions must be between 1 and %d\n", MAX_DIM);
        return 1;
    }

    int matrix1[MAX_DIM][MAX_DIM];
    int matrix2[MAX_DIM][MAX_DIM];
    int result[MAX_DIM][MAX_DIM];

    printf("Enter elements for first matrix (%d x %d):\n", rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scanf("%d", &matrix1[i][j]) != 1) {
                fprintf(stderr, "Error: Invalid matrix element\n");
                return 1;
            }
        }
    }

    printf("Enter elements for second matrix (%d x %d):\n", rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scanf("%d", &matrix2[i][j]) != 1) {
                fprintf(stderr, "Error: Invalid matrix element\n");
                return 1;
            }
        }
    }

    printf("Select operation:\n");
    printf("1. Add matrices\n");
    printf("2. Subtract matrices\n");
    printf("3. Multiply matrices\n");
    int choice;
    if (scanf("%d", &choice) != 1) {
        fprintf(stderr, "Error: Invalid operation choice\n");
        return 1;
    }

    switch (choice) {
        case 1:
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    if ((matrix1[i][j] > 0 && matrix2[i][j] > INT_MAX - matrix1[i][j]) ||
                        (matrix1[i][j] < 0 && matrix2[i][j] < INT_MIN - matrix1[i][j])) {
                        fprintf(stderr, "Error: Integer overflow in addition\n");
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
                    if ((matrix2[i][j] > 0 && matrix1[i][j] < INT_MIN + matrix2[i][j]) ||
                        (matrix2[i][j] < 0 && matrix1[i][j] > INT_MAX + matrix2[i][j])) {
                        fprintf(stderr, "Error: Integer overflow in subtraction\n");
                        return 1;
                    }
                    result[i][j] = matrix1[i][j] - matrix2[i][j];
                }
            }
            printf("Matrix subtraction result:\n");
            break;
        case 3:
            if (rows != cols) {
                fprintf(stderr, "Error: Multiplication requires square matrices\n");
                return 1;
            }
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    result[i][j] = 0;
                    for (int k = 0; k < cols; k++) {
                        long long product = (long long)matrix1[i][k] * matrix2[k][j];
                        if (product > INT_MAX || product < INT_MIN) {
                            fprintf(stderr, "Error: Integer overflow in multiplication\n");
                            return 1;
                        }
                        long long sum = (long long)result[i][j] + product;
                        if (sum > INT_MAX || sum < INT_MIN) {
                            fprintf(stderr, "Error: Integer overflow in multiplication\n");
                            return 1;
                        }
                        result[i][j] = (int)sum;
                    }
                }
            }
            printf("Matrix multiplication result:\n");
            break;
        default:
            fprintf(stderr, "Error: Invalid operation choice\n");
            return 1;
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d ", result[i][j]);
        }
        printf("\n");
    }

    return 0;
}