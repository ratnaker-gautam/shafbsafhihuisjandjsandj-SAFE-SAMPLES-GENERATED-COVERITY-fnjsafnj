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

#define MATRIX_ADD(a, b, result) do { \
    if ((a).rows != (b).rows || (a).cols != (b).cols) { \
        printf("Matrix dimensions mismatch\n"); \
        (result).rows = 0; \
        (result).cols = 0; \
    } else { \
        (result).rows = (a).rows; \
        (result).cols = (a).cols; \
        for (int i = 0; i < (a).rows; i++) { \
            for (int j = 0; j < (a).cols; j++) { \
                MATRIX_SET(result, i, j, MATRIX_GET(a, i, j) + MATRIX_GET(b, i, j)); \
            } \
        } \
    } \
} while(0)

#define MATRIX_MULTIPLY(a, b, result) do { \
    if ((a).cols != (b).rows) { \
        printf("Matrix dimensions incompatible for multiplication\n"); \
        (result).rows = 0; \
        (result).cols = 0; \
    } else { \
        (result).rows = (a).rows; \
        (result).cols = (b).cols; \
        for (int i = 0; i < (a).rows; i++) { \
            for (int j = 0; j < (b).cols; j++) { \
                int sum = 0; \
                for (int k = 0; k < (a).cols; k++) { \
                    sum += MATRIX_GET(a, i, k) * MATRIX_GET(b, k, j); \
                } \
                MATRIX_SET(result, i, j, sum); \
            } \
        } \
    } \
} while(0)

#define MATRIX_PRINT(m) do { \
    if ((m).rows == 0 || (m).cols == 0) { \
        printf("Empty matrix\n"); \
    } else { \
        for (int i = 0; i < (m).rows; i++) { \
            for (int j = 0; j < (m).cols; j++) { \
                printf("%6d", MATRIX_GET(m, i, j)); \
            } \
            printf("\n"); \
        } \
    } \
} while(0)

#define MATRIX_INPUT(m) do { \
    printf("Enter number of rows (1-%d): ", MAX_ROWS); \
    if (scanf("%d", &(m).rows) != 1 || !VALID_DIM((m).rows, 1)) { \
        printf("Invalid rows\n"); \
        (m).rows = 0; \
        (m).cols = 0; \
        break; \
    } \
    printf("Enter number of columns (1-%d): ", MAX_COLS); \
    if (scanf("%d", &(m).cols) != 1 || !VALID_DIM(1, (m).cols)) { \
        printf("Invalid columns\n"); \
        (m).rows = 0; \
        (m).cols = 0; \
        break; \
    } \
    printf("Enter %d elements:\n", (m).rows * (m).cols); \
    for (int i = 0; i < (m).rows; i++) { \
        for (int j = 0; j < (m).cols; j++) { \
            if (scanf("%d", &(m).data[i][j]) != 1) { \
                printf("Invalid input\n"); \
                (m).rows = 0; \
                (m).cols = 0; \
                break; \
            } \
        } \
        if ((m).rows == 0) break; \
    } \
} while(0)

int main() {
    Matrix mat1 = MATRIX_INIT;
    Matrix mat2 = MATRIX_INIT;
    Matrix result = MATRIX_INIT;
    int choice;
    
    printf("Matrix 1:\n");
    MATRIX_INPUT(mat1);
    if (mat1.rows == 0 || mat1.cols == 0) {
        return 1;
    }
    
    printf("