//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 10

int main(void) {
    int rows, cols;
    char input[32];
    
    printf("Enter number of rows (1-%d): ", MAX_DIM);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    if (sscanf(input, "%d", &rows) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    if (rows < 1 || rows > MAX_DIM) {
        fprintf(stderr, "Rows out of range\n");
        return 1;
    }
    
    printf("Enter number of columns (1-%d): ", MAX_DIM);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    if (sscanf(input, "%d", &cols) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    if (cols < 1 || cols > MAX_DIM) {
        fprintf(stderr, "Columns out of range\n");
        return 1;
    }
    
    int matrix1[MAX_DIM][MAX_DIM];
    int matrix2[MAX_DIM][MAX_DIM];
    int result[MAX_DIM][MAX_DIM];
    
    printf("Enter elements of first matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            if (fgets(input, sizeof(input), stdin) == NULL) {
                fprintf(stderr, "Input error\n");
                return 1;
            }
            if (sscanf(input, "%d", &matrix1[i][j]) != 1) {
                fprintf(stderr, "Invalid input\n");
                return 1;
            }
        }
    }
    
    printf("Enter elements of second matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            if (fgets(input, sizeof(input), stdin) == NULL) {
                fprintf(stderr, "Input error\n");
                return 1;
            }
            if (sscanf(input, "%d", &matrix2[i][j]) != 1) {
                fprintf(stderr, "Invalid input\n");
                return 1;
            }
        }
    }
    
    printf("\nMatrix addition result:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if ((matrix1[i][j] > 0 && matrix2[i][j] > INT_MAX - matrix1[i][j]) ||
                (matrix1[i][j] < 0 && matrix2[i][j] < INT_MIN - matrix1[i][j])) {
                fprintf(stderr, "Integer overflow detected\n");
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
                        fprintf(stderr, "Integer overflow detected\n");
                        return 1;
                    }
                    sum += product;
                }
                if (sum > INT_MAX || sum < INT_MIN) {
                    fprintf(stderr, "Integer overflow detected\n");
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