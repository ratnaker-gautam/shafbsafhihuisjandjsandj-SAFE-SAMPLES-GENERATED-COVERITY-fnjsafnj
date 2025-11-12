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
    int matrix[MAX_ROWS][MAX_COLS];
    int result[MAX_ROWS][MAX_COLS];
    
    printf("Enter number of rows (1-%d): ", MAX_ROWS);
    if (scanf("%d", &rows) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    if (rows < 1 || rows > MAX_ROWS) {
        printf("Invalid row count\n");
        return 1;
    }
    
    printf("Enter number of columns (1-%d): ", MAX_COLS);
    if (scanf("%d", &cols) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    if (cols < 1 || cols > MAX_COLS) {
        printf("Invalid column count\n");
        return 1;
    }
    
    printf("Enter matrix elements:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            if (scanf("%d", &matrix[i][j]) != 1) {
                printf("Invalid input\n");
                return 1;
            }
        }
    }
    
    printf("\nOriginal matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d", matrix[i][j]);
        }
        printf("\n");
    }
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = 0;
            for (int k = 0; k < cols; k++) {
                if (matrix[i][k] > 0 && matrix[k][j] > 0) {
                    if (matrix[i][k] > INT_MAX / matrix[k][j]) {
                        printf("Integer overflow detected\n");
                        return 1;
                    }
                    int product = matrix[i][k] * matrix[k][j];
                    if (result[i][j] > INT_MAX - product) {
                        printf("Integer overflow detected\n");
                        return 1;
                    }
                    result[i][j] += product;
                }
            }
        }
    }
    
    printf("\nProcessed matrix (sum of positive element products):\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d", result[i][j]);
        }
        printf("\n");
    }
    
    int trace = 0;
    int min_dim = rows < cols ? rows : cols;
    for (int i = 0; i < min_dim; i++) {
        if (trace > INT_MAX - result[i][i]) {
            printf("Integer overflow detected\n");
            return 1;
        }
        trace += result[i][i];
    }
    printf("Trace of processed matrix: %d\n", trace);
    
    return 0;
}