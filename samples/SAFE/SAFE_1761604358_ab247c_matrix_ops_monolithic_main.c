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
    printf("Enter matrix dimensions (rows columns): ");
    if (scanf("%d %d", &rows, &cols) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (rows <= 0 || cols <= 0 || rows > MAX_ROWS || cols > MAX_COLS) {
        printf("Invalid dimensions\n");
        return 1;
    }
    
    int matrix[MAX_ROWS][MAX_COLS];
    printf("Enter %d elements:\n", rows * cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scanf("%d", &matrix[i][j]) != 1) {
                printf("Invalid input\n");
                return 1;
            }
        }
    }
    
    printf("\nOriginal matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%4d", matrix[i][j]);
        }
        printf("\n");
    }
    
    int transposed[MAX_COLS][MAX_ROWS];
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            transposed[i][j] = matrix[j][i];
        }
    }
    
    printf("\nTransposed matrix:\n");
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            printf("%4d", transposed[i][j]);
        }
        printf("\n");
    }
    
    if (rows == cols) {
        int trace = 0;
        for (int i = 0; i < rows; i++) {
            trace += matrix[i][i];
        }
        printf("\nMatrix trace: %d\n", trace);
        
        int symmetric = 1;
        for (int i = 0; i < rows && symmetric; i++) {
            for (int j = i + 1; j < cols && symmetric; j++) {
                if (matrix[i][j] != matrix[j][i]) {
                    symmetric = 0;
                }
            }
        }
        printf("Matrix is %ssymmetric\n", symmetric ? "" : "not ");
    } else {
        printf("\nMatrix is not square - cannot compute trace or symmetry\n");
    }
    
    int row_sums[MAX_ROWS] = {0};
    int col_sums[MAX_COLS] = {0};
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            row_sums[i] += matrix[i][j];
            col_sums[j] += matrix[i][j];
        }
    }
    
    printf("\nRow sums:\n");
    for (int i = 0; i < rows; i++) {
        printf("Row %d: %d\n", i + 1, row_sums[i]);
    }
    
    printf("\nColumn sums:\n");
    for (int j = 0; j < cols; j++) {
        printf("Col %d: %d\n", j + 1, col_sums[j]);
    }
    
    int max_val = matrix[0][0];
    int min_val = matrix[0][0];
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (matrix[i][j] > max_val) {
                max_val = matrix[i][j];
            }
            if (matrix[i][j] < min_val) {
                min_val = matrix[i][j];
            }
        }
    }
    
    printf("\nMaximum value: %d\n", max_val);
    printf("Minimum value: %d\n", min_val);
    
    return 0;
}