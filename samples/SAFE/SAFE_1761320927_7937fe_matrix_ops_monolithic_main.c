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
    
    printf("Enter number of rows (1-%d): ", MAX_ROWS);
    if (scanf("%d", &rows) != 1) {
        printf("Invalid input for rows\n");
        return 1;
    }
    if (rows < 1 || rows > MAX_ROWS) {
        printf("Rows must be between 1 and %d\n", MAX_ROWS);
        return 1;
    }
    
    printf("Enter number of columns (1-%d): ", MAX_COLS);
    if (scanf("%d", &cols) != 1) {
        printf("Invalid input for columns\n");
        return 1;
    }
    if (cols < 1 || cols > MAX_COLS) {
        printf("Columns must be between 1 and %d\n", MAX_COLS);
        return 1;
    }
    
    printf("Enter elements of first matrix:\n");
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            if (scanf("%d", &matrix_a[i][j]) != 1) {
                printf("Invalid matrix element\n");
                return 1;
            }
        }
    }
    
    printf("Enter elements of second matrix:\n");
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            if (scanf("%d", &matrix_b[i][j]) != 1) {
                printf("Invalid matrix element\n");
                return 1;
            }
        }
    }
    
    printf("Select operation (+, -, *): ");
    if (scanf(" %c", &op) != 1) {
        printf("Invalid operation\n");
        return 1;
    }
    
    if (op != '+' && op != '-' && op != '*') {
        printf("Invalid operation. Use +, -, or *\n");
        return 1;
    }
    
    if (op == '*') {
        int temp;
        for (i = 0; i < rows; i++) {
            for (j = 0; j < cols; j++) {
                temp = matrix_a[i][j] * matrix_b[i][j];
                if ((matrix_a[i][j] > 0 && matrix_b[i][j] > 0 && temp < 0) ||
                    (matrix_a[i][j] < 0 && matrix_b[i][j] < 0 && temp < 0)) {
                    printf("Integer overflow detected\n");
                    return 1;
                }
                result[i][j] = temp;
            }
        }
    } else if (op == '+') {
        for (i = 0; i < rows; i++) {
            for (j = 0; j < cols; j++) {
                if ((matrix_a[i][j] > 0 && matrix_b[i][j] > INT_MAX - matrix_a[i][j]) ||
                    (matrix_a[i][j] < 0 && matrix_b[i][j] < INT_MIN - matrix_a[i][j])) {
                    printf("Integer overflow detected\n");
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
                    printf("Integer overflow detected\n");
                    return 1;
                }
                result[i][j] = matrix_a[i][j] - matrix_b[i][j];
            }
        }
    }
    
    printf("Result matrix:\n");
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            printf("%6d", result[i][j]);
        }
        printf("\n");
    }
    
    return 0;
}