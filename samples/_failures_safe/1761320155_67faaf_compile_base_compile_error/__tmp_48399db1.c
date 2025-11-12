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
    OP_EXIT
};

struct Matrix {
    int rows;
    int cols;
    int **data;
};

int validate_dimensions(int rows, int cols) {
    if (rows <= 0 || cols <= 0) return 0;
    if (rows > 100 || cols > 100) return 0;
    return 1;
}

struct Matrix *create_matrix(int rows, int cols) {
    if (!validate_dimensions(rows, cols)) return NULL;
    
    struct Matrix *m = malloc(sizeof(struct Matrix));
    if (!m) return NULL;
    
    m->rows = rows;
    m->cols = cols;
    m->data = malloc(rows * sizeof(int *));
    if (!m->data) {
        free(m);
        return NULL;
    }
    
    for (int i = 0; i < rows; i++) {
        m->data[i] = malloc(cols * sizeof(int));
        if (!m->data[i]) {
            for (int j = 0; j < i; j++) free(m->data[j]);
            free(m->data);
            free(m);
            return NULL;
        }
    }
    return m;
}

void free_matrix(struct Matrix *m) {
    if (!m) return;
    for (int i = 0; i < m->rows; i++) free(m->data[i]);
    free(m->data);
    free(m);
}

int read_matrix(struct Matrix *m) {
    if (!m) return 0;
    
    printf("Enter %d x %d matrix elements:\n", m->rows, m->cols);
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            if (scanf("%d", &m->data[i][j]) != 1) return 0;
        }
    }
    return 1;
}

void print_matrix(struct Matrix *m) {
    if (!m) return;
    
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%6d", m->data[i][j]);
        }
        printf("\n");
    }
}

struct Matrix *transpose_matrix(struct Matrix *m) {
    if (!m) return NULL;
    
    struct Matrix *result = create_matrix(m->cols, m->rows);
    if (!result) return NULL;
    
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            result->data[j][i] = m->data[i][j];
        }
    }
    return result;
}

struct Matrix *add_matrices(struct Matrix *a, struct Matrix *b) {
    if (!a || !b) return NULL;
    if (a->rows != b->rows || a->cols != b->cols) return NULL;
    
    struct Matrix *result = create_matrix(a->rows, a->cols);
    if (!result) return NULL;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            if ((a->data[i][j] > 0 && b->data[i][j] > INT_MAX - a->data[i][j]) ||
                (a->data[i][j] < 0 && b->data[i][j] < INT_MIN - a->data[i][j])) {
                free_matrix(result);
                return NULL;
            }
            result->data[i][j] = a->data[i][j] + b->data[i][j];
        }
    }
    return result;
}

struct Matrix *subtract_matrices(struct Matrix *a, struct Matrix *b) {
    if (!a || !b) return NULL;
    if (a->rows != b->rows || a->cols != b->cols) return NULL;
    
    struct Matrix *result = create_matrix(a->rows, a->cols);
    if (!result) return NULL;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            if ((b->data[i][j] > 0 && a->data[i][j] < INT_MIN + b->data[i][j]) ||
                (b->data[i][j] < 0 && a->data[i][j] > INT_MAX + b->data[i][j])) {
                free_matrix(result);
                return NULL;
            }
            result->data[i][j] = a->data[i][j] - b->data[i][j];
        }
    }
    return result;
}

struct Matrix *multiply_matrices(struct Matrix *a, struct Matrix *b) {
    if (!a || !b) return NULL;
    if (a->cols != b->rows) return NULL;
    
    struct Matrix *result = create_matrix(a->rows, b->cols);
    if (!result) return NULL;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for