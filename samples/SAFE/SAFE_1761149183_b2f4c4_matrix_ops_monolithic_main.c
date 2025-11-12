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
    
    printf("Enter number of rows (1-%d): ", MAX_ROWS);
    if (scanf("%d", &rows) != 1 || rows < 1 || rows > MAX_ROWS) {
        printf("Invalid input for rows\n");
        return 1;
    }
    
    printf("Enter number of columns (1-%d): ", MAX_COLS);
    if (scanf("%d", &cols) != 1 || cols < 1 || cols > MAX_COLS) {
        printf("Invalid input for columns\n");
        return 1;
    }
    
    printf("Enter elements of first matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            if (scanf("%d", &matrix1[i][j]) != 1) {
                printf("Invalid input\n");
                return 1;
            }
        }
    }
    
    printf("Enter elements of second matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            if (scanf("%d", &matrix2[i][j]) != 1) {
                printf("Invalid input\n");
                return 1;
            }
        }
    }
    
    printf("\nMatrix addition:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if ((matrix2[i][j] > 0 && matrix1[i][j] > INT_MAX - matrix2[i][j]) ||
                (matrix2[i][j] < 0 && matrix1[i][j] < INT_MIN - matrix2[i][j])) {
                printf("Integer overflow detected\n");
                return 1;
            }
            result[i][j] = matrix1[i][j] + matrix2[i][j];
            printf("%6d", result[i][j]);
        }
        printf("\n");
    }
    
    printf("\nMatrix subtraction:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if ((matrix2[i][j] > 0 && matrix1[i][j] < INT_MIN + matrix2[i][j]) ||
                (matrix2[i][j] < 0 && matrix1[i][j] > INT_MAX + matrix2[i][j])) {
                printf("Integer overflow detected\n");
                return 1;
            }
            result[i][j] = matrix1[i][j] - matrix2[i][j];
            printf("%6d", result[i][j]);
        }
        printf("\n");
    }
    
    if (rows == cols) {
        printf("\nMatrix multiplication:\n");
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                result[i][j] = 0;
                for (int k = 0; k < cols; k++) {
                    if (matrix1[i][k] != 0 && matrix2[k][j] != 0) {
                        if ((matrix1[i][k] > 0 && matrix2[k][j] > 0 && matrix1[i][k] > INT_MAX / matrix2[k][j]) ||
                            (matrix1[i][k] > 0 && matrix2[k][j] < 0 && matrix2[k][j] < INT_MIN / matrix1[i][k]) ||
                            (matrix1[i][k] < 0 && matrix2[k][j] > 0 && matrix1[i][k] < INT_MIN / matrix2[k][j]) ||
                            (matrix1[i][k] < 0 && matrix2[k][j] < 0 && matrix1[i][k] < INT_MAX / matrix2[k][j])) {
                            printf("Integer overflow detected\n");
                            return 1;
                        }
                    }
                    int product = matrix1[i][k] * matrix2[k][j];
                    if ((product > 0 && result[i][j] > INT_MAX - product) ||
                        (product < 0 && result[i][j] < INT_MIN - product)) {
                        printf("Integer overflow detected\n");
                        return 1;
                    }
                    result[i][j] += product;
                }
                printf("%6d", result[i][j]);
            }
            printf("\n");
        }
    } else {
        printf("\nMatrix multiplication not possible (matrices must be square)\n");
    }
    
    printf("\nTranspose of first matrix:\n");
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            printf("%6d", matrix1[j][i]);
        }
        printf("\n");
    }
    
    return 0;
}