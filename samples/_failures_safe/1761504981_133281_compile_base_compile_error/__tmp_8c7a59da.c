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

#define MATRIX_INIT(m, r, c) do { \
    (m).rows = (r); \
    (m).cols = (c); \
    memset((m).data, 0, sizeof((m).data)); \
} while(0)

#define MATRIX_GET(m, i, j) ((m).data[(i)][(j)])
#define MATRIX_SET(m, i, j, val) ((m).data[(i)][(j)] = (val))

#define MATRIX_ADD(a, b, result) do { \
    if ((a).rows != (b).rows || (a).cols != (b).cols) { \
        printf("Matrix dimensions mismatch for addition\n"); \
        return 1; \
    } \
    (result).rows = (a).rows; \
    (result).cols = (a).cols; \
    for (int i = 0; i < (a).rows; i++) { \
        for (int j = 0; j < (a).cols; j++) { \
            int sum = MATRIX_GET(a, i, j) + MATRIX_GET(b, i, j); \
            if ((MATRIX_GET(a, i, j) > 0 && MATRIX_GET(b, i, j) > INT_MAX - MATRIX_GET(a, i, j)) || \
                (MATRIX_GET(a, i, j) < 0 && MATRIX_GET(b, i, j) < INT_MIN - MATRIX_GET(a, i, j))) { \
                printf("Integer overflow detected\n"); \
                return 1; \
            } \
            MATRIX_SET(result, i, j, sum); \
        } \
    } \
} while(0)

#define MATRIX_MULTIPLY(a, b, result) do { \
    if ((a).cols != (b).rows) { \
        printf("Matrix dimensions incompatible for multiplication\n"); \
        return 1; \
    } \
    (result).rows = (a).rows; \
    (result).cols = (b).cols; \
    for (int i = 0; i < (a).rows; i++) { \
        for (int j = 0; j < (b).cols; j++) { \
            int sum = 0; \
            for (int k = 0; k < (a).cols; k++) { \
                long long product = (long long)MATRIX_GET(a, i, k) * (long long)MATRIX_GET(b, k, j); \
                if (product > INT_MAX || product < INT_MIN) { \
                    printf("Integer overflow detected\n"); \
                    return 1; \
                } \
                long long new_sum = (long long)sum + product; \
                if (new_sum > INT_MAX || new_sum < INT_MIN) { \
                    printf("Integer overflow detected\n"); \
                    return 1; \
                } \
                sum = (int)new_sum; \
            } \
            MATRIX_SET(result, i, j, sum); \
        } \
    } \
} while(0)

#define MATRIX_PRINT(m) do { \
    for (int i = 0; i < (m).rows; i++) { \
        for (int j = 0; j < (m).cols; j++) { \
            printf("%6d", MATRIX_GET(m, i, j)); \
        } \
        printf("\n"); \
    } \
} while(0)

#define READ_INT(prompt, var, min, max) do { \
    printf("%s", prompt); \
    if (scanf("%d", &(var)) != 1) { \
        printf("Invalid input\n"); \
        return 1; \
    } \
    if ((var) < (min) || (var) > (max)) { \
        printf("Value out of range [%d, %d]\n", min, max); \
        return 1; \
    } \
} while(0)

#define MATRIX_READ(m) do { \
    READ_INT("Enter number of rows (1-10): ", (m).rows, 1, MAX_ROWS); \
    READ_INT("Enter number of columns (1-10): ", (m).cols, 1, MAX_COLS); \
    printf("Enter matrix elements row by row:\n"); \
    for (int i = 0; i < (m).rows; i++) { \
        for (int j = 0; j < (m).cols; j++) { \
            READ_INT("", MATRIX_GET(m, i, j), INT_MIN, INT_MAX); \
        } \
    } \
} while(0)

int main() {
    Matrix mat1, mat2, result;
    int operation;
    
    printf("Matrix Operations Program\n");
    printf("1. Matrix Addition\n");
    printf("2. Matrix Multiplication\n");
    
    READ_INT("Select operation (1-2): ", operation, 1, 2);
    
    printf("Enter first matrix:\n");
    MAT