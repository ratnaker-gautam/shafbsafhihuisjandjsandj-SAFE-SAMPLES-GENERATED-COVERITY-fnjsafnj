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
                printf("%3d ", mat[i][j]); \
            } \
            printf("\n"); \
        } \
    } while (0)

#define MATRIX_ADD(dest, src1, src2, rows, cols) \
    do { \
        for (int i = 0; i < rows; i++) { \
            for (int j = 0; j < cols; j++) { \
                dest[i][j] = src1[i][j] + src2[i][j]; \
            } \
        } \
    } while (0)

#define MATRIX_MULT(dest, mat1, r1, c1, mat2, r2, c2) \
    do { \
        for (int i = 0; i < r1; i++) { \
            for (int j = 0; j < c2; j++) { \
                dest[i][j] = 0; \
                for (int k = 0; k < c1; k++) { \
                    dest[i][j] += mat1[i][k] * mat2[k][j]; \
                } \
            } \
        } \
    } while (0)

#define SAFE_READ_INT(prompt, var, min, max) \
    do { \
        printf("%s", prompt); \
        if (scanf("%d", &var) != 1) { \
            fprintf(stderr, "Invalid input\n"); \
            exit(EXIT_FAILURE); \
        } \
        if (var < min || var > max) { \
            fprintf(stderr, "Value out of range\n"); \
            exit(EXIT_FAILURE); \
        } \
    } while (0)

int main(void) {
    int rows1, cols1, rows2, cols2;
    int matrix1[MAX_ROWS][MAX_COLS];
    int matrix2[MAX_ROWS][MAX_COLS];
    int result[MAX_ROWS][MAX_COLS];
    
    printf("Matrix Operations Program\n");
    printf("Matrix dimensions must be between %d and %d\n", MIN_DIM, MAX_ROWS);
    
    printf("First matrix:\n");
    SAFE_READ_INT("Enter rows: ", rows1, MIN_DIM, MAX_ROWS);
    SAFE_READ_INT("Enter columns: ", cols1, MIN_DIM, MAX_COLS);
    
    printf("Enter elements for first matrix:\n");
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols1; j++) {
            SAFE_READ_INT("", matrix1[i][j], INT_MIN, INT_MAX);
        }
    }
    
    printf("Second matrix:\n");
    SAFE_READ_INT("Enter rows: ", rows2, MIN_DIM, MAX_ROWS);
    SAFE_READ_INT("Enter columns: ", cols2, MIN_DIM, MAX_COLS);
    
    printf("Enter elements for second matrix:\n");
    for (int i = 0; i < rows2; i++) {
        for (int j = 0; j < cols2; j++) {
            SAFE_READ_INT("", matrix2[i][j], INT_MIN, INT_MAX);
        }
    }
    
    printf("\nFirst matrix:\n");
    MATRIX_PRINT(matrix1, rows1, cols1);
    
    printf("\nSecond matrix:\n");
    MATRIX_PRINT(matrix2, rows2, cols2);
    
    printf("\nMatrix Addition:\n");
    if (rows1 == rows2 && cols1 == cols2) {
        MATRIX_ADD(result, matrix1, matrix2, rows1, cols1);
        MATRIX_PRINT(result, rows1, cols1);
    } else {
        printf("Cannot add: dimension mismatch\n");
    }
    
    printf("\nMatrix Multiplication:\n");
    if (cols1 == rows2) {
        MATRIX_MULT(result, matrix1, rows1, cols1, matrix2, rows2, cols2);
        MATRIX_PRINT(result, rows1, cols2);
    } else {
        printf("Cannot multiply: dimension mismatch\n");
    }
    
    printf("\nTranspose of first matrix:\n");
    for (int i = 0; i < cols1; i++) {
        for (int j = 0; j < rows1; j++) {
            printf("%3d ", matrix1[j][i]);
        }
        printf("\n");
    }
    
    return EXIT_SUCCESS;
}