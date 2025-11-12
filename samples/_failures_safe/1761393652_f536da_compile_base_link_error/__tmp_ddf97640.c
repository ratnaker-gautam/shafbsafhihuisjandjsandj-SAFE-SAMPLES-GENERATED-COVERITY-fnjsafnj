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

void matrix_init(Matrix *m, int rows, int cols) {
    if (rows <= 0 || rows > MAX_ROWS || cols <= 0 || cols > MAX_COLS) {
        fprintf(stderr, "Invalid matrix dimensions\n");
        exit(EXIT_FAILURE);
    }
    m->rows = rows;
    m->cols = cols;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            m->data[i][j] = 0;
        }
    }
}

void matrix_read(Matrix *m) {
    printf("Enter matrix elements (%d x %d):\n", m->rows, m->cols);
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            char buffer[32];
            if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                fprintf(stderr, "Input error\n");
                exit(EXIT_FAILURE);
            }
            char *endptr;
            long val = strtol(buffer, &endptr, 10);
            if (endptr == buffer || *endptr != '\n') {
                fprintf(stderr, "Invalid input\n");
                exit(EXIT_FAILURE);
            }
            if (val < INT_MIN || val > INT_MAX) {
                fprintf(stderr, "Value out of range\n");
                exit(EXIT_FAILURE);
            }
            m->data[i][j] = (int)val;
        }
    }
}

void matrix_print(const Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%6d", m->data[i][j]);
        }
        printf("\n");
    }
}

#define MATRIX_ADD(dest, a, b) do { \
    if ((a)->rows != (b)->rows || (a)->cols != (b)->cols) { \
        fprintf(stderr, "Matrix dimensions mismatch\n"); \
        exit(EXIT_FAILURE); \
    } \
    matrix_init(dest, (a)->rows, (a)->cols); \
    for (int i = 0; i < (a)->rows; i++) { \
        for (int j = 0; j < (a)->cols; j++) { \
            if (((a)->data[i][j] > 0 && (b)->data[i][j] > INT_MAX - (a)->data[i][j]) || \
                ((a)->data[i][j] < 0 && (b)->data[i][j] < INT_MIN - (a)->data[i][j])) { \
                fprintf(stderr, "Integer overflow\n"); \
                exit(EXIT_FAILURE); \
            } \
            (dest)->data[i][j] = (a)->data[i][j] + (b)->data[i][j]; \
        } \
    } \
} while(0)

#define MATRIX_MULTIPLY(dest, a, b) do { \
    if ((a)->cols != (b)->rows) { \
        fprintf(stderr, "Matrix dimensions incompatible\n"); \
        exit(EXIT_FAILURE); \
    } \
    matrix_init(dest, (a)->rows, (b)->cols); \
    for (int i = 0; i < (a)->rows; i++) { \
        for (int j = 0; j < (b)->cols; j++) { \
            long sum = 0; \
            for (int k = 0; k < (a)->cols; k++) { \
                long product = (long)(a)->data[i][k] * (long)(b)->data[k][j]; \
                if ((a)->data[i][k] != 0 && product / (a)->data[i][k] != (b)->data[k][j]) { \
                    fprintf(stderr, "Multiplication overflow\n"); \
                    exit(EXIT_FAILURE); \
                } \
                if ((sum > 0 && product > LONG_MAX - sum) || (sum < 0 && product < LONG_MIN - sum)) { \
                    fprintf(stderr, "Summation overflow\n"); \
                    exit(EXIT_FAILURE); \
                } \
                sum += product; \
            } \
            if (sum < INT_MIN || sum > INT_MAX) { \
                fprintf(stderr, "Result out of integer range\n"); \
                exit(EXIT_FAILURE); \
            } \
            (dest)->data[i][j] = (int)sum; \
        } \
    } \
} while(0)

#define MATRIX_TRANSPOSE(dest, src) do { \
    matrix_init(dest, (src)->cols, (src)->rows); \
    for (int i = 0; i < (src)->rows; i++) { \
        for (int j = 0; j < (src)->cols; j++) { \
            (dest)->data[j][i] = (src)->data[i][j]; \
        } \