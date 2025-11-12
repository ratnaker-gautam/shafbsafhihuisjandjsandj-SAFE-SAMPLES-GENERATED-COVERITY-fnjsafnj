//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

struct Matrix {
    size_t rows;
    size_t cols;
    double *data;
};

struct MatrixOps {
    struct Matrix (*create)(size_t, size_t);
    void (*destroy)(struct Matrix*);
    int (*multiply)(const struct Matrix*, const struct Matrix*, struct Matrix*);
    int (*transpose)(const struct Matrix*, struct Matrix*);
    void (*print)(const struct Matrix*);
};

struct Matrix matrix_create(size_t rows, size_t cols) {
    struct Matrix m = {0, 0, NULL};
    if (rows == 0 || cols == 0 || rows > SIZE_MAX / cols / sizeof(double)) {
        return m;
    }
    m.data = malloc(rows * cols * sizeof(double));
    if (m.data == NULL) {
        return m;
    }
    m.rows = rows;
    m.cols = cols;
    for (size_t i = 0; i < rows * cols; i++) {
        m.data[i] = 0.0;
    }
    return m;
}

void matrix_destroy(struct Matrix *m) {
    if (m != NULL) {
        free(m->data);
        m->data = NULL;
        m->rows = 0;
        m->cols = 0;
    }
}

int matrix_multiply(const struct Matrix *a, const struct Matrix *b, struct Matrix *result) {
    if (a == NULL || b == NULL || result == NULL) {
        return 0;
    }
    if (a->cols != b->rows) {
        return 0;
    }
    if (result->rows != a->rows || result->cols != b->cols) {
        return 0;
    }
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < b->cols; j++) {
            double sum = 0.0;
            for (size_t k = 0; k < a->cols; k++) {
                sum += a->data[i * a->cols + k] * b->data[k * b->cols + j];
            }
            result->data[i * result->cols + j] = sum;
        }
    }
    return 1;
}

int matrix_transpose(const struct Matrix *m, struct Matrix *result) {
    if (m == NULL || result == NULL) {
        return 0;
    }
    if (result->rows != m->cols || result->cols != m->rows) {
        return 0;
    }
    for (size_t i = 0; i < m->rows; i++) {
        for (size_t j = 0; j < m->cols; j++) {
            result->data[j * result->cols + i] = m->data[i * m->cols + j];
        }
    }
    return 1;
}

void matrix_print(const struct Matrix *m) {
    if (m == NULL) {
        return;
    }
    for (size_t i = 0; i < m->rows; i++) {
        for (size_t j = 0; j < m->cols; j++) {
            printf("%8.2f", m->data[i * m->cols + j]);
        }
        printf("\n");
    }
}

struct MatrixOps init_ops(void) {
    struct MatrixOps ops;
    ops.create = matrix_create;
    ops.destroy = matrix_destroy;
    ops.multiply = matrix_multiply;
    ops.transpose = matrix_transpose;
    ops.print = matrix_print;
    return ops;
}

int main(void) {
    struct MatrixOps ops = init_ops();
    
    struct Matrix a = ops.create(2, 3);
    if (a.data == NULL) {
        return 1;
    }
    a.data[0] = 1.0; a.data[1] = 2.0; a.data[2] = 3.0;
    a.data[3] = 4.0; a.data[4] = 5.0; a.data[5] = 6.0;
    
    struct Matrix b = ops.create(3, 2);
    if (b.data == NULL) {
        ops.destroy(&a);
        return 1;
    }
    b.data[0] = 7.0; b.data[1] = 8.0;
    b.data[2] = 9.0; b.data[3] = 10.0;
    b.data[4] = 11.0; b.data[5] = 12.0;
    
    struct Matrix c = ops.create(2, 2);
    if (c.data == NULL) {
        ops.destroy(&a);
        ops.destroy(&b);
        return 1;
    }
    
    printf("Matrix A:\n");
    ops.print(&a);
    printf("Matrix B:\n");
    ops.print(&b);
    
    if (ops.multiply(&a, &b, &c)) {
        printf("Matrix A * B:\n");
        ops.print(&c);
    } else {
        printf("Matrix multiplication failed.\n");
    }
    
    struct Matrix d = ops.create(3, 2);
    if (d.data == NULL) {
        ops.destroy(&a);
        ops.destroy(&b);
        ops.destroy(&c);
        return 1;
    }
    
    if (ops.transpose(&a, &d)) {
        printf