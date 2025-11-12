//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: matrix_ops
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
                if (src1[i][j] > 0 && src2[i][j] > INT_MAX - src1[i][j]) { \
                    printf("Integer overflow detected\n"); \
                    return 1; \
                } \
                if (src1[i][j] < 0 && src2[i][j] < INT_MIN - src1[i][j]) { \
                    printf("Integer underflow detected\n"); \
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
                int64_t sum = 0; \
                for (int k = 0; k < cols1; k++) { \
                    if (mat1[i][k] != 0 && mat2[k][j] != 0) { \
                        if (mat1[i][k] > 0 && mat2[k][j] > INT_MAX / mat1[i][k]) { \
                            printf("Integer overflow in multiplication\n"); \
                            return 1; \
                        } \
                        if (mat1[i][k] < 0 && mat2[k][j] < INT_MAX / mat1[i][k]) { \
                            printf("Integer overflow in multiplication\n"); \
                            return 1; \
                        } \
                        int64_t product = (int64_t)mat1[i][k] * (int64_t)mat2[k][j]; \
                        if (sum > 0 && product > INT64_MAX - sum) { \
                            printf("Integer overflow in summation\n"); \
                            return 1; \
                        } \
                        if (sum < 0 && product < INT64_MIN - sum) { \
                            printf("Integer underflow in summation\n"); \
                            return 1; \
                        } \
                        sum += product; \
                    } \
                } \
                if (sum > INT_MAX || sum < INT_MIN) { \
                    printf("Result out of integer range\n"); \
                    return 1; \
                } \
                dest[i][j] = (int)sum; \
            } \
        } \
    } while (0)

int main(void) {
    int matrix1[MAX_ROWS][MAX_COLS];
    int matrix2[MAX_ROWS][MAX_COLS];
    int result[MAX_ROWS][MAX_COLS];
    
    int rows1, cols1, rows2, cols2;
    
    printf("Enter dimensions for first matrix (rows columns): ");
    if (scanf("%d %d", &rows1, &cols1) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (rows1 < MIN_DIM || rows1 > MAX_ROWS || cols1 < MIN_DIM || cols1 > MAX_COLS) {
        printf("Invalid dimensions\n");
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
        printf("Invalid dimensions\n");
        return 1;
    }
    
    printf("Enter %d elements for second matrix:\n", rows2 * cols2);
    for (int i = 0; i < rows2; i++) {
        for (int j = 0; j < cols2; j++) {
            if (scanf("%d", &matrix2[i][j])