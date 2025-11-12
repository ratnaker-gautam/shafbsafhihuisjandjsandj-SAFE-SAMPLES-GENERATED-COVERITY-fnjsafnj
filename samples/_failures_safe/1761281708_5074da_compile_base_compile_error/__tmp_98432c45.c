//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ROWS 10
#define MAX_COLS 10
#define MIN_DIM 1

#define MATRIX_INIT(mat, rows, cols) \
    do { \
        for (int i = 0; i < rows; i++) { \
            for (int j = 0; j < cols; j++) { \
                mat[i][j] = 0; \
            } \
        } \
    } while (0)

#define MATRIX_PRINT(mat, rows, cols) \
    do { \
        for (int i = 0; i < rows; i++) { \
            for (int j = 0; j < cols; j++) { \
                printf("%6d", mat[i][j]); \
            } \
            printf("\n"); \
        } \
    } while (0)

#define MATRIX_ADD(dest, src1, src2, rows, cols) \
    do { \
        for (int i = 0; i < rows; i++) { \
            for (int j = 0; j < cols; j++) { \
                if ((src1[i][j] > 0 && src2[i][j] > INT_MAX - src1[i][j]) || \
                    (src1[i][j] < 0 && src2[i][j] < INT_MIN - src1[i][j])) { \
                    printf("Integer overflow detected at [%d][%d]\n", i, j); \
                    return 1; \
                } \
                dest[i][j] = src1[i][j] + src2[i][j]; \
            } \
        } \
    } while (0)

#define MATRIX_MULTIPLY(dest, mat1, rows1, cols1, mat2, rows2, cols2) \
    do { \
        for (int i = 0; i < rows1; i++) { \
            for (int j = 0; j < cols2; j++) { \
                int sum = 0; \
                for (int k = 0; k < cols1; k++) { \
                    if (mat1[i][k] != 0 && mat2[k][j] != 0) { \
                        if ((mat1[i][k] > 0 && mat2[k][j] > INT_MAX / mat1[i][k]) || \
                            (mat1[i][k] < 0 && mat2[k][j] < INT_MAX / mat1[i][k])) { \
                            printf("Integer overflow detected at [%d][%d]\n", i, j); \
                            return 1; \
                        } \
                        int product = mat1[i][k] * mat2[k][j]; \
                        if ((product > 0 && sum > INT_MAX - product) || \
                            (product < 0 && sum < INT_MIN - product)) { \
                            printf("Integer overflow detected at [%d][%d]\n", i, j); \
                            return 1; \
                        } \
                        sum += product; \
                    } \
                } \
                dest[i][j] = sum; \
            } \
        } \
    } while (0)

int main(void) {
    int rows1, cols1, rows2, cols2;
    int matrix1[MAX_ROWS][MAX_COLS];
    int matrix2[MAX_ROWS][MAX_COLS];
    int result[MAX_ROWS][MAX_COLS];
    
    printf("Enter dimensions for first matrix (rows columns): ");
    if (scanf("%d %d", &rows1, &cols1) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (rows1 < MIN_DIM || rows1 > MAX_ROWS || cols1 < MIN_DIM || cols1 > MAX_COLS) {
        printf("Invalid dimensions. Must be between %d and %d\n", MIN_DIM, MAX_ROWS);
        return 1;
    }
    
    printf("Enter %d elements for first matrix:\n", rows1 * cols1);
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols1; j++) {
            if (scanf("%d", &matrix1[i][j]) != 1) {
                printf("Invalid input\n");
                return 1;
            }
        }
    }
    
    printf("Enter dimensions for second matrix (rows columns): ");
    if (scanf("%d %d", &rows2, &cols2) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (rows2 < MIN_DIM || rows2 > MAX_ROWS || cols2 < MIN_DIM || cols2 > MAX_COLS) {
        printf("Invalid dimensions. Must be between %d and %d\n", MIN_DIM, MAX_ROWS);
        return 1;
    }
    
    printf("Enter %d elements for second matrix:\n", rows2 * cols2);
    for (int i = 0; i < rows2; i++) {
        for (int j = 0; j < cols2; j++) {
            if (scanf("%d", &matrix2[i][j]) != 1) {
                printf("Invalid input\n");
                return 1;
            }
        }
    }
    
    printf("\nMatrix 1:\n");
    MATRIX_PRINT(matrix1, rows1, cols1