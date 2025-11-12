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
    int matrix_a[MAX_ROWS][MAX_COLS];
    int matrix_b[MAX_ROWS][MAX_COLS];
    int result[MAX_ROWS][MAX_COLS];
    char input_buffer[256];
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
    
    printf("Enter elements for matrix A:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("A[%d][%d]: ", i, j);
            if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                fprintf(stderr, "Input error\n");
                return 1;
            }
            temp = strtol(input_buffer, &endptr, 10);
            if (endptr == input_buffer || *endptr != '\n') {
                fprintf(stderr, "Invalid matrix element\n");
                return 1;
            }
            if (temp < INT_MIN || temp > INT_MAX) {
                fprintf(stderr, "Value out of range\n");
                return 1;
            }
            matrix_a[i][j] = (int)temp;
        }
    }
    
    printf("Enter elements for matrix B:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("B[%d][%d]: ", i, j);
            if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                fprintf(stderr, "Input error\n");
                return 1;
            }
            temp = strtol(input_buffer, &endptr, 10);
            if (endptr == input_buffer || *endptr != '\n') {
                fprintf(stderr, "Invalid matrix element\n");
                return 1;
            }
            if (temp < INT_MIN || temp > INT_MAX) {
                fprintf(stderr, "Value out of range\n");
                return 1;
            }
            matrix_b[i][j] = (int)temp;
        }
    }
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if ((matrix_a[i][j] > 0 && matrix_b[i][j] > INT_MAX - matrix_a[i][j]) ||
                (matrix_a[i][j] < 0 && matrix_b[i][j] < INT_MIN - matrix_a[i][j])) {
                fprintf(stderr, "Integer overflow detected\n");
                return 1;
            }
            result[i][j] = matrix_a[i][j] + matrix_b[i][j];
        }
    }
    
    printf("Matrix A:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d", matrix_a[i][j]);
        }
        printf("\n");
    }
    
    printf("Matrix B:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d", matrix_b[i][j]);
        }
        printf("\n");
    }
    
    printf("Sum (A + B):\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d", result[i][j]);
        }
        printf("\n");
    }
    
    return 0;
}