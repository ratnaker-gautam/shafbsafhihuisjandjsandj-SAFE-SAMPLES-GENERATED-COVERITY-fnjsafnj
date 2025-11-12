//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 10
#define MATRIX_ELEMENT_MAX 1000
#define MATRIX_ELEMENT_MIN -1000

#define MATRIX_OP_ADD 1
#define MATRIX_OP_SUBTRACT 2
#define MATRIX_OP_MULTIPLY 3

#define CHECK_BOUNDS(idx, max) if ((idx) < 0 || (idx) >= (max)) return 0
#define CHECK_NULL(ptr) if ((ptr) == NULL) return 0

typedef struct {
    int rows;
    int cols;
    int data[MAX_SIZE][MAX_SIZE];
} Matrix;

int matrix_init(Matrix *m, int rows, int cols) {
    CHECK_NULL(m);
    if (rows <= 0 || rows > MAX_SIZE || cols <= 0 || cols > MAX_SIZE) return 0;
    m->rows = rows;
    m->cols = cols;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            m->data[i][j] = 0;
        }
    }
    return 1;
}

int matrix_set(Matrix *m, int row, int col, int value) {
    CHECK_NULL(m);
    CHECK_BOUNDS(row, m->rows);
    CHECK_BOUNDS(col, m->cols);
    if (value < MATRIX_ELEMENT_MIN || value > MATRIX_ELEMENT_MAX) return 0;
    m->data[row][col] = value;
    return 1;
}

int matrix_get(const Matrix *m, int row, int col, int *value) {
    CHECK_NULL(m);
    CHECK_NULL(value);
    CHECK_BOUNDS(row, m->rows);
    CHECK_BOUNDS(col, m->cols);
    *value = m->data[row][col];
    return 1;
}

int matrix_add(const Matrix *a, const Matrix *b, Matrix *result) {
    CHECK_NULL(a);
    CHECK_NULL(b);
    CHECK_NULL(result);
    if (a->rows != b->rows || a->cols != b->cols) return 0;
    if (!matrix_init(result, a->rows, a->cols)) return 0;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            int sum = a->data[i][j] + b->data[i][j];
            if (sum < MATRIX_ELEMENT_MIN || sum > MATRIX_ELEMENT_MAX) return 0;
            result->data[i][j] = sum;
        }
    }
    return 1;
}

int matrix_multiply(const Matrix *a, const Matrix *b, Matrix *result) {
    CHECK_NULL(a);
    CHECK_NULL(b);
    CHECK_NULL(result);
    if (a->cols != b->rows) return 0;
    if (!matrix_init(result, a->rows, b->cols)) return 0;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                int product = a->data[i][k] * b->data[k][j];
                if (a->data[i][k] != 0 && product / a->data[i][k] != b->data[k][j]) return 0;
                if (sum > 0 && product > INT_MAX - sum) return 0;
                if (sum < 0 && product < INT_MIN - sum) return 0;
                sum += product;
                if (sum < MATRIX_ELEMENT_MIN || sum > MATRIX_ELEMENT_MAX) return 0;
            }
            result->data[i][j] = sum;
        }
    }
    return 1;
}

int matrix_subtract(const Matrix *a, const Matrix *b, Matrix *result) {
    CHECK_NULL(a);
    CHECK_NULL(b);
    CHECK_NULL(result);
    if (a->rows != b->rows || a->cols != b->cols) return 0;
    if (!matrix_init(result, a->rows, a->cols)) return 0;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            int diff = a->data[i][j] - b->data[i][j];
            if (diff < MATRIX_ELEMENT_MIN || diff > MATRIX_ELEMENT_MAX) return 0;
            result->data[i][j] = diff;
        }
    }
    return 1;
}

void matrix_print(const Matrix *m) {
    if (m == NULL) return;
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%6d", m->data[i][j]);
        }
        printf("\n");
    }
}

int read_int(const char *prompt, int min, int max) {
    char buffer[32];
    int value;
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL