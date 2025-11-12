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
    int matrix1[MAX_ROWS][MAX_COLS];
    int matrix2[MAX_ROWS][MAX_COLS];
    int result[MAX_ROWS][MAX_COLS];
    int i, j, k;
    char input_buffer[32];
    char *endptr;
    long temp;

    printf("Enter number of rows (1-%d): ", MAX_ROWS);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    temp = strtol(input_buffer, &endptr, 10);
    if (endptr == input_buffer || *endptr != '\n' || temp < 1 || temp > MAX_ROWS) {
        fprintf(stderr, "Invalid row count\n");
        return 1;
    }
    rows = (int)temp;

    printf("Enter number of columns (1-%d): ", MAX_COLS);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    temp = strtol(input_buffer, &endptr, 10);
    if (endptr == input_buffer || *endptr != '\n' || temp < 1 || temp > MAX_COLS) {
        fprintf(stderr, "Invalid column count\n");
        return 1;
    }
    cols = (int)temp;

    printf("Enter elements of first matrix:\n");
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                fprintf(stderr, "Input error\n");
                return 1;
            }
            temp = strtol(input_buffer, &endptr, 10);
            if (endptr == input_buffer || *endptr != '\n') {
                fprintf(stderr, "Invalid matrix element\n");
                return 1;
            }
            matrix1[i][j] = (int)temp;
        }
    }

    printf("Enter elements of second matrix:\n");
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                fprintf(stderr, "Input error\n");
                return 1;
            }
            temp = strtol(input_buffer, &endptr, 10);
            if (endptr == input_buffer || *endptr != '\n') {
                fprintf(stderr, "Invalid matrix element\n");
                return 1;
            }
            matrix2[i][j] = (int)temp;
        }
    }

    printf("Matrix addition:\n");
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            if ((matrix1[i][j] > 0 && matrix2[i][j] > INT_MAX - matrix1[i][j]) ||
                (matrix1[i][j] < 0 && matrix2[i][j] < INT_MIN - matrix1[i][j])) {
                fprintf(stderr, "Integer overflow in addition\n");
                return 1;
            }
            result[i][j] = matrix1[i][j] + matrix2[i][j];
            printf("%6d", result[i][j]);
        }
        printf("\n");
    }

    if (rows == cols) {
        printf("Matrix multiplication:\n");
        for (i = 0; i < rows; i++) {
            for (j = 0; j < cols; j++) {
                result[i][j] = 0;
                for (k = 0; k < cols; k++) {
                    long long product = (long long)matrix1[i][k] * (long long)matrix2[k][j];
                    if (product > INT_MAX || product < INT_MIN) {
                        fprintf(stderr, "Integer overflow in multiplication\n");
                        return 1;
                    }
                    long long sum = (long long)result[i][j] + product;
                    if (sum > INT_MAX || sum < INT_MIN) {
                        fprintf(stderr, "Integer overflow in multiplication\n");
                        return 1;
                    }
                    result[i][j] = (int)sum;
                }
                printf("%6d", result[i][j]);
            }
            printf("\n");
        }
    } else {
        printf("Matrices are not square, skipping multiplication\n");
    }

    printf("Matrix transpose (first matrix):\n");
    for (i = 0; i < cols; i++) {
        for (j = 0; j < rows; j++) {
            printf("%6d", matrix1[j][i]);
        }
        printf("\n");
    }

    return 0;
}