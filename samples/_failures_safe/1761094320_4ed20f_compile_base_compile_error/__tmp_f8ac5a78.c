//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

enum matrix_op {
    MATRIX_ADD,
    MATRIX_SUBTRACT,
    MATRIX_MULTIPLY,
    MATRIX_TRANSPOSE,
    MATRIX_DETERMINANT
};

struct matrix {
    double **data;
    int rows;
    int cols;
};

int matrix_init(struct matrix *m, int rows, int cols) {
    if (rows <= 0 || cols <= 0) return 0;
    m->rows = rows;
    m->cols = cols;
    m->data = malloc(rows * sizeof(double*));
    if (m->data == NULL) return 0;
    for (int i = 0; i < rows; i++) {
        m->data[i] = malloc(cols * sizeof(double));
        if (m->data[i] == NULL) {
            for (int j = 0; j < i; j++) free(m->data[j]);
            free(m->data);
            return 0;
        }
    }
    return 1;
}

void matrix_free(struct matrix *m) {
    if (m->data != NULL) {
        for (int i = 0; i < m->rows; i++) free(m->data[i]);
        free(m->data);
    }
}

int matrix_input(struct matrix *m) {
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            if (scanf("%lf", &m->data[i][j]) != 1) return 0;
        }
    }
    return 1;
}

void matrix_print(struct matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%8.2f", m->data[i][j]);
        }
        printf("\n");
    }
}

int matrix_add(struct matrix *a, struct matrix *b, struct matrix *result) {
    if (a->rows != b->rows || a->cols != b->cols) return 0;
    if (!matrix_init(result, a->rows, a->cols)) return 0;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            result->data[i][j] = a->data[i][j] + b->data[i][j];
        }
    }
    return 1;
}

int matrix_subtract(struct matrix *a, struct matrix *b, struct matrix *result) {
    if (a->rows != b->rows || a->cols != b->cols) return 0;
    if (!matrix_init(result, a->rows, a->cols)) return 0;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            result->data[i][j] = a->data[i][j] - b->data[i][j];
        }
    }
    return 1;
}

int matrix_multiply(struct matrix *a, struct matrix *b, struct matrix *result) {
    if (a->cols != b->rows) return 0;
    if (!matrix_init(result, a->rows, b->cols)) return 0;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            result->data[i][j] = 0.0;
            for (int k = 0; k < a->cols; k++) {
                result->data[i][j] += a->data[i][k] * b->data[k][j];
            }
        }
    }
    return 1;
}

int matrix_transpose(struct matrix *a, struct matrix *result) {
    if (!matrix_init(result, a->cols, a->rows)) return 0;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            result->data[j][i] = a->data[i][j];
        }
    }
    return 1;
}

double matrix_determinant(struct matrix *a) {
    if (a->rows != a->cols) return NAN;
    int n = a->rows;
    if (n == 1) return a->data[0][0];
    if (n == 2) return a->data[0][0] * a->data[1][1] - a->data[0][1] * a->data[1][0];
    double det = 0.0;
    for (int p = 0; p < n; p++) {
        struct matrix sub;
        if (!matrix_init(&sub, n-1, n-1)) return NAN;
        for (int i = 1; i < n; i++) {
            int col = 0;
            for (int j = 0; j < n; j++) {
                if (j == p) continue;
                sub.data[i-1][col] = a->data[i][j];
                col++;
            }
        }
        double sign = (p %