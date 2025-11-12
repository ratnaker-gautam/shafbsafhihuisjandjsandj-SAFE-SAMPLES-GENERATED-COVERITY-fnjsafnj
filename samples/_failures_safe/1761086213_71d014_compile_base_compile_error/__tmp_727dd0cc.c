//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ROWS 10
#define MAX_COLS 10
#define MIN_DIM 1

#define MATRIX_INIT(m, r, c) \
    do { \
        for (int i = 0; i < r; i++) \
            for (int j = 0; j < c; j++) \
                m[i][j] = 0; \
    } while (0)

#define MATRIX_PRINT(m, r, c) \
    do { \
        for (int i = 0; i < r; i++) { \
            for (int j = 0; j < c; j++) \
                printf("%3d ", m[i][j]); \
            printf("\n"); \
        } \
    } while (0)

#define MATRIX_ADD(a, b, r, c) \
    do { \
        for (int i = 0; i < r; i++) \
            for (int j = 0; j < c; j++) \
                a[i][j] += b[i][j]; \
    } while (0)

#define MATRIX_MULT(a, b, c, m, n, p) \
    do { \
        for (int i = 0; i < m; i++) { \
            for (int j = 0; j < p; j++) { \
                c[i][j] = 0; \
                for (int k = 0; k < n; k++) { \
                    if (a[i][k] > 0 && b[k][j] > 0) { \
                        if (a[i][k] > INT_MAX / b[k][j]) { \
                            printf("Integer overflow detected\n"); \
                            return 1; \
                        } \
                    } else if (a[i][k] < 0 && b[k][j] < 0) { \
                        if (a[i][k] < INT_MIN / b[k][j]) { \
                            printf("Integer overflow detected\n"); \
                            return 1; \
                        } \
                    } \
                    c[i][j] += a[i][k] * b[k][j]; \
                } \
            } \
        } \
    } while (0)

int main(void) {
    int rows1, cols1, rows2, cols2;
    int matrix1[MAX_ROWS][MAX_COLS];
    int matrix2[MAX_ROWS][MAX_COLS];
    int result[MAX_ROWS][MAX_COLS];
    
    printf("Enter dimensions for first matrix (rows cols): ");
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
    
    printf("Enter dimensions for second matrix (rows cols): ");
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
            if (scanf("%d", &matrix2[i][j]) != 1) {
                printf("Invalid input\n");
                return 1;
            }
        }
    }
    
    printf("\nFirst matrix:\n");
    MATRIX_PRINT(matrix1, rows1, cols1);
    
    printf("\nSecond matrix:\n");
    MATRIX_PRINT(matrix2, rows2, cols2);
    
    printf("\nMatrix addition:\n");
    if (rows1 == rows2 && cols1 == cols2) {
        MATRIX_INIT(result, rows1, cols1);
        for (int i = 0; i < rows1; i++) {
            for (int j = 0; j < cols1; j++) {
                if ((matrix1[i][j] > 0 && matrix2[i][j] > INT_MAX - matrix1[i][j]) ||
                    (matrix1[i][j] < 0 && matrix2[i][j] < INT_MIN - matrix1[i][j])) {
                    printf("Integer overflow detected\n");
                    return 1;
                }
                result[i][j] = matrix1[i][j] + matrix2[i][j];
            }
        }
        MATRIX_PRINT(result, rows1, cols1);
    } else {
        printf("Cannot add: dimension mismatch\n");
    }
    
    printf("\nMatrix multiplication:\n");
    if (cols