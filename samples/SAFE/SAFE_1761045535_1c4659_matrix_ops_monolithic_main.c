//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: monolithic_main ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 10

int main(void) {
    int rows, cols;
    printf("Enter matrix dimensions (rows columns, max %dx%d): ", MAX_DIM, MAX_DIM);
    if (scanf("%d %d", &rows, &cols) != 2) {
        printf("Invalid input format\n");
        return 1;
    }
    
    if (rows <= 0 || cols <= 0 || rows > MAX_DIM || cols > MAX_DIM) {
        printf("Invalid dimensions. Must be between 1 and %d\n", MAX_DIM);
        return 1;
    }
    
    int matrix1[MAX_DIM][MAX_DIM];
    int matrix2[MAX_DIM][MAX_DIM];
    int result[MAX_DIM][MAX_DIM];
    
    printf("Enter elements for first matrix (%dx%d):\n", rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scanf("%d", &matrix1[i][j]) != 1) {
                printf("Invalid matrix element\n");
                return 1;
            }
        }
    }
    
    printf("Enter elements for second matrix (%dx%d):\n", rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scanf("%d", &matrix2[i][j]) != 1) {
                printf("Invalid matrix element\n");
                return 1;
            }
        }
    }
    
    printf("\nMatrix addition result:\n");
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
    
    if (rows == cols) {
        printf("\nMatrix multiplication result:\n");
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                long long sum = 0;
                for (int k = 0; k < cols; k++) {
                    long long product = (long long)matrix1[i][k] * matrix2[k][j];
                    if (product > INT_MAX - sum || product < INT_MIN + sum) {
                        printf("Integer overflow detected\n");
                        return 1;
                    }
                    sum += product;
                }
                if (sum > INT_MAX || sum < INT_MIN) {
                    printf("Integer overflow detected\n");
                    return 1;
                }
                result[i][j] = (int)sum;
                printf("%6d", result[i][j]);
            }
            printf("\n");
        }
    } else {
        printf("\nMatrices not square, skipping multiplication\n");
    }
    
    printf("\nMatrix transpose of first matrix:\n");
    for (int j = 0; j < cols; j++) {
        for (int i = 0; i < rows; i++) {
            printf("%6d", matrix1[i][j]);
        }
        printf("\n");
    }
    
    return 0;
}