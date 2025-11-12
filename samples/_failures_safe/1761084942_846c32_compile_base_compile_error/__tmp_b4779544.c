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
                mat[i][j] = (i == j) ? 1 : 0; \
            } \
        } \
    } while (0)

#define MATRIX_PRINT(mat, rows, cols) \
    do { \
        for (int i = 0; i < rows; i++) { \
            for (int j = 0; j < cols; j++) { \
                printf("%3d ", mat[i][j]); \
            } \
            printf("\n"); \
        } \
    } while (0)

#define MATRIX_ADD(a, b, result, rows, cols) \
    do { \
        for (int i = 0; i < rows; i++) { \
            for (int j = 0; j < cols; j++) { \
                if ((a[i][j] > 0 && b[i][j] > INT_MAX - a[i][j]) || \
                    (a[i][j] < 0 && b[i][j] < INT_MIN - a[i][j])) { \
                    printf("Integer overflow detected\n"); \
                    return 1; \
                } \
                result[i][j] = a[i][j] + b[i][j]; \
            } \
        } \
    } while (0)

#define MATRIX_MULTIPLY(a, b, result, a_rows, a_cols, b_cols) \
    do { \
        for (int i = 0; i < a_rows; i++) { \
            for (int j = 0; j < b_cols; j++) { \
                result[i][j] = 0; \
                for (int k = 0; k < a_cols; k++) { \
                    if ((a[i][k] > 0 && b[k][j] > 0 && a[i][k] > INT_MAX / b[k][j]) || \
                        (a[i][k] < 0 && b[k][j] < 0 && a[i][k] < INT_MAX / b[k][j]) || \
                        (a[i][k] > 0 && b[k][j] < 0 && b[k][j] < INT_MIN / a[i][k]) || \
                        (a[i][k] < 0 && b[k][j] > 0 && a[i][k] < INT_MIN / b[k][j])) { \
                        printf("Integer overflow detected\n"); \
                        return 1; \
                    } \
                    int product = a[i][k] * b[k][j]; \
                    if ((product > 0 && result[i][j] > INT_MAX - product) || \
                        (product < 0 && result[i][j] < INT_MIN - product)) { \
                        printf("Integer overflow detected\n"); \
                        return 1; \
                    } \
                    result[i][j] += product; \
                } \
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
        printf("Invalid dimensions\n");
        return 1;
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
    
    printf("Enter elements for first matrix:\n");
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols1; j++) {
            if (scanf("%d", &matrix1[i][j]) != 1) {
                printf("Invalid input\n");
                return 1;
            }
        }
    }
    
    printf("Enter elements for second matrix:\n");
    for (int i = 0; i < rows2; i++) {
        for (int j = 0; j < cols2; j++) {
            if (scanf("%d", &matrix2[i][j]) != 1) {
                printf("Invalid input\n");
                return 1;
            }
        }
    }
    
    printf("\nMatrix 1:\n");
    MATRIX_PRINT(matrix1, rows1, cols1);
    
    printf("\nMatrix 2:\n");
    MATRIX_PRINT(matrix2, rows2, cols2);
    
    if (