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

#define MATRIX_VALID_DIMS(r, c) ((r) > 0 && (r) <= MAX_ROWS && (c) > 0 && (c) <= MAX_COLS)

#define MATRIX_GET(m, i, j) ((m).data[(i)][(j)])

#define MATRIX_SET(m, i, j, val) do { \
    if ((i) >= 0 && (i) < (m).rows && (j) >= 0 && (j) < (m).cols) \
        (m).data[(i)][(j)] = (val); \
} while(0)

#define MATRIX_ADD(a, b, result) do { \
    if ((a).rows != (b).rows || (a).cols != (b).cols) { \
        printf("Matrix dimensions mismatch for addition\n"); \
        return 1; \
    } \
    MATRIX_INIT((result), (a).rows, (a).cols); \
    for (int i = 0; i < (a).rows; i++) { \
        for (int j = 0; j < (a).cols; j++) { \
            int sum = MATRIX_GET(a, i, j) + MATRIX_GET(b, i, j); \
            if ((MATRIX_GET(a, i, j) > 0 && MATRIX_GET(b, i, j) > INT_MAX - MATRIX_GET(a, i, j)) || \
                (MATRIX_GET(a, i, j) < 0 && MATRIX_GET(b, i, j) < INT_MIN - MATRIX_GET(a, i, j))) { \
                printf("Integer overflow detected\n"); \
                return 1; \
            } \
            MATRIX_SET((result), i, j, sum); \
        } \
    } \
} while(0)

#define MATRIX_MULTIPLY(a, b, result) do { \
    if ((a).cols != (b).rows) { \
        printf("Matrix dimensions incompatible for multiplication\n"); \
        return 1; \
    } \
    MATRIX_INIT((result), (a).rows, (b).cols); \
    for (int i = 0; i < (a).rows; i++) { \
        for (int j = 0; j < (b).cols; j++) { \
            int sum = 0; \
            for (int k = 0; k < (a).cols; k++) { \
                long long product = (long long)MATRIX_GET(a, i, k) * MATRIX_GET(b, k, j); \
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
            MATRIX_SET((result), i, j, sum); \
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

int read_matrix(Matrix *m) {
    int rows, cols;
    printf("Enter number of rows (1-%d): ", MAX_ROWS);
    if (scanf("%d", &rows) != 1 || !MATRIX_VALID_DIMS(rows, 1)) {
        printf("Invalid rows\n");
        return 0;
    }
    printf("Enter number of columns (1-%d): ", MAX_COLS);
    if (scanf("%d", &cols) != 1 || !MATRIX_VALID_DIMS(1, cols)) {
        printf("Invalid columns\n");
        return 0;
    }
    
    MATRIX_INIT(*m, rows, cols);
    
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            if (scanf("%d", &m->data[i][j]) != 1) {
                printf("Invalid input\n");
                return 0;
            }
        }
    }
    return 1;
}

int main() {
    Matrix mat