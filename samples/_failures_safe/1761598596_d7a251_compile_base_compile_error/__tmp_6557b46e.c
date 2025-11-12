//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ROWS 10
#define MAX_COLS 10
#define MATRIX_SIZE (MAX_ROWS * MAX_COLS)

typedef struct {
    int data[MAX_ROWS][MAX_COLS];
    int rows;
    int cols;
} Matrix;

#define MATRIX_INIT { .data = {{0}}, .rows = 0, .cols = 0 }

#define VALID_DIM(r, c) ((r) > 0 && (r) <= MAX_ROWS && (c) > 0 && (c) <= MAX_COLS)
#define VALID_INDEX(m, r, c) ((r) >= 0 && (r) < (m).rows && (c) >= 0 && (c) < (m).cols)

#define PRINT_MATRIX(m) do { \
    for (int i = 0; i < (m).rows; i++) { \
        for (int j = 0; j < (m).cols; j++) { \
            printf("%6d", (m).data[i][j]); \
        } \
        printf("\n"); \
    } \
} while(0)

#define READ_MATRIX(m) do { \
    printf("Enter matrix elements row by row:\n"); \
    for (int i = 0; i < (m).rows; i++) { \
        for (int j = 0; j < (m).cols; j++) { \
            if (scanf("%d", &(m).data[i][j]) != 1) { \
                printf("Invalid input\n"); \
                exit(1); \
            } \
        } \
    } \
} while(0)

#define MATRIX_ADD(a, b, result) do { \
    if ((a).rows != (b).rows || (a).cols != (b).cols) { \
        printf("Matrix dimensions don't match for addition\n"); \
        return; \
    } \
    (result).rows = (a).rows; \
    (result).cols = (a).cols; \
    for (int i = 0; i < (a).rows; i++) { \
        for (int j = 0; j < (a).cols; j++) { \
            if (((a).data[i][j] > 0 && (b).data[i][j] > INT_MAX - (a).data[i][j]) || \
                ((a).data[i][j] < 0 && (b).data[i][j] < INT_MIN - (a).data[i][j])) { \
                printf("Integer overflow in addition\n"); \
                return; \
            } \
            (result).data[i][j] = (a).data[i][j] + (b).data[i][j]; \
        } \
    } \
} while(0)

#define MATRIX_MULTIPLY(a, b, result) do { \
    if ((a).cols != (b).rows) { \
        printf("Matrix dimensions incompatible for multiplication\n"); \
        return; \
    } \
    (result).rows = (a).rows; \
    (result).cols = (b).cols; \
    for (int i = 0; i < (a).rows; i++) { \
        for (int j = 0; j < (b).cols; j++) { \
            int sum = 0; \
            for (int k = 0; k < (a).cols; k++) { \
                long long product = (long long)(a).data[i][k] * (long long)(b).data[k][j]; \
                if (product > INT_MAX || product < INT_MIN) { \
                    printf("Integer overflow in multiplication\n"); \
                    return; \
                } \
                if ((product > 0 && sum > INT_MAX - product) || \
                    (product < 0 && sum < INT_MIN - product)) { \
                    printf("Integer overflow in summation\n"); \
                    return; \
                } \
                sum += (int)product; \
            } \
            (result).data[i][j] = sum; \
        } \
    } \
} while(0)

void matrix_operations() {
    Matrix mat1 = MATRIX_INIT;
    Matrix mat2 = MATRIX_INIT;
    Matrix result = MATRIX_INIT;
    
    printf("Enter dimensions for first matrix (rows cols): ");
    if (scanf("%d %d", &mat1.rows, &mat1.cols) != 2) {
        printf("Invalid dimensions\n");
        return;
    }
    if (!VALID_DIM(mat1.rows, mat1.cols)) {
        printf("Invalid dimensions (max %dx%d)\n", MAX_ROWS, MAX_COLS);
        return;
    }
    READ_MATRIX(mat1);
    
    printf("Enter dimensions for second matrix (rows cols): ");
    if (scanf("%d %d", &mat2.rows, &mat2.cols) != 2) {
        printf("Invalid dimensions\n");
        return;
    }
    if (!VALID_DIM(mat2.rows, mat2.cols)) {
        printf("Invalid dimensions (max %dx%d)\n", MAX_ROWS, MAX_COLS);
        return;
    }
    READ_MATRIX(mat2);
    
    printf("\nMatrix 1:\n");
    PRINT_MATRIX(mat1);
    
    printf("\nMatrix 2:\n");
    PRINT