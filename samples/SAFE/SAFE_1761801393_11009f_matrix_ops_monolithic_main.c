//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 10

int main(void) {
    int rows, cols;
    printf("Enter matrix dimensions (rows columns, max %dx%d): ", MAX_SIZE, MAX_SIZE);
    if (scanf("%d %d", &rows, &cols) != 2) {
        printf("Invalid input format\n");
        return 1;
    }
    
    if (rows <= 0 || cols <= 0 || rows > MAX_SIZE || cols > MAX_SIZE) {
        printf("Invalid dimensions. Must be between 1 and %d\n", MAX_SIZE);
        return 1;
    }
    
    int matrix1[MAX_SIZE][MAX_SIZE];
    int matrix2[MAX_SIZE][MAX_SIZE];
    int result[MAX_SIZE][MAX_SIZE];
    
    printf("Enter elements of first matrix (%dx%d):\n", rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scanf("%d", &matrix1[i][j]) != 1) {
                printf("Invalid matrix element\n");
                return 1;
            }
        }
    }
    
    printf("Enter elements of second matrix (%dx%d):\n", rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scanf("%d", &matrix2[i][j]) != 1) {
                printf("Invalid matrix element\n");
                return 1;
            }
        }
    }
    
    printf("\nMatrix addition:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if ((matrix1[i][j] > 0 && matrix2[i][j] > INT_MAX - matrix1[i][j]) ||
                (matrix1[i][j] < 0 && matrix2[i][j] < INT_MIN - matrix1[i][j])) {
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
                            (matrix1[i][k] < 0 && matrix2[k][j] < 0 && matrix1[i][k] < INT_MAX / matrix2[k][j]) ||
                            (matrix1[i][k] > 0 && matrix2[k][j] < 0 && matrix2[k][j] < INT_MIN / matrix1[i][k]) ||
                            (matrix1[i][k] < 0 && matrix2[k][j] > 0 && matrix1[i][k] < INT_MIN / matrix2[k][j])) {
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
        printf("\nMatrix multiplication not possible (non-square matrices)\n");
    }
    
    printf("\nMatrix transpose:\n");
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            printf("%6d", matrix1[j][i]);
        }
        printf("\n");
    }
    
    return 0;
}