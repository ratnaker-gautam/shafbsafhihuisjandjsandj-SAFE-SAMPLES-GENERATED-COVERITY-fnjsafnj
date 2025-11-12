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
    char op;
    int i, j;
    
    printf("Enter matrix dimensions (rows columns, max %dx%d): ", MAX_ROWS, MAX_COLS);
    if (scanf("%d %d", &rows, &cols) != 2) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    if (rows <= 0 || cols <= 0 || rows > MAX_ROWS || cols > MAX_COLS) {
        printf("Error: Invalid dimensions\n");
        return 1;
    }
    
    printf("Enter operation (+, -, *): ");
    if (scanf(" %c", &op) != 1) {
        printf("Error: Invalid operation\n");
        return 1;
    }
    
    if (op != '+' && op != '-' && op != '*') {
        printf("Error: Unsupported operation\n");
        return 1;
    }
    
    printf("Enter matrix A (%dx%d):\n", rows, cols);
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            if (scanf("%d", &matrix_a[i][j]) != 1) {
                printf("Error: Invalid matrix element\n");
                return 1;
            }
        }
    }
    
    printf("Enter matrix B (%dx%d):\n", rows, cols);
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            if (scanf("%d", &matrix_b[i][j]) != 1) {
                printf("Error: Invalid matrix element\n");
                return 1;
            }
        }
    }
    
    if (op == '+') {
        for (i = 0; i < rows; i++) {
            for (j = 0; j < cols; j++) {
                if ((matrix_a[i][j] > 0 && matrix_b[i][j] > INT_MAX - matrix_a[i][j]) ||
                    (matrix_a[i][j] < 0 && matrix_b[i][j] < INT_MIN - matrix_a[i][j])) {
                    printf("Error: Integer overflow in addition\n");
                    return 1;
                }
                result[i][j] = matrix_a[i][j] + matrix_b[i][j];
            }
        }
    } else if (op == '-') {
        for (i = 0; i < rows; i++) {
            for (j = 0; j < cols; j++) {
                if ((matrix_b[i][j] > 0 && matrix_a[i][j] < INT_MIN + matrix_b[i][j]) ||
                    (matrix_b[i][j] < 0 && matrix_a[i][j] > INT_MAX + matrix_b[i][j])) {
                    printf("Error: Integer overflow in subtraction\n");
                    return 1;
                }
                result[i][j] = matrix_a[i][j] - matrix_b[i][j];
            }
        }
    } else if (op == '*') {
        for (i = 0; i < rows; i++) {
            for (j = 0; j < cols; j++) {
                long long product = (long long)matrix_a[i][j] * (long long)matrix_b[i][j];
                if (product > INT_MAX || product < INT_MIN) {
                    printf("Error: Integer overflow in multiplication\n");
                    return 1;
                }
                result[i][j] = (int)product;
            }
        }
    }
    
    printf("Result matrix:\n");
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            printf("%d ", result[i][j]);
        }
        printf("\n");
    }
    
    return 0;
}