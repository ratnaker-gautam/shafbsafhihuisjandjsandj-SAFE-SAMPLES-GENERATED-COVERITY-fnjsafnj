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

#define MATRIX_GET(m, r, c) (VALID_INDEX(m, r, c) ? (m).data[r][c] : 0)
#define MATRIX_SET(m, r, c, v) do { if (VALID_INDEX(m, r, c)) (m).data[r][c] = (v); } while(0)

#define MATRIX_ZERO(m) do { \
    memset((m).data, 0, sizeof((m).data)); \
    (m).rows = 0; \
    (m).cols = 0; \
} while(0)

#define MATRIX_IDENTITY(m, size) do { \
    if (VALID_DIM(size, size)) { \
        MATRIX_ZERO(m); \
        (m).rows = size; \
        (m).cols = size; \
        for (int i = 0; i < size; i++) { \
            (m).data[i][i] = 1; \
        } \
    } \
} while(0)

#define MATRIX_ADD(a, b, result) do { \
    if ((a).rows == (b).rows && (a).cols == (b).cols && VALID_DIM((a).rows, (a).cols)) { \
        (result).rows = (a).rows; \
        (result).cols = (a).cols; \
        for (int i = 0; i < (a).rows; i++) { \
            for (int j = 0; j < (a).cols; j++) { \
                int sum = (a).data[i][j] + (b).data[i][j]; \
                if (((a).data[i][j] > 0 && (b).data[i][j] > INT_MAX - (a).data[i][j]) || \
                    ((a).data[i][j] < 0 && (b).data[i][j] < INT_MIN - (a).data[i][j])) { \
                    sum = 0; \
                } \
                (result).data[i][j] = sum; \
            } \
        } \
    } else { \
        MATRIX_ZERO(result); \
    } \
} while(0)

#define MATRIX_MULTIPLY(a, b, result) do { \
    if ((a).cols == (b).rows && VALID_DIM((a).rows, (b).cols)) { \
        (result).rows = (a).rows; \
        (result).cols = (b).cols; \
        for (int i = 0; i < (a).rows; i++) { \
            for (int j = 0; j < (b).cols; j++) { \
                int sum = 0; \
                for (int k = 0; k < (a).cols; k++) { \
                    long long product = (long long)(a).data[i][k] * (long long)(b).data[k][j]; \
                    if (product > INT_MAX || product < INT_MIN) { \
                        sum = 0; \
                        break; \
                    } \
                    sum += (int)product; \
                    if ((sum > 0 && (int)product > INT_MAX - sum) || \
                        (sum < 0 && (int)product < INT_MIN - sum)) { \
                        sum = 0; \
                        break; \
                    } \
                } \
                (result).data[i][j] = sum; \
            } \
        } \
    } else { \
        MATRIX_ZERO(result); \
    } \
} while(0)

#define MATRIX_TRANSPOSE(m, result) do { \
    if (VALID_DIM((m).rows, (m).cols)) { \
        (result).rows = (m).cols; \
        (result).cols = (m).rows; \
        for (int i = 0; i < (m).rows; i++) { \
            for (int j = 0; j < (m).cols; j++) { \
                (result).data[j][i] = (m).data[i][j]; \
            } \
        } \
    } else { \
        MATRIX_ZERO(result); \
    } \
} while(0)

#define MATRIX_PRINT(m) do { \
    if (VALID_DIM((m).rows, (m).cols)) { \
        for (int i = 0; i < (m).rows; i++) { \
            for (int j = 0; j < (m).cols; j++) { \
                printf("%6d", (m).data[i][j]);