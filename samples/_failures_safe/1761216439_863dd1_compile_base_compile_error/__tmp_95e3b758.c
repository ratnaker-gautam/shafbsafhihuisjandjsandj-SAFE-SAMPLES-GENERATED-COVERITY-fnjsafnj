//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

enum Operation {
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_TRANSPOSE,
    OP_INVALID
};

struct Matrix {
    size_t rows;
    size_t cols;
    int *data;
};

int matrix_init(struct Matrix *m, size_t rows, size_t cols) {
    if (rows == 0 || cols == 0 || rows > 100 || cols > 100) return 0;
    m->data = malloc(rows * cols * sizeof(int));
    if (m->data == NULL) return 0;
    m->rows = rows;
    m->cols = cols;
    return 1;
}

void matrix_free(struct Matrix *m) {
    if (m->data != NULL) free(m->data);
    m->data = NULL;
    m->rows = 0;
    m->cols = 0;
}

int matrix_read(struct Matrix *m) {
    for (size_t i = 0; i < m->rows; i++) {
        for (size_t j = 0; j < m->cols; j++) {
            if (scanf("%d", &m->data[i * m->cols + j]) != 1) return 0;
        }
    }
    return 1;
}

void matrix_print(const struct Matrix *m) {
    for (size_t i = 0; i < m->rows; i++) {
        for (size_t j = 0; j < m->cols; j++) {
            printf("%d ", m->data[i * m->cols + j]);
        }
        printf("\n");
    }
}

int matrix_add(const struct Matrix *a, const struct Matrix *b, struct Matrix *result) {
    if (a->rows != b->rows || a->cols != b->cols) return 0;
    if (!matrix_init(result, a->rows, a->cols)) return 0;
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < a->cols; j++) {
            result->data[i * a->cols + j] = a->data[i * a->cols + j] + b->data[i * a->cols + j];
        }
    }
    return 1;
}

int matrix_subtract(const struct Matrix *a, const struct Matrix *b, struct Matrix *result) {
    if (a->rows != b->rows || a->cols != b->cols) return 0;
    if (!matrix_init(result, a->rows, a->cols)) return 0;
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < a->cols; j++) {
            result->data[i * a->cols + j] = a->data[i * a->cols + j] - b->data[i * a->cols + j];
        }
    }
    return 1;
}

int matrix_multiply(const struct Matrix *a, const struct Matrix *b, struct Matrix *result) {
    if (a->cols != b->rows) return 0;
    if (!matrix_init(result, a->rows, b->cols)) return 0;
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < b->cols; j++) {
            int sum = 0;
            for (size_t k = 0; k < a->cols; k++) {
                sum += a->data[i * a->cols + k] * b->data[k * b->cols + j];
            }
            result->data[i * result->cols + j] = sum;
        }
    }
    return 1;
}

int matrix_transpose(const struct Matrix *a, struct Matrix *result) {
    if (!matrix_init(result, a->cols, a->rows)) return 0;
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < a->cols; j++) {
            result->data[j * result->cols + i] = a->data[i * a->cols + j];
        }
    }
    return 1;
}

enum Operation get_operation() {
    char input[16];
    if (fgets(input, sizeof(input), stdin) == NULL) return OP_INVALID;
    input[strcspn(input, "\n")] = 0;
    if (strcmp(input, "add") == 0) return OP_ADD;
    if (strcmp(input, "subtract") == 0) return OP_SUBTRACT;
    if (strcmp(input, "multiply") == 0) return OP_MULTIPLY;
    if (strcmp(input, "transpose") == 0) return OP_TRANSPOSE;
    return OP_INVALID;
}

int main(void) {
    struct Matrix a, b, result;
    enum Operation op;
    size_t rows, cols;

    printf("Enter operation (add/subtract/multiply/transpose): ");
    op = get_operation();
    if (op == OP_INVALID) {
        printf("Invalid operation\n");
        return 1;
    }

    if (op == OP_TRANSPOSE) {
        printf("Enter matrix rows and columns: ");
        if (scanf("%zu %zu", &rows, &cols) !=