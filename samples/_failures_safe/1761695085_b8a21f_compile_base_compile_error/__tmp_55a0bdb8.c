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

#define MATRIX_ADD(a, b, result) do { \
    if ((a).rows != (b).rows || (a).cols != (b).cols) { \
        fprintf(stderr, "Matrix dimensions mismatch for addition\n"); \
        return 1; \
    } \
    for (int i = 0; i < (a).rows; i++) { \
        for (int j = 0; j < (a).cols; j++) { \
            if (((a).data[i][j] > 0 && (b).data[i][j] > INT_MAX - (a).data[i][j]) || \
                ((a).data[i][j] < 0 && (b).data[i][j] < INT_MIN - (a).data[i][j])) { \
                fprintf(stderr, "Integer overflow in addition\n"); \
                return 1; \
            } \
            (result).data[i][j] = (a).data[i][j] + (b).data[i][j]; \
        } \
    } \
    (result).rows = (a).rows; \
    (result).cols = (a).cols; \
} while(0)

#define MATRIX_MULTIPLY(a, b, result) do { \
    if ((a).cols != (b).rows) { \
        fprintf(stderr, "Matrix dimensions incompatible for multiplication\n"); \
        return 1; \
    } \
    for (int i = 0; i < (a).rows; i++) { \
        for (int j = 0; j < (b).cols; j++) { \
            int sum = 0; \
            for (int k = 0; k < (a).cols; k++) { \
                if ((a).data[i][k] != 0 && (b).data[k][j] != 0) { \
                    if (((a).data[i][k] > 0) == ((b).data[k][j] > 0)) { \
                        if ((a).data[i][k] > INT_MAX / (b).data[k][j]) { \
                            fprintf(stderr, "Integer overflow in multiplication\n"); \
                            return 1; \
                        } \
                    } else { \
                        if ((a).data[i][k] < INT_MIN / (b).data[k][j]) { \
                            fprintf(stderr, "Integer overflow in multiplication\n"); \
                            return 1; \
                        } \
                    } \
                    int product = (a).data[i][k] * (b).data[k][j]; \
                    if ((product > 0 && sum > INT_MAX - product) || \
                        (product < 0 && sum < INT_MIN - product)) { \
                        fprintf(stderr, "Integer overflow in sum\n"); \
                        return 1; \
                    } \
                    sum += product; \
                } \
            } \
            (result).data[i][j] = sum; \
        } \
    } \
    (result).rows = (a).rows; \
    (result).cols = (b).cols; \
} while(0)

#define MATRIX_PRINT(m) do { \
    for (int i = 0; i < (m).rows; i++) { \
        for (int j = 0; j < (m).cols; j++) { \
            printf("%6d", (m).data[i][j]); \
        } \
        printf("\n"); \
    } \
} while(0)

int read_matrix(Matrix *m) {
    int rows, cols;
    printf("Enter number of rows (1-%d): ", MAX_ROWS);
    if (scanf("%d", &rows) != 1) {
        fprintf(stderr, "Invalid input for rows\n");
        return 0;
    }
    printf("Enter number of columns (1-%d): ", MAX_COLS);
    if (scanf("%d", &cols) != 1) {
        fprintf(stderr, "Invalid input for columns\n");
        return 0;
    }
    if (!MATRIX_VALID_DIMS(rows, cols)) {
        fprintf(stderr, "Invalid matrix dimensions\n");
        return 0;
    }
    m->rows = rows;
    m->cols = cols;
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scanf("%d", &m->data[i][j]) != 1) {
                fprintf(stderr, "Invalid matrix element\n");
                return 0;
            }
        }
    }
    return 1;
}

int main(void) {