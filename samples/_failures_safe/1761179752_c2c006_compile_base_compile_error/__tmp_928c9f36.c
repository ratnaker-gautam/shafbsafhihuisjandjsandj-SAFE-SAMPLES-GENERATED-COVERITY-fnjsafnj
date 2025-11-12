//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ROWS 10
#define MAX_COLS 10
#define MIN_DIM 1
#define MATRIX_SIZE (MAX_ROWS * MAX_COLS)

typedef struct {
    int data[MAX_ROWS][MAX_COLS];
    int rows;
    int cols;
} Matrix;

#define MATRIX_GET(m, r, c) ((r) >= 0 && (r) < (m).rows && (c) >= 0 && (c) < (m).cols ? (m).data[r][c] : 0)
#define MATRIX_SET(m, r, c, v) do { \
    if ((r) >= 0 && (r) < (m).rows && (c) >= 0 && (c) < (m).cols) { \
        (m).data[r][c] = (v); \
    } \
} while(0)

#define PRINT_MATRIX(m) do { \
    for (int i = 0; i < (m).rows; i++) { \
        for (int j = 0; j < (m).cols; j++) { \
            printf("%6d", MATRIX_GET((m), i, j)); \
        } \
        printf("\n"); \
    } \
} while(0)

#define READ_MATRIX(m) do { \
    for (int i = 0; i < (m).rows; i++) { \
        for (int j = 0; j < (m).cols; j++) { \
            int val; \
            if (scanf("%d", &val) != 1) { \
                fprintf(stderr, "Invalid input\n"); \
                exit(EXIT_FAILURE); \
            } \
            MATRIX_SET((m), i, j, val); \
        } \
    } \
} while(0)

#define MATRIX_ADD(a, b, result) do { \
    if ((a).rows != (b).rows || (a).cols != (b).cols) { \
        fprintf(stderr, "Matrix dimensions mismatch\n"); \
        exit(EXIT_FAILURE); \
    } \
    (result).rows = (a).rows; \
    (result).cols = (a).cols; \
    for (int i = 0; i < (a).rows; i++) { \
        for (int j = 0; j < (a).cols; j++) { \
            int sum = MATRIX_GET((a), i, j) + MATRIX_GET((b), i, j); \
            if ((MATRIX_GET((a), i, j) > 0 && MATRIX_GET((b), i, j) > INT_MAX - MATRIX_GET((a), i, j)) || \
                (MATRIX_GET((a), i, j) < 0 && MATRIX_GET((b), i, j) < INT_MIN - MATRIX_GET((a), i, j))) { \
                fprintf(stderr, "Integer overflow\n"); \
                exit(EXIT_FAILURE); \
            } \
            MATRIX_SET((result), i, j, sum); \
        } \
    } \
} while(0)

#define MATRIX_MULTIPLY(a, b, result) do { \
    if ((a).cols != (b).rows) { \
        fprintf(stderr, "Matrix dimensions incompatible for multiplication\n"); \
        exit(EXIT_FAILURE); \
    } \
    (result).rows = (a).rows; \
    (result).cols = (b).cols; \
    for (int i = 0; i < (a).rows; i++) { \
        for (int j = 0; j < (b).cols; j++) { \
            int sum = 0; \
            for (int k = 0; k < (a).cols; k++) { \
                int product = MATRIX_GET((a), i, k) * MATRIX_GET((b), k, j); \
                if (MATRIX_GET((a), i, k) != 0 && product / MATRIX_GET((a), i, k) != MATRIX_GET((b), k, j)) { \
                    fprintf(stderr, "Integer overflow\n"); \
                    exit(EXIT_FAILURE); \
                } \
                if ((product > 0 && sum > INT_MAX - product) || (product < 0 && sum < INT_MIN - product)) { \
                    fprintf(stderr, "Integer overflow\n"); \
                    exit(EXIT_FAILURE); \
                } \
                sum += product; \
            } \
            MATRIX_SET((result), i, j, sum); \
        } \
    } \
} while(0)

int main() {
    Matrix mat1, mat2, result;
    
    printf("Enter number of rows for matrix 1 (1-%d): ", MAX_ROWS);
    if (scanf("%d", &mat1.rows) != 1 || mat1.rows < MIN_DIM || mat1.rows > MAX_ROWS) {
        fprintf(stderr, "Invalid row count\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter number of columns for matrix 1 (1-%d): ", MAX_COLS);
    if (scanf("%d", &mat1.cols) != 1 || mat1.col