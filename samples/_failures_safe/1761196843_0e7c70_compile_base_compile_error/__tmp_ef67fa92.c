//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

enum MatrixOp {
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_TRANSPOSE,
    OP_DETERMINANT
};

struct Matrix {
    int rows;
    int cols;
    int **data;
};

int validate_matrix(const struct Matrix *m) {
    if (m == NULL) return 0;
    if (m->rows <= 0 || m->cols <= 0) return 0;
    if (m->data == NULL) return 0;
    for (int i = 0; i < m->rows; i++) {
        if (m->data[i] == NULL) return 0;
    }
    return 1;
}

int safe_int_add(int a, int b) {
    if ((b > 0 && a > INT_MAX - b) || (b < 0 && a < INT_MIN - b)) {
        return 0;
    }
    return 1;
}

int safe_int_mult(int a, int b) {
    if (a > 0) {
        if (b > 0) {
            if (a > INT_MAX / b) return 0;
        } else {
            if (b < INT_MIN / a) return 0;
        }
    } else {
        if (b > 0) {
            if (a < INT_MIN / b) return 0;
        } else {
            if (a != 0 && b < INT_MAX / a) return 0;
        }
    }
    return 1;
}

struct Matrix *create_matrix(int rows, int cols) {
    if (rows <= 0 || cols <= 0) return NULL;
    struct Matrix *m = malloc(sizeof(struct Matrix));
    if (m == NULL) return NULL;
    m->rows = rows;
    m->cols = cols;
    m->data = malloc(rows * sizeof(int *));
    if (m->data == NULL) {
        free(m);
        return NULL;
    }
    for (int i = 0; i < rows; i++) {
        m->data[i] = malloc(cols * sizeof(int));
        if (m->data[i] == NULL) {
            for (int j = 0; j < i; j++) free(m->data[j]);
            free(m->data);
            free(m);
            return NULL;
        }
    }
    return m;
}

void free_matrix(struct Matrix *m) {
    if (m == NULL) return;
    if (m->data != NULL) {
        for (int i = 0; i < m->rows; i++) free(m->data[i]);
        free(m->data);
    }
    free(m);
}

int read_matrix(struct Matrix *m) {
    if (!validate_matrix(m)) return 0;
    printf("Enter %d x %d matrix elements:\n", m->rows, m->cols);
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            if (scanf("%d", &m->data[i][j]) != 1) return 0;
        }
    }
    return 1;
}

void print_matrix(const struct Matrix *m) {
    if (!validate_matrix(m)) return;
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%d ", m->data[i][j]);
        }
        printf("\n");
    }
}

struct Matrix *matrix_add(const struct Matrix *a, const struct Matrix *b) {
    if (!validate_matrix(a) || !validate_matrix(b)) return NULL;
    if (a->rows != b->rows || a->cols != b->cols) return NULL;
    struct Matrix *result = create_matrix(a->rows, a->cols);
    if (result == NULL) return NULL;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            if (!safe_int_add(a->data[i][j], b->data[i][j])) {
                free_matrix(result);
                return NULL;
            }
            result->data[i][j] = a->data[i][j] + b->data[i][j];
        }
    }
    return result;
}

struct Matrix *matrix_subtract(const struct Matrix *a, const struct Matrix *b) {
    if (!validate_matrix(a) || !validate_matrix(b)) return NULL;
    if (a->rows != b->rows || a->cols != b->cols) return NULL;
    struct Matrix *result = create_matrix(a->rows, a->cols);
    if (result == NULL) return NULL;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            if (!safe_int_add(a->data[i][j], -b->data[i][j])) {
                free_matrix(result);
                return NULL;
            }
            result->data[i][j] = a->data[i][j] - b->data[i][j];
        }
    }
    return result;
}

struct Matrix *matrix_multiply(const struct Matrix *a, const struct Matrix *b) {
    if (!validate_matrix(a) || !validate_matrix(b)) return NULL;
    if (a->cols != b->rows) return NULL;