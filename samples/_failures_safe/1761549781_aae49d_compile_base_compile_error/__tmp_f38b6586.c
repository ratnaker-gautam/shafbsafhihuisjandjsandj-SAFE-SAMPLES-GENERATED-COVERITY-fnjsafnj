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
    int rows;
    int cols;
    int data[MATRIX_SIZE];
} Matrix;

#define MATRIX_INDEX(m, r, c) ((r) * (m).cols + (c))
#define MATRIX_ELEMENT(m, r, c) ((m).data[MATRIX_INDEX((m), (r), (c))])
#define VALID_ROW(m, r) ((r) >= 0 && (r) < (m).rows)
#define VALID_COL(m, c) ((c) >= 0 && (c) < (m).cols)
#define VALID_INDEX(m, r, c) (VALID_ROW((m), (r)) && VALID_COL((m), (c)))

void matrix_init(Matrix *m, int rows, int cols) {
    if (rows <= 0 || rows > MAX_ROWS || cols <= 0 || cols > MAX_COLS) {
        fprintf(stderr, "Invalid matrix dimensions\n");
        exit(EXIT_FAILURE);
    }
    m->rows = rows;
    m->cols = cols;
    memset(m->data, 0, sizeof(m->data));
}

void matrix_read(Matrix *m) {
    printf("Enter matrix dimensions (rows cols): ");
    if (scanf("%d %d", &m->rows, &m->cols) != 2) {
        fprintf(stderr, "Invalid input\n");
        exit(EXIT_FAILURE);
    }
    if (m->rows <= 0 || m->rows > MAX_ROWS || m->cols <= 0 || m->cols > MAX_COLS) {
        fprintf(stderr, "Invalid matrix dimensions\n");
        exit(EXIT_FAILURE);
    }
    printf("Enter %d elements:\n", m->rows * m->cols);
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            if (scanf("%d", &m->data[MATRIX_INDEX(*m, i, j)]) != 1) {
                fprintf(stderr, "Invalid input\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

void matrix_print(const Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%d ", MATRIX_ELEMENT(*m, i, j));
        }
        printf("\n");
    }
}

void matrix_add(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->rows != b->rows || a->cols != b->cols) {
        fprintf(stderr, "Matrix dimensions do not match for addition\n");
        exit(EXIT_FAILURE);
    }
    matrix_init(result, a->rows, a->cols);
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            int sum = MATRIX_ELEMENT(*a, i, j) + MATRIX_ELEMENT(*b, i, j);
            if ((MATRIX_ELEMENT(*a, i, j) > 0 && MATRIX_ELEMENT(*b, i, j) > INT_MAX - MATRIX_ELEMENT(*a, i, j)) ||
                (MATRIX_ELEMENT(*a, i, j) < 0 && MATRIX_ELEMENT(*b, i, j) < INT_MIN - MATRIX_ELEMENT(*a, i, j))) {
                fprintf(stderr, "Integer overflow in addition\n");
                exit(EXIT_FAILURE);
            }
            result->data[MATRIX_INDEX(*result, i, j)] = sum;
        }
    }
}

void matrix_multiply(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->cols != b->rows) {
        fprintf(stderr, "Matrix dimensions do not match for multiplication\n");
        exit(EXIT_FAILURE);
    }
    matrix_init(result, a->rows, b->cols);
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                long long product = (long long)MATRIX_ELEMENT(*a, i, k) * MATRIX_ELEMENT(*b, k, j);
                if (product > INT_MAX || product < INT_MIN) {
                    fprintf(stderr, "Integer overflow in multiplication\n");
                    exit(EXIT_FAILURE);
                }
                long long new_sum = (long long)sum + product;
                if (new_sum > INT_MAX || new_sum < INT_MIN) {
                    fprintf(stderr, "Integer overflow in multiplication\n");
                    exit(EXIT_FAILURE);
                }
                sum = (int)new_sum;
            }
            result->data[MATRIX_INDEX(*result, i, j)] = sum;
        }
    }
}

void matrix_transpose(const Matrix *m, Matrix *result) {
    matrix_init(result, m->cols,