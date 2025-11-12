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
                int product = MATRIX_GET(a, i, k) * MATRIX_GET(b, k, j); \
                if (MATRIX_GET(a, i, k) != 0 && product / MATRIX_GET(a, i, k) != MATRIX_GET(b, k, j)) { \
                    printf("Integer overflow detected\n"); \
                    return 1; \
                } \
                if ((sum > 0 && product > INT_MAX - sum) || (sum < 0 && product < INT_MIN - sum)) { \
                    printf("Integer overflow detected\n"); \
                    return 1; \
                } \
                sum += product; \
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

int read_matrix(Matrix *m) {
    printf("Enter number of rows (1-%d): ", MAX_ROWS);
    if (scanf("%d", &m->rows) != 1 || m->rows < 1 || m->rows > MAX_ROWS) {
        printf("Invalid number of rows\n");
        return 1;
    }
    
    printf("Enter number of columns (1-%d): ", MAX_COLS);
    if (scanf("%d", &m->cols) != 1 || m->cols < 1 || m->cols > MAX_COLS) {
        printf("Invalid number of columns\n");
        return 1;
    }
    
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            if (scanf("%d", &m->data[i][j]) != 1) {
                printf("Invalid input\n");
                return 1;
            }
        }
    }
    return 0;
}

int main() {
    Matrix mat1, mat2, result;
    int operation;
    
    printf("Matrix Operations Program\n");
    printf("1. Matrix Addition\n");
    printf("2. Matrix Multiplication\n");
    printf("Select operation (1-2): ");
    
    if (scanf("%d", &operation) != 1 || (operation != 1