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
    int i, j, k, valid_input;
    
    printf("Enter number of rows (1-%d): ", MAX_ROWS);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    if (sscanf(input_buffer, "%d", &rows) != 1) {
        fprintf(stderr, "Invalid input for rows\n");
        return 1;
    }
    if (rows < 1 || rows > MAX_ROWS) {
        fprintf(stderr, "Rows must be between 1 and %d\n", MAX_ROWS);
        return 1;
    }
    
    printf("Enter number of columns (1-%d): ", MAX_COLS);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    if (sscanf(input_buffer, "%d", &cols) != 1) {
        fprintf(stderr, "Invalid input for columns\n");
        return 1;
    }
    if (cols < 1 || cols > MAX_COLS) {
        fprintf(stderr, "Columns must be between 1 and %d\n", MAX_COLS);
        return 1;
    }
    
    printf("Enter elements of first matrix:\n");
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            valid_input = 0;
            while (!valid_input) {
                printf("Element [%d][%d]: ", i, j);
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    fprintf(stderr, "Error reading input\n");
                    return 1;
                }
                if (sscanf(input_buffer, "%d", &matrix1[i][j]) == 1) {
                    valid_input = 1;
                } else {
                    printf("Invalid input. Please enter an integer.\n");
                }
            }
        }
    }
    
    printf("Enter elements of second matrix:\n");
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            valid_input = 0;
            while (!valid_input) {
                printf("Element [%d][%d]: ", i, j);
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    fprintf(stderr, "Error reading input\n");
                    return 1;
                }
                if (sscanf(input_buffer, "%d", &matrix2[i][j]) == 1) {
                    valid_input = 1;
                } else {
                    printf("Invalid input. Please enter an integer.\n");
                }
            }
        }
    }
    
    printf("\nMatrix 1:\n");
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            printf("%d ", matrix1[i][j]);
        }
        printf("\n");
    }
    
    printf("\nMatrix 2:\n");
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            printf("%d ", matrix2[i][j]);
        }
        printf("\n");
    }
    
    printf("\nMatrix Addition Result:\n");
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            if ((matrix1[i][j] > 0 && matrix2[i][j] > INT_MAX - matrix1[i][j]) ||
                (matrix1[i][j] < 0 && matrix2[i][j] < INT_MIN - matrix1[i][j])) {
                fprintf(stderr, "Integer overflow detected\n");
                return 1;
            }
            result[i][j] = matrix1[i][j] + matrix2[i][j];
            printf("%d ", result[i][j]);
        }
        printf("\n");
    }
    
    printf("\nMatrix Subtraction Result:\n");
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            if ((matrix2[i][j] > 0 && matrix1[i][j] < INT_MIN + matrix2[i][j]) ||
                (matrix2[i][j] < 0 && matrix1[i][j] > INT_MAX + matrix2[i][j])) {
                fprintf(stderr, "Integer overflow detected\n");
                return 1;
            }
            result[i][j] = matrix1[i][j] - matrix2[i][j];
            printf("%d ", result[i][j]);
        }
        printf("\n");
    }
    
    if (rows == cols) {
        printf("\nMatrix Multiplication Result:\n");
        for (i = 0; i < rows; i++) {
            for (j = 0; j < cols; j++) {
                result[i][j] = 0;