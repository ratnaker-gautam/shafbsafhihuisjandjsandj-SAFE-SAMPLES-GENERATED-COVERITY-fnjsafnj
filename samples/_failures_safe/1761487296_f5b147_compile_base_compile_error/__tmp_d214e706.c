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

#define VALIDATE_MATRIX(m) ((m).rows > 0 && (m).rows <= MAX_ROWS && (m).cols > 0 && (m).cols <= MAX_COLS)
#define MATRIX_ELEMENTS(m) ((m).rows * (m).cols)

void matrix_init(Matrix *m, int rows, int cols) {
    if (rows <= 0 || rows > MAX_ROWS || cols <= 0 || cols > MAX_COLS) {
        m->rows = 0;
        m->cols = 0;
        return;
    }
    m->rows = rows;
    m->cols = cols;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            m->data[i][j] = 0;
        }
    }
}

int matrix_input(Matrix *m) {
    int rows, cols;
    printf("Enter matrix dimensions (rows cols, max %dx%d): ", MAX_ROWS, MAX_COLS);
    if (scanf("%d %d", &rows, &cols) != 2) {
        return 0;
    }
    if (rows <= 0 || rows > MAX_ROWS || cols <= 0 || cols > MAX_COLS) {
        return 0;
    }
    matrix_init(m, rows, cols);
    printf("Enter %d elements:\n", MATRIX_ELEMENTS(*m));
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            if (scanf("%d", &m->data[i][j]) != 1) {
                return 0;
            }
        }
    }
    return 1;
}

void matrix_print(const Matrix *m) {
    if (!VALIDATE_MATRIX(*m)) {
        printf("Invalid matrix\n");
        return;
    }
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%6d", m->data[i][j]);
        }
        printf("\n");
    }
}

int matrix_add(const Matrix *a, const Matrix *b, Matrix *result) {
    if (!VALIDATE_MATRIX(*a) || !VALIDATE_MATRIX(*b)) {
        return 0;
    }
    if (a->rows != b->rows || a->cols != b->cols) {
        return 0;
    }
    matrix_init(result, a->rows, a->cols);
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            if ((a->data[i][j] > 0 && b->data[i][j] > INT_MAX - a->data[i][j]) ||
                (a->data[i][j] < 0 && b->data[i][j] < INT_MIN - a->data[i][j])) {
                return 0;
            }
            result->data[i][j] = a->data[i][j] + b->data[i][j];
        }
    }
    return 1;
}

int matrix_multiply(const Matrix *a, const Matrix *b, Matrix *result) {
    if (!VALIDATE_MATRIX(*a) || !VALIDATE_MATRIX(*b)) {
        return 0;
    }
    if (a->cols != b->rows) {
        return 0;
    }
    matrix_init(result, a->rows, b->cols);
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                long long product = (long long)a->data[i][k] * b->data[k][j];
                if (product > INT_MAX || product < INT_MIN) {
                    return 0;
                }
                if ((product > 0 && sum > INT_MAX - product) ||
                    (product < 0 && sum < INT_MIN - product)) {
                    return 0;
                }
                sum += (int)product;
            }
            result->data[i][j] = sum;
        }
    }
    return 1;
}

int matrix_transpose(const Matrix *m, Matrix *result) {
    if (!VALIDATE_MATRIX(*m)) {
        return 0;
    }
    matrix_init(result, m->cols, m->rows);
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            result->data[j][i] = m->data[i][j];
        }
    }
    return 1;
}

int main() {
    Matrix a = MATRIX_INIT;
    Matrix b = MAT