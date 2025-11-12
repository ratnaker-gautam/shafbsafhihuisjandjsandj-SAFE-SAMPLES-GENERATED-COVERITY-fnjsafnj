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
        for (int i = 0; i < rows; i++) \
            for (int j = 0; j < cols; j++) \
                mat[i][j] = 0; \
    } while (0)

#define MATRIX_PRINT(mat, rows, cols) \
    do { \
        for (int i = 0; i < rows; i++) { \
            for (int j = 0; j < cols; j++) \
                printf("%6d", mat[i][j]); \
            printf("\n"); \
        } \
    } while (0)

#define MATRIX_ADD(dest, src1, src2, rows, cols) \
    do { \
        for (int i = 0; i < rows; i++) \
            for (int j = 0; j < cols; j++) \
                dest[i][j] = src1[i][j] + src2[i][j]; \
    } while (0)

#define MATRIX_MULT(dest, mat1, r1, c1, mat2, r2, c2) \
    do { \
        for (int i = 0; i < r1; i++) { \
            for (int j = 0; j < c2; j++) { \
                int sum = 0; \
                for (int k = 0; k < c1; k++) { \
                    if (mat1[i][k] > 0 && mat2[k][j] > 0) { \
                        if (mat1[i][k] > INT_MAX / mat2[k][j]) { \
                            printf("Integer overflow detected\n"); \
                            return 1; \
                        } \
                    } else if (mat1[i][k] < 0 && mat2[k][j] < 0) { \
                        if (mat1[i][k] < INT_MIN / mat2[k][j]) { \
                            printf("Integer overflow detected\n"); \
                            return 1; \
                        } \
                    } \
                    sum += mat1[i][k] * mat2[k][j]; \
                } \
                dest[i][j] = sum; \
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
    
    printf("\nFirst matrix:\n");
    MATRIX_PRINT(matrix1, rows1, cols1);
    
    printf("\nSecond matrix:\n");
    MATRIX_PRINT(matrix2, rows2, cols2);
    
    printf("\nMatrix addition:\n");
    if (rows1 == rows2 && cols1 == cols2) {
        MATRIX_INIT(result, rows1, cols1);
        MATRIX_ADD(result, matrix1, matrix2, rows1, cols1);
        MATRIX_PRINT(result, rows1, cols1);
    } else {
        printf("Matrices must have same dimensions for addition\n");
    }
    
    printf("\nMatrix multiplication:\n");
    if (cols1 == rows2) {
        MATRIX_INIT(result, rows1, cols2);
        MATRIX_MULT(result, matrix1, rows1, cols1, matrix2, rows2, cols2);
        MATRIX_PRINT(result, rows1, cols2);
    } else {
        printf("Matrices incompatible for multiplication\n");
    }
    
    return 0;
}