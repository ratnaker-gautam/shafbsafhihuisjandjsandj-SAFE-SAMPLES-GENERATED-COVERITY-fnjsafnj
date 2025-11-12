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
    int matrix1[MAX_ROWS][MAX_COLS];
    int matrix2[MAX_ROWS][MAX_COLS];
    int result[MAX_ROWS][MAX_COLS];
    char input_buffer[256];
    long temp;
    char *endptr;
    int i, j;
    
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
    
    printf("Enter elements of first matrix:\n");
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                fprintf(stderr, "Error reading input\n");
                return 1;
            }
            temp = strtol(input_buffer, &endptr, 10);
            if (endptr == input_buffer || *endptr != '\n') {
                fprintf(stderr, "Invalid matrix element\n");
                return 1;
            }
            if (temp < INT_MIN || temp > INT_MAX) {
                fprintf(stderr, "Matrix element out of range\n");
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
                fprintf(stderr, "Error reading input\n");
                return 1;
            }
            temp = strtol(input_buffer, &endptr, 10);
            if (endptr == input_buffer || *endptr != '\n') {
                fprintf(stderr, "Invalid matrix element\n");
                return 1;
            }
            if (temp < INT_MIN || temp > INT_MAX) {
                fprintf(stderr, "Matrix element out of range\n");
                return 1;
            }
            matrix2[i][j] = (int)temp;
        }
    }
    
    printf("Matrix addition result:\n");
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
    
    printf("Matrix subtraction result:\n");
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            if ((matrix2[i][j] > 0 && matrix1[i][j] < INT_MIN + matrix2[i][j]) ||
                (matrix2[i][j] < 0 && matrix1[i][j] > INT_MAX + matrix2[i][j])) {
                fprintf(stderr, "Integer overflow in subtraction\n");
                return 1;
            }
            result[i][j] = matrix1[i][j] - matrix2[i][j];
            printf("%6d", result[i][j]);
        }
        printf("\n");
    }
    
    if (rows == cols) {
        printf("Matrix multiplication result:\n");
        for (i = 0; i < rows; i++) {
            for (j = 0; j < cols; j++) {
                long sum = 0;
                for (int k = 0; k < cols; k++) {
                    long product = (long)matrix1[i][k] * (long)matrix2[k][j];
                    if (matrix1[i][k] != 0 && product / matrix1[i][k] != matrix2[k][j]) {
                        fprintf(stderr, "Integer overflow in multiplication\n");