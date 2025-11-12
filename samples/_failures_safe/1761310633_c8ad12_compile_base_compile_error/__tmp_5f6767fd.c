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

#define VALID_DIMENSIONS(r, c) ((r) > 0 && (r) <= MAX_ROWS && (c) > 0 && (c) <= MAX_COLS)
#define VALID_INDEX(m, r, c) ((r) >= 0 && (r) < (m).rows && (c) >= 0 && (c) < (m).cols)

#define PRINT_MATRIX(m) do { \
    for (int i = 0; i < (m).rows; i++) { \
        for (int j = 0; j < (m).cols; j++) { \
            printf("%6d", (m).data[i][j]); \
        } \
        printf("\n"); \
    } \
} while(0)

#define INPUT_MATRIX(m) do { \
    printf("Enter matrix elements row by row:\n"); \
    for (int i = 0; i < (m).rows; i++) { \
        for (int j = 0; j < (m).cols; j++) { \
            if (scanf("%d", &(m).data[i][j]) != 1) { \
                printf("Invalid input\n"); \
                return 1; \
            } \
        } \
    } \
} while(0)

#define ADD_MATRICES(a, b, result) do { \
    if ((a).rows != (b).rows || (a).cols != (b).cols) { \
        printf("Matrix dimensions don't match for addition\n"); \
        return 1; \
    } \
    (result).rows = (a).rows; \
    (result).cols = (a).cols; \
    for (int i = 0; i < (a).rows; i++) { \
        for (int j = 0; j < (a).cols; j++) { \
            if (((a).data[i][j] > 0 && (b).data[i][j] > INT_MAX - (a).data[i][j]) || \
                ((a).data[i][j] < 0 && (b).data[i][j] < INT_MIN - (a).data[i][j])) { \
                printf("Integer overflow detected\n"); \
                return 1; \
            } \
            (result).data[i][j] = (a).data[i][j] + (b).data[i][j]; \
        } \
    } \
} while(0)

#define MULTIPLY_MATRICES(a, b, result) do { \
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
                long long product = (long long)(a).data[i][k] * (long long)(b).data[k][j]; \
                if (product > INT_MAX || product < INT_MIN) { \
                    printf("Integer overflow detected\n"); \
                    return 1; \
                } \
                if ((sum > 0 && product > INT_MAX - sum) || (sum < 0 && product < INT_MIN - sum)) { \
                    printf("Integer overflow detected\n"); \
                    return 1; \
                } \
                sum += (int)product; \
            } \
            (result).data[i][j] = sum; \
        } \
    } \
} while(0)

#define TRANSPOSE_MATRIX(m, result) do { \
    (result).rows = (m).cols; \
    (result).cols = (m).rows; \
    for (int i = 0; i < (m).rows; i++) { \
        for (int j = 0; j < (m).cols; j++) { \
            (result).data[j][i] = (m).data[i][j]; \
        } \
    } \
} while(0)

int main() {
    Matrix mat1 = MATRIX_INIT;
    Matrix mat2 = MATRIX_INIT;
    Matrix result = MATRIX_INIT;
    int choice;
    
    printf("Enter dimensions for first matrix (rows columns): ");
    if (scanf("%d %d", &mat1.rows, &mat1.cols) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    if (!VALID_DIMENSIONS(mat1.rows, mat1.cols)) {
        printf("Invalid matrix dimensions\n");
        return 1;
    }
    INPUT_MATRIX(mat1);
    
    printf("Enter dimensions for second matrix (rows columns): ");
    if (scanf("%d %d", &mat2