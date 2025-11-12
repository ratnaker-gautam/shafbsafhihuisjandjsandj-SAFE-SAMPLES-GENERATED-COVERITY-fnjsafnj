//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

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
    double *data;
};

int matrix_init(struct Matrix *m, int rows, int cols) {
    if (rows <= 0 || cols <= 0) return 0;
    m->rows = rows;
    m->cols = cols;
    m->data = malloc(rows * cols * sizeof(double));
    return m->data != NULL;
}

void matrix_free(struct Matrix *m) {
    free(m->data);
    m->data = NULL;
}

int matrix_read(struct Matrix *m) {
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            if (scanf("%lf", &m->data[i * m->cols + j]) != 1) {
                return 0;
            }
        }
    }
    return 1;
}

void matrix_print(const struct Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%8.2f", m->data[i * m->cols + j]);
        }
        printf("\n");
    }
}

int matrix_add(const struct Matrix *a, const struct Matrix *b, struct Matrix *result) {
    if (a->rows != b->rows || a->cols != b->cols) return 0;
    if (!matrix_init(result, a->rows, a->cols)) return 0;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            result->data[i * a->cols + j] = a->data[i * a->cols + j] + b->data[i * a->cols + j];
        }
    }
    return 1;
}

int matrix_subtract(const struct Matrix *a, const struct Matrix *b, struct Matrix *result) {
    if (a->rows != b->rows || a->cols != b->cols) return 0;
    if (!matrix_init(result, a->rows, a->cols)) return 0;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            result->data[i * a->cols + j] = a->data[i * a->cols + j] - b->data[i * a->cols + j];
        }
    }
    return 1;
}

int matrix_multiply(const struct Matrix *a, const struct Matrix *b, struct Matrix *result) {
    if (a->cols != b->rows) return 0;
    if (!matrix_init(result, a->rows, b->cols)) return 0;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            double sum = 0.0;
            for (int k = 0; k < a->cols; k++) {
                sum += a->data[i * a->cols + k] * b->data[k * b->cols + j];
            }
            result->data[i * b->cols + j] = sum;
        }
    }
    return 1;
}

int matrix_transpose(const struct Matrix *a, struct Matrix *result) {
    if (!matrix_init(result, a->cols, a->rows)) return 0;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            result->data[j * a->rows + i] = a->data[i * a->cols + j];
        }
    }
    return 1;
}

int get_matrix_dimensions(int *rows, int *cols) {
    printf("Enter matrix dimensions (rows cols): ");
    if (scanf("%d %d", rows, cols) != 2) return 0;
    return *rows > 0 && *cols > 0;
}

int main(void) {
    enum MatrixOp op;
    int running = 1;
    
    while (running) {
        printf("\nMatrix Operations:\n");
        printf("0: Add\n");
        printf("1: Subtract\n");
        printf("2: Multiply\n");
        printf("3: Transpose\n");
        printf("4: Exit\n");
        printf("Select operation: ");
        
        int op_input;
        if (scanf("%d", &op_input) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }
        
        if (op_input < 0 || op_input > 4) {
            printf("Invalid operation\n");
            continue;
        }
        
        op = (enum MatrixOp)op_input;
        
        switch (op) {
            case OP_ADD:
            case OP_SUBTRACT: {
                struct Matrix a, b, result;
                printf("First matrix:\n");
                if (!get_matrix_dimensions(&a.rows, &a.cols) || !matrix_init(&a, a.rows