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
    char input_buffer[32];
    int i, j, choice;
    
    printf("Enter number of rows (1-%d): ", MAX_ROWS);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    if (sscanf(input_buffer, "%d", &rows) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    if (rows < 1 || rows > MAX_ROWS) {
        fprintf(stderr, "Rows out of range\n");
        return 1;
    }
    
    printf("Enter number of columns (1-%d): ", MAX_COLS);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    if (sscanf(input_buffer, "%d", &cols) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    if (cols < 1 || cols > MAX_COLS) {
        fprintf(stderr, "Columns out of range\n");
        return 1;
    }
    
    printf("Enter elements for matrix 1:\n");
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                fprintf(stderr, "Input error\n");
                return 1;
            }
            if (sscanf(input_buffer, "%d", &matrix1[i][j]) != 1) {
                fprintf(stderr, "Invalid input\n");
                return 1;
            }
        }
    }
    
    printf("Enter elements for matrix 2:\n");
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                fprintf(stderr, "Input error\n");
                return 1;
            }
            if (sscanf(input_buffer, "%d", &matrix2[i][j]) != 1) {
                fprintf(stderr, "Invalid input\n");
                return 1;
            }
        }
    }
    
    printf("\nMatrix operations:\n");
    printf("1. Add matrices\n");
    printf("2. Subtract matrices\n");
    printf("3. Multiply matrices (element-wise)\n");
    printf("Enter choice (1-3): ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    if (sscanf(input_buffer, "%d", &choice) != 1) {
        fprintf(stderr, "Invalid choice\n");
        return 1;
    }
    
    switch (choice) {
        case 1:
            for (i = 0; i < rows; i++) {
                for (j = 0; j < cols; j++) {
                    if ((matrix1[i][j] > 0 && matrix2[i][j] > INT_MAX - matrix1[i][j]) ||
                        (matrix1[i][j] < 0 && matrix2[i][j] < INT_MIN - matrix1[i][j])) {
                        fprintf(stderr, "Integer overflow in addition\n");
                        return 1;
                    }
                    result[i][j] = matrix1[i][j] + matrix2[i][j];
                }
            }
            printf("\nMatrix addition result:\n");
            break;
            
        case 2:
            for (i = 0; i < rows; i++) {
                for (j = 0; j < cols; j++) {
                    if ((matrix2[i][j] > 0 && matrix1[i][j] < INT_MIN + matrix2[i][j]) ||
                        (matrix2[i][j] < 0 && matrix1[i][j] > INT_MAX + matrix2[i][j])) {
                        fprintf(stderr, "Integer overflow in subtraction\n");
                        return 1;
                    }
                    result[i][j] = matrix1[i][j] - matrix2[i][j];
                }
            }
            printf("\nMatrix subtraction result:\n");
            break;
            
        case 3:
            for (i = 0; i < rows; i++) {
                for (j = 0; j < cols; j++) {
                    if (matrix1[i][j] > 0 && matrix2[i][j] > 0) {
                        if (matrix1[i][j] > INT_MAX / matrix2[i][j]) {
                            fprintf(stderr, "Integer overflow in multiplication\n");
                            return 1;
                        }
                    } else if (matrix1[i][j] < 0 && matrix2[i][j] < 0) {
                        if (matrix1[i][j] < INT_MAX / matrix2