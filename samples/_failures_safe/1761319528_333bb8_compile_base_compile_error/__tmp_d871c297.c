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
    int i, j, k;
    
    printf("Enter number of rows (1-%d): ", MAX_ROWS);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    temp = strtol(input_buffer, &endptr, 10);
    if (endptr == input_buffer || *endptr != '\n' || temp < 1 || temp > MAX_ROWS) {
        fprintf(stderr, "Invalid number of rows\n");
        return 1;
    }
    rows = (int)temp;
    
    printf("Enter number of columns (1-%d): ", MAX_COLS);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    temp = strtol(input_buffer, &endptr, 10);
    if (endptr == input_buffer || *endptr != '\n' || temp < 1 || temp > MAX_COLS) {
        fprintf(stderr, "Invalid number of columns\n");
        return 1;
    }
    cols = (int)temp;
    
    printf("Enter elements for matrix A:\n");
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            printf("A[%d][%d]: ", i, j);
            if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                fprintf(stderr, "Error reading input\n");
                return 1;
            }
            temp = strtol(input_buffer, &endptr, 10);
            if (endptr == input_buffer || *endptr != '\n') {
                fprintf(stderr, "Invalid matrix element\n");
                return 1;
            }
            matrix_a[i][j] = (int)temp;
        }
    }
    
    printf("Enter elements for matrix B:\n");
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            printf("B[%d][%d]: ", i, j);
            if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                fprintf(stderr, "Error reading input\n");
                return 1;
            }
            temp = strtol(input_buffer, &endptr, 10);
            if (endptr == input_buffer || *endptr != '\n') {
                fprintf(stderr, "Invalid matrix element\n");
                return 1;
            }
            matrix_b[i][j] = (int)temp;
        }
    }
    
    printf("\nMatrix A:\n");
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            printf("%6d", matrix_a[i][j]);
        }
        printf("\n");
    }
    
    printf("\nMatrix B:\n");
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            printf("%6d", matrix_b[i][j]);
        }
        printf("\n");
    }
    
    printf("\nMatrix Addition (A + B):\n");
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            if ((matrix_a[i][j] > 0 && matrix_b[i][j] > INT_MAX - matrix_a[i][j]) ||
                (matrix_a[i][j] < 0 && matrix_b[i][j] < INT_MIN - matrix_a[i][j])) {
                fprintf(stderr, "Integer overflow in addition\n");
                return 1;
            }
            result[i][j] = matrix_a[i][j] + matrix_b[i][j];
            printf("%6d", result[i][j]);
        }
        printf("\n");
    }
    
    printf("\nMatrix Subtraction (A - B):\n");
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            if ((matrix_b[i][j] > 0 && matrix_a[i][j] < INT_MIN + matrix_b[i][j]) ||
                (matrix_b[i][j] < 0 && matrix_a[i][j] > INT_MAX + matrix_b[i][j])) {
                fprintf(stderr, "Integer overflow in subtraction\n");
                return 1;
            }
            result[i][j] = matrix_a[i][j] - matrix_b[i][j];
            printf("%6d", result[i][j]);
        }
        printf("\n");
    }
    
    if (rows == cols) {
        printf("\nMatrix Multiplication (A * B):\n");
        for (i = 0; i < rows; i++) {
            for (j = 0; j < cols; j++) {
                long sum = 0;
                for (k = 0; k