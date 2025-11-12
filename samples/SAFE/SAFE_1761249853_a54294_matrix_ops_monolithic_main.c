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
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    if (rows <= 0 || cols <= 0 || rows > MAX_ROWS || cols > MAX_COLS) {
        fprintf(stderr, "Error: Dimensions out of range (1-%d)\n", MAX_ROWS);
        return 1;
    }

    int matrix1[MAX_ROWS][MAX_COLS];
    int matrix2[MAX_ROWS][MAX_COLS];
    int result[MAX_ROWS][MAX_COLS];

    printf("Enter elements of first matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scanf("%d", &matrix1[i][j]) != 1) {
                fprintf(stderr, "Error: Invalid matrix element\n");
                return 1;
            }
        }
    }

    printf("Enter elements of second matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scanf("%d", &matrix2[i][j]) != 1) {
                fprintf(stderr, "Error: Invalid matrix element\n");
                return 1;
            }
        }
    }

    printf("Select operation (1=add, 2=subtract, 3=multiply): ");
    int operation;
    if (scanf("%d", &operation) != 1) {
        fprintf(stderr, "Error: Invalid operation selection\n");
        return 1;
    }

    if (operation == 1 || operation == 2) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (operation == 1) {
                    if ((matrix2[i][j] > 0 && matrix1[i][j] > INT_MAX - matrix2[i][j]) ||
                        (matrix2[i][j] < 0 && matrix1[i][j] < INT_MIN - matrix2[i][j])) {
                        fprintf(stderr, "Error: Integer overflow\n");
                        return 1;
                    }
                    result[i][j] = matrix1[i][j] + matrix2[i][j];
                } else {
                    if ((matrix2[i][j] < 0 && matrix1[i][j] > INT_MAX + matrix2[i][j]) ||
                        (matrix2[i][j] > 0 && matrix1[i][j] < INT_MIN + matrix2[i][j])) {
                        fprintf(stderr, "Error: Integer overflow\n");
                        return 1;
                    }
                    result[i][j] = matrix1[i][j] - matrix2[i][j];
                }
            }
        }
    } else if (operation == 3) {
        int cols2;
        printf("Enter columns for second matrix: ");
        if (scanf("%d", &cols2) != 1) {
            fprintf(stderr, "Error: Invalid column count\n");
            return 1;
        }
        if (cols2 <= 0 || cols2 > MAX_COLS) {
            fprintf(stderr, "Error: Column count out of range\n");
            return 1;
        }
        if (cols != rows) {
            fprintf(stderr, "Error: Matrix dimensions incompatible for multiplication\n");
            return 1;
        }

        printf("Enter elements of second matrix (%dx%d):\n", rows, cols2);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols2; j++) {
                if (scanf("%d", &matrix2[i][j]) != 1) {
                    fprintf(stderr, "Error: Invalid matrix element\n");
                    return 1;
                }
            }
        }

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols2; j++) {
                long long sum = 0;
                for (int k = 0; k < cols; k++) {
                    long long product = (long long)matrix1[i][k] * matrix2[k][j];
                    if (product > INT_MAX - sum || product < INT_MIN + sum) {
                        fprintf(stderr, "Error: Integer overflow during multiplication\n");
                        return 1;
                    }
                    sum += product;
                }
                if (sum > INT_MAX || sum < INT_MIN) {
                    fprintf(stderr, "Error: Integer overflow in result\n");
                    return 1;
                }
                result[i][j] = (int)sum;
            }
        }
        cols = cols2;
    } else {
        fprintf(stderr, "Error: Invalid operation\n");
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